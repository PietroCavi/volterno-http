#ifndef __RESOURCES_CAVI__
#define __RESOURCES_CAVI__

#include <stddef.h>

typedef enum{
    DONE
}function_code;

typedef struct{
    char* server_path;
    char* real_path;
    function_code(*function)(int,char*);
}http_resource;

extern size_t get_resources_len;
extern size_t post_resources_len;
extern size_t delete_resources_len;
extern size_t put_resources_len;
extern size_t patch_resources_len;

extern size_t get_dir_resources_len;
extern size_t post_dir_resources_len;
extern size_t delete_dir_resources_len;
extern size_t put_dir_resources_len;
extern size_t patch_dir_resources_len;

extern http_resource base;
extern http_resource get_resources[];
extern http_resource post_resources[];
extern http_resource delete_resources[];
extern http_resource put_resources[];
extern http_resource patch_resources[];

extern http_resource get_dir_resources[];
extern http_resource post_dir_resources[];
extern http_resource delete_dir_resources[];
extern http_resource put_dir_resources[];
extern http_resource patch_dir_resources[];

#endif
