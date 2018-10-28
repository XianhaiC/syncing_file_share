#ifndef TF_NODE_H
#define TF_NODE_H

#include "list.h"

typedef struct _tf_node tf_node;

struct _tf_node {
    char *p_abs;
    char *p_base;
    tf_node* parent;
    list *children;
};

tf_node *tf_node_init(tf_node *parent, char *p_abs, char *p_base);
tf_node *tf_node_find_base(tf_node *root, char *base);
tf_node *tf_node_delete(tf_node *root);
tf_node *tf_node_delete_helper(tf_node *root);

#endif
