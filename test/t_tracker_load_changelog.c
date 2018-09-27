#include "test.h"
#include "tracker.h"

int main() {
    // generic vars
    int i;

    char *path = "test/.t_changelog";
    FILE *fp;
    list *changelog;

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

    fp = fopen(path, "w");

    // create a temp changelog
    for (i = 0; i < size_arr; i++) {
        fputs(arr[i], fp); 
        fputc('\0', fp);
    } 
    
    fclose(fp);    
    
    changelog = load_changelog(path);
    
    for (i = 0; i < changelog->size; i++) {
        TEST(strcmp(changelog->data[i], arr[i]) == 0);
    }

    remove(path);
}
