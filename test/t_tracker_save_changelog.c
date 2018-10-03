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

    list_init(&changelog, &data_free_string);
    
    for (i = 0; i < size_arr; i++) {
        list_append(&changelog, arr[i]);
    }

    save_changelog(path, &changelog);
    
    changelog_new = load_changelog(path);

    for (i = 0; i < changelog_new->size; i++) {
        TEST(strcmp(changelog_new->data[i], arr[i]) == 0);
    }

    remove(path);
}
