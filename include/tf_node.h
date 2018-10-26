#ifndef TF_NODE_H
#define TF_NODE_H

typedef struct _tf_node tf_node;

struct _tf_node {
    char *p_abs;
    char *p_base;
    tf_node* parent;
    list *children;
};

#endif
