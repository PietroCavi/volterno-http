#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

// These are the default values if we want the define
// to take action we have to declare them before the include
#define PORT 8080
#define MAX_HEADER_SIZE 8192

#include "volterno_http.h"


// The return of this function is never checked
void* serve(void* socket){
    int new_socket = ((int*)socket)[0];
    free(socket);

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
        return NULL;
    }

    if(strstr(header,"GET")!=NULL){
        handle_get(new_socket,header);
    }
    else if(strstr(header,"POST")!=NULL){
        handle_post(new_socket,header);
    }
    else if(strstr(header,"PUT")!=NULL){
        handle_put(new_socket,header);
    }
    else if(strstr(header,"DELETE")!=NULL){
        handle_delete(new_socket,header);
    }
    else if(strstr(header,"PATCH")!=NULL){
        handle_patch(new_socket,header);
    }
        
    //write(new_socket, "All good", strlen("All good"));
    close(new_socket); 

    printf("%s",header);
    
    return NULL;
}

int main(){

    signal(SIGPIPE, SIG_IGN);

    int server_fd;
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

    pthread_t t;
    int* thread_args;
    while(1){
        thread_args = malloc(sizeof(int));
        if((thread_args[0] = accept(server_fd,(struct sockaddr*)&address,&addrlen))<0){
            log_error("accept failed");
        }
    
        pthread_create(&t,NULL,serve,thread_args);

    }
    
    close(server_fd);
    return 0;

}

