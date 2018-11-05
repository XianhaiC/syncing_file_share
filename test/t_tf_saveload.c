#include "test.h"
#include "tree_file.h"
#include "tf_node.h"

int main() {
    char buf[BUF_LEN];
    char *path_load = "test/data/d_files";
    char *path_save = "test/data/d_files_copy";
    FILE *fp;

    tree_file *tree_load;
    tree_file *tree_save;
    tf_node *node_found;

    fp = fopen(path_load, "r");
    tree_load = tf_init();

    while (fgets(buf, BUF_LEN, fp)) {
        tf_insert(tree_load, buf);
    }

    // save the tree_file to file and read it back in
    tf_save(tree_load, path_save); 
    tree_save = tf_load(path_save);

    // perform tests
    fseek(fp, 0, SEEK_SET);

    while (fgets(buf, BUF_LEN, fp)) {
        node_found = tf_find(tree_save, buf);
        TEST(node_found != NULL);
        if (node_found != NULL) {
            TEST(strcmp(node_found->p_abs, buf) == 0);
        }
    }

    TEST(tf_find(tree_save, "/dasdad") == NULL);
    TEST(tf_find(tree_save, "/bin/dsadad") == NULL);
    TEST(tf_find(tree_save, "/dadsadfer") == NULL);
    TEST(tf_find(tree_save, "/hello") == NULL);
    TEST(tf_find(tree_save, "/whatsup") == NULL);
    TEST(tf_find(tree_save, "/mist/non") == NULL);
    TEST(tf_find(tree_save, "/misc/non") == NULL);
    TEST(tf_find(tree_save, "/bin/prog69") == NULL);
    TEST(tf_find(tree_save, "/misc/thin6") == NULL);
    TEST(tf_find(tree_save, "/wqree") == NULL);
    TEST(tf_find(tree_save, "/bloo") == NULL);
    TEST(tf_find(tree_save, "/qwerty") == NULL);
    TEST(tf_find(tree_save, "/base/a/b/c/d/e") == NULL);
}
