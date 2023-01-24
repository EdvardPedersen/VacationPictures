#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <libexif/exif-data.h>

#define SEPERATOR "/"

typedef struct list list_t;
typedef struct listnode listnode_t;

struct list {
    listnode_t *head;
}

struct listnode {
    void *item;
    listnode_t *next;
}

list_t *make_list() {
    list_t *list = malloc(sizeof(list_t));
    list->head = NULL;
    return list;
}

int list_add(list_t list, void *item) {
    listnode_t *node = list->head;
    if(!node) {
        list->head = make_node(item);
        return 1;
    }
    while(node->next != NULL) {
        node = node->next;
    }
    node->next = make_node(item);
}

listnode_t *make_node(void *item) {
    listnode_t *cur = malloc(sizeof(listnode_t));
    cur->item = item;
    cur->next = NULL;
    return cur;
}

list_t *read_images(const char *dir) {
    DIR *current_dir = opendir(dir);
    struct dirent *entry = NULL;
    list_t *our_list = make_list();

    while(entry = readdir(current_dir)) {
        if(entry->d_name[0] != '.') {
            char *full_path = calloc(sizeof(char), strlen(dir) * strlen(entry->d_name) + 2);
            strcpy(full_path, dir);
            strcpy(full_path + strlen(full_path), SEPERATOR);
            strcpy(full_path + strlen(full_path), entry->d_name);
            printf("READING %s\n", full_path);
            ExifData *image_data = exif_data_new_from_file(full_path);
            list_add(our_list, image_data);
        }
    }
    return our_list;
}

int main() {
    read_images("images");
}
