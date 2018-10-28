#include "tree_file.h"

// create new tree
tree_file *tf_init() {
    tree_file *tf = (tree_file *) malloc(sizeof(tree_file));

    tf->size = 0;
    tf->height = 0;

    // create new node
    tf->root = (tf_node *) malloc(sizeof(tf_node));
    tf->root->parent = NULL;
    tf->root->p_base = strdup("");
    tf->root->p_abs = strdup("");
    
    // create empty list for children
    tf->root->children = list_init(
            LIST_INIT_LEN, 
            &data_free_tf_node, 
            &data_comp_tf_node);

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
    
    // if no root, create the root dir
    if (tf->root == NULL) {
        tf->root = tf_node_init(NULL, "", e);
        return 1;
    }

    // slice path into segments
    list_seg = tf_slice(e);

    n_curr = tf->root;

    // add path to tree by inserting seps
    for (i = 0; i < list_seg->size; i++) {
        // determine if n_curr has the segment as a child
        // if so, then the path to the segment exists and can be further
        // traversed
        // TODO something seems to be wrong with this
        n_next = tf_node_find_base(n_curr, list_get(list_seg, i));

        // create new segment node if it doesnt exist
        if (n_next == NULL) {
            n_next = tf_node_init(
                    n_curr, 
                    n_curr->p_abs, 
                    list_get(list_seg, i));

            list_append(n_curr->children, n_next);

            // move onto next seg
            n_curr = n_next;
            fl_inserted = 1;
        }
        // else move down it
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

    list_seg = tf_slice(e);

    n_curr = tf->root;

    // add path to tree by inserting seps
    for (i = 0; i < list_seg->size; i++) {
        // determine if n_curr has the segment as a child
        // if so, then the path to the segment exists and can be further
        // traversed
        n_next = tf_node_find_base(n_curr, list_get(list_seg, i));

        // if n_next is null, then e is not in the tree
        if (n_next == NULL) {
            return NULL;
        }
        // else continue to search down n_next
        else {
            n_curr = n_next;
        }
    }

    return n_curr;
}

// write tree to disk

// read tree from disk

// helper functions

// slice path into individual segments and return an in order array containing
// them.
// expects e to be absolute path from the sync_root, beggining with '/'
// the returned list will not contain root
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
    
    tmp_f = e;

    // loop through all sep chars, where each precedes a path segment
    while (tmp_f = strchr(tmp_f, TF_SEP_CHAR)) {
        // increment tmp_f to point to seg
        tmp_f++;

        // find pointer to the next sep or end of path 
        if (!(tmp_e = strchr(tmp_f, TF_SEP_CHAR))) {
            // strip any newlines
            tmp_f[strcspn(tmp_f, "\n")] = '\0';

            // set tmp_e to point to end of path if no more seps to parse
            tmp_e = strchr(tmp_f, '\0');
        }

        // calculate length of seg
        len_seg = tmp_e - tmp_f;

        // dont add empty segs
        if (len_seg < 1) continue;

        // allocate space for it
        seg = (char *) malloc(sizeof(char) * (len_seg + 1));

        // copy seg over into memory
        strncpy(seg, tmp_f, len_seg);

        // teminate it
        seg[len_seg] = '\0';

        // append seg pointer to list
        list_append(list_seg, seg);
    }

    return list_seg;
}
