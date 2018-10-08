#include <stdlib.h>
#include "test.h"
#include "tracker.h"

// finish this
// current error, make sure not to free strings on stack
int main() {
    // generic vars
    int i;

    char *path = "test/.t_changelog";
    FILE *fp;
    list changelog;
    list *changelog_new;
    sync_file_update * sfu;

    // array to store
    char *arr[] = {
        "helloworld.txt",
        "worldhello.txt",
        "china.txt",
        "japan.txt",
        "russia.txt",
        "usa.txt",
        "1234532456.txt"
    };
    int size_arr = sizeof(arr) / sizeof(char *);

    srand(420);

    list_init(&changelog, &data_free_string);
    
    for (i = 0; i < size_arr; i++) {
        sfu = (sync_file_udpate *) calloc(1, sizeof(sync_file_update));

        SHA1(arr[i], strlen(arr[i]), sfu->hash);
        sfu->del = rand() % 2;

        list_append(&changelog, sfu);
    }

    save_changelog(path, &changelog);
    
    changelog_new = load_changelog(path);

    for (i = 0; i < changelog_new->size; i++) {
        TEST(strcmp(changelog_new->data[i], arr[i]) == 0);
    }

    remove(path);
}
