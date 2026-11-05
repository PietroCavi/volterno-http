#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <signal.h>

#define PORT 80

const char html_header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n";
const char css_header [] = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n";
const char js_header  [] = "HTTP/1.1 200 OK\r\nContent-Type: text/javascript\r\n";
const char wasm_header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/wasm\r\n";
const char json_header[] = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";
const char jpeg_header[] = "HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n";

char* not_found   = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    
char* transform_image(char*);

typedef struct{

    char* http_location;
    char* file_name;
    char* (*extra_args)(char*);
    const char* header;

}http_GET;

typedef struct{

    char* http_location;
    void (*process_post)(char*,int);
    const char* header;

}http_POST;

http_POST POST_requests[]={

};

http_GET GET_requests[]={
    
    {
        .http_location = "/",
        .file_name     = "index.html",
        .extra_args    = NULL,
        .header        = html_header
    },
    {
        .http_location = "/style.css",
        .file_name     = "style.css",
        .extra_args    = NULL,
        .header        = css_header
    },
    {
        .http_location = "/script.js",
        .file_name     = "script.js",
        .extra_args    = NULL,
        .header        = js_header
    },
    
    {
        .http_location = "/worms",
        .file_name     = "worms/index.html",
        .extra_args    = NULL,
        .header        = html_header
    },
    {
        .http_location = "/worms/style.css",
        .file_name     = "worms/style.css",
        .extra_args    = NULL,
        .header        = css_header
    },
    {
        .http_location = "/worms/script.js",
        .file_name     = "worms/script.js",
        .extra_args    = NULL,
        .header        = js_header
    },
    
    {
        .http_location = "/conway",
        .file_name     = "conway/index.html",
        .extra_args    = NULL,
        .header        = html_header
    },
    {
        .http_location = "/conway/style.css",
        .file_name     = "conway/style.css",
        .extra_args    = NULL,
        .header        = css_header
    },
    {
        .http_location = "/conway/script.js",
        .file_name     = "conway/script.js",
        .extra_args    = NULL,
        .header        = js_header
    },
    
    {
        .http_location = "/conway+",
        .file_name     = "conway+/index.html",
        .extra_args    = NULL,
        .header        = html_header
    },
    {
        .http_location = "/conway+/style.css",
        .file_name     = "conway+/style.css",
        .extra_args    = NULL,
        .header        = css_header
    },
    {
        .http_location = "/conway+/script.js",
        .file_name     = "conway+/script.js",
        .extra_args    = NULL,
        .header        = js_header
    },

    {
        .http_location = "/volt",
        .file_name     = "volt/index.html",
        .extra_args    = NULL,
        .header        = html_header
    },
    {
        .http_location = "/volt/style.css",
        .file_name     = "volt/style.css",
        .extra_args    = NULL,
        .header        = css_header
    },
    {
        .http_location = "/volt/output.jpeg",
        .file_name     = "volt/output.jpeg",
        .extra_args    = transform_image,
        .header        = jpeg_header
    }
};

char* transform_image(char* in){
    pid_t p;
    p = fork();
    if(p<0){
        printf("fork fail\n");
    }
    else if(p==0){
        system("java -cp volt transformer");
        exit(0);
    }

    char* ret = malloc(sizeof(char));
    ret[0] = '\0';

    return ret;
}

long int find_file_size(char file_name[]){
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("%s - File Not Found!\n",file_name);
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);
    fclose(fp);
    return res;
}

void send_file_http(const char* header,char* (extra_args)(char*),char* file_name, int client_socket,char* full_request){

    if(header==NULL || file_name == NULL){
        send(client_socket, not_found, strlen(not_found),0);
        return;
    }

    long int file_size = find_file_size(file_name);
    int file_content = open(file_name,O_RDONLY);
    
    char* args;
    if(extra_args!=NULL){
        args = extra_args(full_request);
    }
    else{
        args = malloc(1);
        args[0]='\0';
    }

    send(client_socket, header, strlen(header), 0);
    send(client_socket, args  , strlen(args),   0);
    send(client_socket, "\r\n", strlen("\r\n"), 0);

    free(args);

    if(file_size != -1 && file_content != -1){
        sendfile(client_socket,file_content,NULL,file_size);
        close(file_content);
    }

}

void* serve(void*socket){
    int client_socket = ((int*)socket)[0];
    free(socket);
    ssize_t valread;
    char prov_buffer[1024] = { 0 };
    char buffer[1024] = { 0 };
    
    valread = read(client_socket, prov_buffer, 1024 - 1); 
    printf("%s\n", prov_buffer);

    sscanf(prov_buffer,"%s",buffer);

    if(strcmp(buffer,"GET")==0){
        sscanf(prov_buffer,"%s %s",buffer,buffer);
        unsigned char sent = 0;
        for(size_t i=0;i<sizeof(GET_requests)/sizeof(GET_requests[0]);i++){
            if(strcmp(buffer, GET_requests[i].http_location)==0){
                send_file_http(
                    GET_requests[i].header,
                    GET_requests[i].extra_args,
                    GET_requests[i].file_name,
                    client_socket,
                    prov_buffer
                );

                sent = 1;
                break;
            }
        }

        if(sent == 0){
            send_file_http(
                NULL,
                NULL,
                NULL,
                client_socket,
                ""
            );
        }

    }
    else if(strcmp(buffer,"POST")==0){
        sscanf(prov_buffer,"%s %s",buffer,buffer);
        unsigned char sent = 0;
        for(size_t i=0;i<sizeof(POST_requests)/sizeof(POST_requests[0]);i++){
            if(strcmp(buffer, POST_requests[i].http_location)==0){
                send(client_socket,POST_requests[i].header,strlen(POST_requests[i].header),0);
                POST_requests[i].process_post(prov_buffer,client_socket);
                sent = 1;
                break;
            }
        }

        if(sent == 0){
            send_file_http(
                NULL,
                NULL,
                NULL,
                client_socket,
                ""
            );
        }
    
    }



    // closing the connected socket
    close(client_socket);

}

int main(int argc, char const* argv[])
{

    signal(SIGPIPE, SIG_IGN);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int* thread_args;
    pthread_t t;
    while(1){

        if (listen(server_fd, 40) < 0) {
            perror("listen");
            exit(1);
            continue;
        }
        
        thread_args = malloc(sizeof(int));
        if(thread_args == NULL){
            perror("malloc");
            continue;
        }
        
        if ((thread_args[0]
             = accept(server_fd, (struct sockaddr*)&address,
                      &addrlen))
            < 0) {
            perror("accept");
            continue;
        }

        pthread_create(&t,NULL,serve,thread_args);

    }
     
    // closing the listening socket
    close(server_fd);
    return 0;
}
