#ifndef TREE_FILE
#define TREE_FILE

typedef struct _tf_node tf_node;
typedef struct _tree_file tree_file;

struct _tf_node {
    char *p_abs;
    char *p_base;
    tf_node* parent;
    list *children;
};

struct _tree_file {
    tf_node *root;
    unsigned int size;
    unsigned int height;
};


#endif
