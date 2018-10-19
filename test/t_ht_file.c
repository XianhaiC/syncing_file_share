#include <uuid/uuid.h>
#include "test.h"
#include "ht_file.h"

void print_ht(ht_file *ht) {
    int i, j;
    ht_node *node_curr;

    for (i = 0; i < ht->cap; i++) {
        node_curr = ht->list[i];
        
        if (node_curr) {
            fprintf(stderr, "HASH INDEX: %d\n", i);
        }

        // go through the linked list
        while (node_curr) {
            fprintf(stderr, "uuid: ");
            for (j = 0; j < LEN_UUID_T; j++) {
                fprintf(stderr, "%02x", node_curr->key[j]);
            }
            fprintf(stderr, " inode: %u\n", node_curr->val);

            node_curr = node_curr->next;
        }
    }
}

void print_item(uuid_t id, int val, int i) {
    for (int j = 0; j < LEN_UUID_T; j++) {
        fprintf(stderr, "%02x", id[j]);
    }
    fprintf(stderr, " inode: %u index: %d\n", val, i);

}

int main() {
    int i, j;
    char *path = "test/data/d_hashtable";
    FILE *fp;
    uuid_t id;
    uuid_t id_test;
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
    int items = 0;
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);
        
        ht_file_insert(ht, id, inode);
        items++;
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

    // check that hashtable returns NULL for values not in table
    for (i = 0; i < 50; i++) {
        uuid_generate(id_test);
        TEST(ht_file_lookup(ht, id_test) == NULL);
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
