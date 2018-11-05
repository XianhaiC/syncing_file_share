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

    TEST(tf_find(tf, "/dasdad") == NULL);
    TEST(tf_find(tf, "/bin/dsadad") == NULL);
    TEST(tf_find(tf, "/dadsadfer") == NULL);
    TEST(tf_find(tf, "/hello") == NULL);
    TEST(tf_find(tf, "/whatsup") == NULL);
    TEST(tf_find(tf, "/mist/non") == NULL);
    TEST(tf_find(tf, "/misc/non") == NULL);
    TEST(tf_find(tf, "/bin/prog69") == NULL);
    TEST(tf_find(tf, "/misc/thin6") == NULL);
    TEST(tf_find(tf, "/wqree") == NULL);
    TEST(tf_find(tf, "/bloo") == NULL);
    TEST(tf_find(tf, "/qwerty") == NULL);
    TEST(tf_find(tf, "/base/a/b/c/d/e") == NULL);
}
