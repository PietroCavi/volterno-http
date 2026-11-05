#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "timerino/timerino.h"

#define PORT 8080

#define MAX_HEADER_SIZE 8192

typedef enum{
    HEADER_GOOD,
    HEADER_TOO_BIG,
    HEADER_FAIL,
    HEADER_TIMEOUT
}header_code;

header_code get_header(int new_socket, char* header, size_t header_size){
        ssize_t total_peeked_bytes = 0;
        ssize_t peeked_bytes = 0;
        
        ssize_t total_read_bytes= 0;
        ssize_t read_bytes = 0;

        char* needle_addr = NULL;
        double elapsed_time = 0;
    
        timerone timeout_timer = {0};

        struct timeval tv = { .tv_sec = 1, .tv_usec = 0 };
        setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
        for(;total_peeked_bytes < header_size && needle_addr == NULL;){
            
            start_timerone(&timeout_timer);
            
            peeked_bytes = recv(new_socket, header+total_peeked_bytes, header_size - total_peeked_bytes, MSG_PEEK);
            if(peeked_bytes <= 0){ 
                int e = errno;                                   // save immediately
                if(e == EAGAIN) 
                    return HEADER_TIMEOUT;
                return HEADER_FAIL;
            }

            total_peeked_bytes += peeked_bytes;
            header[total_peeked_bytes] = '\0';

            needle_addr = strstr(header,"\r\n\r\n");
            if(needle_addr != NULL){
                (needle_addr + strlen("\r\n\r\n"))[0] = '\0';
                
                read_bytes = read(new_socket, header+total_read_bytes, strlen(header)-total_read_bytes);
            }
            else{
                read_bytes = read(new_socket, header+total_read_bytes, peeked_bytes);
            }
            if(read_bytes <= 0){ 
                int e = errno;                                   // save immediately
                if(e == EAGAIN) 
                    return HEADER_TIMEOUT;
                return HEADER_FAIL;
            }
            
            total_read_bytes += read_bytes;
            
            stop_timerone(&timeout_timer);
            elapsed_time += get_timerone(&timeout_timer, "ms");
            if(elapsed_time > 1000){ return HEADER_TIMEOUT; }
        }
        if(needle_addr == NULL){
            return HEADER_TOO_BIG;
            exit(EXIT_FAILURE);
        }

        return HEADER_GOOD;
}

static void send_error(int sock, const char *status_line) {
    char buf[256];
    int n = snprintf(buf, sizeof buf,
        "HTTP/1.1 %s\r\n"
        "Content-Length: 0\r\n"
        "Connection: close\r\n"
        "\r\n", status_line);
    send(sock, buf, n, MSG_NOSIGNAL);
}

void throw_error(char* error_msg){

    fprintf(stderr,"%s\n",error_msg);

    exit(EXIT_FAILURE);

}

int main(int argc, char const* argv[]){

    signal(SIGPIPE, SIG_IGN);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw_error("socket failed");
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        throw_error("setsockopt");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        throw_error("bind failed");
    }
        
    if (listen(server_fd, 40) < 0) {
        throw_error("listen");
    }
    
    if((new_socket = accept(server_fd,(struct sockaddr*)&address,&addrlen))<0){
        throw_error("accept");
    }
    
    //----- This will be inside a thread -----//    
    
    char header[MAX_HEADER_SIZE+1] = {0};
    
    header_code header_status = get_header(new_socket, header, MAX_HEADER_SIZE);
    
    if(header_status != HEADER_GOOD){
        switch(header_status){
            case HEADER_TOO_BIG: send_error(new_socket, "431 Request Header Fields Too Large"); break;
            case HEADER_TIMEOUT: send_error(new_socket, "408 Request Timeout"); break;
            case HEADER_FAIL:    send_error(new_socket, "500 Internal Server Error"); break;
            default: break;
        }
        close(new_socket);
        return 1;
        //continue;
    }

    write(new_socket, "All good", strlen("All good"));
    close(new_socket); 

    printf("%s",header);
    
    //----------------------------------------//    

    close(server_fd);
    return 0;

}

