#include "test.h"
#include "tracker.h"
#include "ht_file.h"

int main() {
    char *path_load = "test/data/d_hashtable";
    char *path_save = "test/data/d_hashtable_copy";
    FILE *fp;
    unsigned int inode;
    uuid_t id;
    ht_file *ht_load;
    ht_file *ht_save;

    fp = fopen(path_load, "r");
    ht_load = load_inode_res(path_load);
    
    // test loaded hashtable
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);

        TEST(*((unsigned int *) ht_file_lookup(ht_load, id)) == inode);
    }

    // save hashtable to new location
    save_inode_res(ht_load, path_save);
    ht_save = load_inode_res(path_save);

    // test hashtable loaded with saved data
    fseek(fp, 0, SEEK_SET);
    while (1) {
        fread(id, sizeof(uuid_t), 1, fp);
        if (feof(fp)) {
            break;
        }
        fread(&inode, sizeof(unsigned int), 1, fp);

        TEST(*((unsigned int *) ht_file_lookup(ht_save, id)) == inode);
    }
}
