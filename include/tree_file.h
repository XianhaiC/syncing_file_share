#ifndef TREE_FILE
#define TREE_FILE

typedef struct _tree_file tree_file;

struct _tree_file {
    tf_node *root;
    unsigned int size;
    unsigned int height;
};

#endif
