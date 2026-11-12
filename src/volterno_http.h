#ifndef __VOLTERNO_HTTP_CAVI__
#define __VOLTERNO_HTTP_CAVI__

#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "timerino/timerino.h"

#ifndef PORT
    #define PORT 8080
#endif

#ifndef MAX_HEADER_SIZE
    #define MAX_HEADER_SIZE 8192
#endif

#ifndef MAX_HEADER_SIZE
    #define MAX_HEADER_SIZE 8192
#endif

typedef enum{
    HEADER_GOOD,
    HEADER_TOO_BIG,
    HEADER_FAIL,
    HEADER_TIMEOUT
}header_code;

header_code get_header(int new_socket, char* header, size_t header_size);

void handle_get(int socket, char* header);

void handle_post(int socket, char* header);

void handle_put(int socket, char* header);

void handle_delete(int socket, char* header);

void handle_patch(int socket, char* header);

void send_error(int sock, const char *status_line);

void throw_error(char* error_msg);

void log_error(char* error_msg);

#endif
