#ifndef TREE_FILE
#define TREE_FILE

#include "tf_node.h"

typedef struct _tree_file tree_file;

struct _tree_file {
    tf_node *root;
    unsigned int size;
    unsigned int height;
};

tree_file *tf_init();
int tf_insert(tree_file *tf, char *e);
tf_node *tf_find(tree_file *tf, char *e);
list *tf_slice(char *e);

#endif
