#include "test.h"
#include "tree_file.h"
#include "tf_node.h"

int main() {
    char buf[BUF_LEN];
    char *path = "test/data/d_files";
    FILE *fp;

    tree_file *tf;
    tf_node *node_found;

    fp = fopen(path, "r");
    tf = tf_init();

    while (fgets(buf, BUF_LEN, fp)) {
        tf_insert(tf, buf);
    }

    fseek(fp, 0, SEEK_SET);

    while (fgets(buf, BUF_LEN, fp)) {
        node_found = tf_find(tf, buf);
        TEST(node_found != NULL);
        if (node_found != NULL) {
            TEST(strcmp(node_found->p_abs, buf) == 0);
        }
    }
}
