#include <stdlib.h>
#include <openssl/sha.h>
#include "test.h"
#include "tracker.h"

int main() {
    // generic vars
    int i, j;

    char *path = "test/.t_changelog";
    FILE *fp;
    list changelog;
    list *changelog_new;
    sync_file_update *sfu;
    sync_file_update *sfu_new;

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

    fp = fopen(path, "w");
    list_init(&changelog, &data_free_sync_file_update);

    // create a temp changelog
    for (i = 0; i < size_arr; i++) {
        sfu = (sync_file_update *) calloc(1, sizeof(sync_file_update));

        SHA1(arr[i], strlen(arr[i]), sfu->hash);
        sfu->del = rand() % 2;
        fwrite(sfu, sizeof(sync_file_update), 1, fp);

        list_append(&changelog, sfu);

        // print out contents
        fprintf(stderr, "del and hash before: %d, ", sfu->del);
        for (j = 0; j < SHA_DIGEST_LENGTH; j++) {
            fprintf(stderr, "%02x", sfu->hash[j]);
        }
        fprintf(stderr, "\n");
    } 

    fprintf(stderr, "\n");
    fclose(fp);    
    
    changelog_new = load_changelog(path);
    
    for (i = 0; i < changelog.size; i++) {
        sfu = (sync_file_update *) list_get(&changelog, i);
        sfu_new = (sync_file_update *) list_get(changelog_new, i);

        // print out contents
        fprintf(stderr, "del and hash after : %d, ", sfu_new->del);
        for (j = 0; j < SHA_DIGEST_LENGTH; j++) {
            fprintf(stderr, "%02x", sfu_new->hash[j]);
        }
        fprintf(stderr, "\n");
    }

    for (i = 0; i < changelog.size; i++) {
        sfu = (sync_file_update *) list_get(&changelog, i);
        sfu_new = (sync_file_update *) list_get(changelog_new, i);

        TEST(strncmp(sfu->hash, sfu_new->hash, SHA_DIGEST_LENGTH) == 0);
        TEST(sfu->del == sfu_new->del);

    }

    remove(path);
}
