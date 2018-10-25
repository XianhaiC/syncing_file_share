#include "tree_file.h"

// create new tree
tree_file *tf_init() {
    tree_file *tf = (tree_file *) malloc(sizeof(tree_file));

    tf->size = 0;
    tf->height = 0;

    tf_insert(tf, TF_ROOT);

    return tf;
}
// delete tree
// insert into tree
int tf_insert(tree_file *tf, char *e) {
    int i;

    list *list_seg;
    tf_node *n_curr;
    tf_node *n_next;
    int fl_inserted = 0;
    
    // slice path into segments
    list_seg = tf_slice(tf, e);

    n_curr = tf->root;

    // add path to tree by inserting seps
    for (i = 0; i < list_seg->size; i++) {
        n_next = list_find(n_curr->children, list_seg.get(i));

        if (n_next == NULL) {
            // create new node for the seg and add it to n_curr's children list
            n_next = (tf_node *) malloc(sizeof(tf_node));
            n_next->parent = n_curr;
            n_next->p_base = list_seg.get(i);
            list_append(n_curr->children, n_next);

            // allocate space for new abs path, including an extra byte for the
            // sep char
            len_abs = strlen(n_curr->p_abs) + strlen(n_next->p_base) + 2;
            n_next->p_abs = (char *) malloc(sizeof(char) * len_abs);

            // create and store abs path in node
            // note: this copies null byte also
            strcpy(n_next->p_abs, n_curr->p_abs);
            strcat(n_next->p_abs, TF_SEP);
            strcat(n_next->p_abs, n_next->p_base); 

            // create empty list for children
            // TODO: create helper methods to support node lists
            n_next->children = list_init();

            // move onto next seg
            n_curr = n_next;
            fl_inserted = 1;
        }
        else {
            n_curr = n_next;
        }
    }

    return fl_inserted;
}

// find in tree
tf_node *tf_find(tree_file *tf, char *e) {
    int i;

    list *list_seg;
    tf_node *n_curr;
    tf_node *n_next;

    list_seg = tf_slice(tf, e);

    n_curr = tf->root;

    // add path to tree by inserting seps
    for (i = 0; i < list_seg->size; i++) {
        n_next = list_find(n_curr->children, list_seg.get(i));

        if (n_next == NULL) {
            
        }
        else {
            n_curr = n_next;
        }
    }
}
// remove from tree
// write tree to disk
// read tree from disk

// helper functions

// slice path into individual segments and return an in order array containing
// them.
// expects e to be absolute path from the sync_root, beggining with '/'
// the returned list will not contain '/'
// TODO: Fix end edge case where the base seg is not added since there are no
// more seg chars
list *tf_slice(char *e) {
    char *tmp_f;
    char *tmp_e;
    char *seg;
    unsigned int len_seg;
    list *list_seg;

    // create list of path segments including the root '/'
    // note: contents aren't freed when list is freed, they are still
    // referenced by node objects
    list_seg = list_init(
            LIST_INIT_LEN, 
            &data_free_string, 
            &data_comp_str);
    
    // set front pointer to the first non root sep seg
    tmp_f = strchr(e, TF_SEP) + 1;

    // append pointer to each dynamically allocated sep string to list_sep
    while (tmp_e = strchr(tmp_f, TF_SEP)) {
        // calculate length of seg
        len_seg = tmp_e - tmp_f;

        // allocate space for it
        seg = (char *) malloc(sizeof(char) * (len_seg + 1));

        // copy seg over into memory
        strncpy(seg, tmp_f, len_seg);

        // teminate it
        seg[len_seg] = '\0';

        // append seg pointer to list
        list_append(list_seg, seg);

        // reset the front pointer
        tmp_f = tmp_e + 1;
    }

    return list_seg;
}
