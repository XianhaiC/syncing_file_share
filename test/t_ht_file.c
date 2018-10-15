#include "test.h"
#include "ht_file.h"

int main() {
    int i, j;
    char *path = "test/data/d_hashtable";
    FILE *fp;
    uuid_t id;
    unsigned int inode;
    ht_file *ht = ht_file_init(HT_CAP_INIT, HT_THRESH);

    fp = fopen(path, "r");

    // fill hashtable with values
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);
        
        ht_file_insert(ht, id, inode);
    }

    // check if the hashtable holds all the inserted values
    fseek(fp, 0, SEEK_SET);
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);

        TEST(*((unsigned int *) ht_file_lookup(ht, id)) == inode);
    }

    // check remove function by removing all inserted nodes
    fseek(fp, 0, SEEK_SET);
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);

        TEST(ht_file_remove(ht, id));
    }

    TEST(ht->size == 0);

    // test dynamic expansion

    // create a new table with smaller capacity
    ht_file_free(ht);
    ht = ht_file_init(8, HT_THRESH);

    // fill hashtable with values
    fseek(fp, 0, SEEK_SET);
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);
        
        ht_file_insert(ht, id, inode);
    }

    // check if the hashtable holds all the inserted values
    fseek(fp, 0, SEEK_SET);
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);

        TEST(*((unsigned int *) ht_file_lookup(ht, id)) == inode);
    }

    // check remove function by removing all inserted nodes
    fseek(fp, 0, SEEK_SET);
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);

        TEST(ht_file_remove(ht, id));
    }

    TEST(ht->size == 0);

    ht_file_free(ht);
}
