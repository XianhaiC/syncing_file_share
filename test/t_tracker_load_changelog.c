#include "test.h"
#include "tracker.h"

int main() {
    // generic vars
    int i;

    FILE *fp;
    char **changelog;

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

    fp = fopen("test/t_changelog", "w");

    // create a temp changelog
    for (i = 0; i < size_arr; i++) {
        fputs(arr[i], fp); 
        fputc('\n', fp);
    } 
    
    fclose(fp);    
    
    changelog = load_changelog("test/t_changelog");
    
    for (i = 0; changelog[i] != NULL; i++) {
        TEST(changelog[i] == arr[i]);
    }
}
