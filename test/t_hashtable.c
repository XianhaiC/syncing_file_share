#include "test.h"
#include "hashtable.h"

int main() {
    char buf[BUF_LEN];
    FILE *fp;
    char *path = "TODO";
    char *sep = NULL;
    char *endptr = NULL;
    int val;
    hashtable ht = hashtable_init(HT_CAP_INIT, HT_THRESH,
            &hash_uuid, &hash_comp_uuid, &data_free_int);

    fp = fopen(path, "r");

    while (fgets(buf, BUF_LEN, fp) != NULL) {
        // set a null char where the key/val is seperated
        sep = strchr(buf, ' ');
        
        // modify buf to hold the key
        buf[sep] = '\0';

        // parse the int value
        val = strtol(sep + 1, &endptr, 10);

        // insert into table
        hashtable_insert(ht, buf, val);
    }

    fseek(fp, 0, SEEK_SET);

    while (fgets(buf, BUF_LEN, fp) != NULL) {
        // set a null char where the key/val is seperated
        sep = strchr(buf, ' ');
        
        // modify buf to hold the key
        buf[sep] = '\0';

        // parse the int value
        val = strtol(sep + 1, &endptr, 10);

        TEST(hashtable_lookup(ht, buf) == val);
    }
    
}
