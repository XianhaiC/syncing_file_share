#include "test.h"
#include "tracker.h"
#include <openssl/sha.h>

int main() {
    // generic vars
    int i;

    char *path = "test/.t_changelog";
    FILE *fp;
    unsigned char hash[SHA_DIGEST_LENGTH];
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
        SHA1(arr[i], strlen(arr[i]), hash);
        fprintf(stderr, "hash before: %s\n", hash);
        fwrite(hash, SHA_DIGEST_LENGTH, 1, fp);
    } 
    fprintf(stderr, "\n");
    fclose(fp);    
    
    changelog = load_changelog(path);
    
    for (i = 0; i < changelog->size; i++) {
        SHA1(arr[i], strlen(arr[i]), hash);
        fprintf(stderr, "hash after: %s\n", hash);
        TEST(strncmp(((sync_file_update *) list_get(changelog, i))->hash, hash, SHA_DIGEST_LENGTH) == 0);
    }

    remove(path);
}
