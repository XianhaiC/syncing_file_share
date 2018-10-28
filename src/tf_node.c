#include "tf_node.h"

tf_node *tf_node_init(tf_node *n_parent, char *p_abs, char *p_base) {
    unsigned int len_abs;

    // create new node
    tf_node *n_new = (tf_node *) malloc(sizeof(tf_node));
    n_new->parent = n_parent;

    // dup p_base over to node's p_base
    n_new->p_base = strdup(p_base);

    // allocate space for new abs path, including an extra byte for the
    // sep char
    len_abs = strlen(p_abs) + strlen(p_base) + 2;
    n_new->p_abs = (char *) malloc(sizeof(char) * len_abs);

    // create and store abs path in node
    // note: this copies null byte also
    strcpy(n_new->p_abs, p_abs);
    strcat(n_new->p_abs, TF_SEP_STR);
    strcat(n_new->p_abs, p_base); 

    // create empty list for children
    n_new->children = list_init(
            LIST_INIT_LEN, 
            &data_free_tf_node, 
            &data_comp_tf_node);

    return n_new;
}

tf_node *tf_node_find_base(tf_node *n_root, char *base) {
    int i;
    tf_node *n_query;
    
    // loop through n_roots children to see if there's a match
    for (i = 0; i < n_root->children->size; i++) {
        n_query = list_get(n_root->children, i);

        if (strncmp(base, n_query->p_base, STRCMP_LEN)) {
            return n_query;
        }
    }

    // not found
    return NULL;
}

// recursively delete n_root's children, then delete n_root
tf_node *tf_node_delete(tf_node *n_root) {
    // remove parent's reference to n_root node
    if (n_root->parent) {
        list_remove(n_root->parent->children, n_root);
    }

    // recursively free nodes starting at n_root
    tf_node_delete_helper(n_root);
}

tf_node *tf_node_delete_helper(tf_node *n_root) {
    int i;

    // perform a post order traversal and delete all children nodes first
    for (i = 0; i < n_root->children->size; i++) {
        tf_node_delete_helper(list_get(n_root->children, i));
    }

    // free n_root node members
    free(n_root->p_abs);
    free(n_root->p_base);

    // freeing the list will not affect the pointers that it holds due to the
    // dummy data_free
    list_free(n_root->children);

    free(n_root);
}
