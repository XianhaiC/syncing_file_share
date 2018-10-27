#include "tf_node.h"

tf_node *tf_node_init(tf_node *parent, char *p_abs, char *p_base) {
    unsigned int len_abs;

    // create new node
    tf_node *node_new = (tf_node *) malloc(sizeof(tf_node));
    node_new->parent = parent;
    node_new->p_base = p_base;

    // allocate space for new abs path, including an extra byte for the
    // sep char
    len_abs = strlen(p_abs) + strlen(p_base) + 2;
    node_new->p_abs = (char *) malloc(sizeof(char) * len_abs);

    // create and store abs path in node
    // note: this copies null byte also
    strcpy(node_new->p_abs, p_abs);
    strcat(node_new->p_abs, TF_SEP);
    strcat(node_new->p_abs, p_base); 

    // create empty list for children
    node_new->children = list_init(LIST_INIT_LEN, &data_free_tf_node, &data_comp_tf_node);

    return node_new;
}

tf_node *tf_node_find_base(tf_node *root, char *base) {
    int i;
    tf_node *node_query;
    
    // loop through roots children to see if there's a match
    for (i = 0; i < root->children->size; i++) {
        node_query = list_get(root->children, i);

        if (strncmp(base, node_query->p_base, STRCMP_LEN)) {
            return node_query;
        }
    }

    // not found
    return NULL;
}

// recursively delete root's children, then delete root
tf_node *tf_node_delete(tf_node *root) {
    // remove parent's reference to root node
    if (root->parent) {
        list_remove(root->parent->children, root);
    }

    // recursively free nodes starting at root
    tf_node_delete_helper(root);
}

tf_node *tf_node_delete_helper(tf_node *root) {
    int i;

    // perform a post order traversal and delete all children nodes first
    for (i = 0; i < root->children->size; i++) {
        tf_node_delete_helper(list_get(root->children, i));
    }

    // free root node members
    free(root->p_abs);
    free(root->p_base);

    // freeing the list will not affect the pointers that it holds due to the
    // dummy data_free
    list_free(root->children);

    free(root);
}
