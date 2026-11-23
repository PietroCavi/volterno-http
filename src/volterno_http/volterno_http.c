#include "volterno_http.h"

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
        for(;total_peeked_bytes < (ssize_t)header_size && needle_addr == NULL;){
            
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

void send_error(int sock, const char *status_line) {
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

void log_error(char* error_msg){

    fprintf(stderr,"%s\n",error_msg);

}

void get_resource(char* header,const char* method,char* resource_buf, size_t n){
    char* prov_header = strstr(header,method);
    prov_header += strlen(method)*sizeof(char);
    for(size_t i=0;prov_header[0]=='0' && i<strlen(header)-n;i++)prov_header++;
    memcpy(resource_buf,prov_header,n);
    resource_buf[n-1] = '\0';
    sscanf(resource_buf,"%s",resource_buf);
}

void handle_get(int socket, char* header){
    char resource[256] = {0};
    get_resource(header,"GET",resource,sizeof(resource));

    if(strcmp(resource,"/")==0){
        const char html_header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        const char body[] = "<!DOCTYPE html><html lang=\"en\"><body><p>hello</p></body></html>";
        write(socket,html_header,strlen(html_header));
        write(socket,body,strlen(body));
        
    }
}

void handle_post(int socket, char* header){
    (void)socket;
    (void)header;
}

void handle_put(int socket, char* header){
    (void)socket;
    (void)header;
}

void handle_delete(int socket, char* header){
    (void)socket;
    (void)header;
}

void handle_patch(int socket, char* header){
    (void)socket;
    (void)header;
}


