#include <stdlib.h>
#include <stdio.h>
#include <uuid/uuid.h>


#define DATA_SIZE 32
#define BUF_SIZ 1024
#define LEN_UUID_T 16

// creates an unsigned int within thr range [0, 2^30 -1]
unsigned int rand_large() {
    return (rand() << 15) | rand();

}

int main() {
    int i, j;
    char *path = "test/data/d_hashtable";
    FILE *fp;
    uuid_t id;
    unsigned int inode;

    srand(0);
    fp = fopen(path, "w");

    for (i = 0; i < DATA_SIZE; i++) {
        uuid_generate(id);
        inode = rand_large();
        fwrite(id, sizeof(uuid_t), 1, fp);
        fwrite(&inode, sizeof(unsigned int), 1, fp);  

        fprintf(stderr, "uuid: ");
        for (j = 0; j < LEN_UUID_T; j++) {
            fprintf(stderr, "%02x", id[j]);
        }
        fprintf(stderr, " inode: %u\n", inode);
    }

    fprintf(stdout, "\n");

    fclose(fp);

    fp = fopen(path, "r");

    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);

        fprintf(stderr, "uuid: ");
        for (j = 0; j < LEN_UUID_T; j++) {
            fprintf(stderr, "%02x", id[j]);
        }
        fprintf(stderr, " inode: %u\n", inode);
    }

    fclose(fp);
}
