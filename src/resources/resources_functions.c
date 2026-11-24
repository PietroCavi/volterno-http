#include "resources.h"

int get_get_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<get_resources_len;i++){
        if(strcmp(path,get_resources[i].server_path)==0){
            index = i;
        }
    }

    return index;
}

int get_post_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<post_resources_len;i++){
        if(strcmp(path,post_resources[i].server_path)==0){
            index = i;
        }
    }

    return index;
}

int get_delete_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<delete_resources_len;i++){
        if(strcmp(path,delete_resources[i].server_path)==0){
            index = i;
        }
    }

    return index;
}

int get_put_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<put_resources_len;i++){
        if(strcmp(path,put_resources[i].server_path)==0){
            index = i;
        }
    }

    return index;
}

int get_patch_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<patch_resources_len;i++){
        if(strcmp(path,patch_resources[i].server_path)==0){
            index = i;
        }
    }

    return index;
}

int get_get_dir_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<get_dir_resources_len;i++){
        if(strstr(get_dir_resources[i].server_path,path)==0){
            index = i;
        }
    }

    return index;
}

int get_post_dir_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<post_dir_resources_len;i++){
        if(strstr(post_dir_resources[i].server_path,path)==0){
            index = i;
        }
    }

    return index;
}

int get_delete_dir_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<delete_dir_resources_len;i++){
        if(strstr(delete_dir_resources[i].server_path,path)==0){
            index = i;
        }
    }

    return index;
}

int get_put_dir_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<put_dir_resources_len;i++){
        if(strstr(put_dir_resources[i].server_path,path)==0){
            index = i;
        }
    }

    return index;
}

int get_patch_dir_resource(char* path){
    int index = -1;
    
    for(size_t i=0;i<patch_dir_resources_len;i++){
        if(strstr(patch_dir_resources[i].server_path,path)==0){
            index = i;
        }
    }

    return index;
}
