#include "resources.h"

size_t get_resources_len = 0;
size_t post_resources_len = 0;
size_t delete_resources_len = 0;
size_t put_resources_len = 0;
size_t patch_resources_len = 0;
                      
size_t get_dir_resources_len = 0;
size_t post_dir_resources_len = 0;
size_t delete_dir_resources_len = 0;
size_t put_dir_resources_len = 0;
size_t patch_dir_resources_len = 0;

http_resource get_resources    [0];
http_resource post_resources   [0];
http_resource delete_resources [0];
http_resource put_resources    [0];
http_resource patch_resources  [0];

http_resource get_dir_resources    [0];
http_resource post_dir_resources   [0];
http_resource delete_dir_resources [0];
http_resource put_dir_resources    [0];
http_resource patch_dir_resources  [0];
