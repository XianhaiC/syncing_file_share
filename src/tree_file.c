#include "tree_file.h"

// create new tree
tree_file *tf_create() {
    tree_file *tf = (tree_file *) malloc(sizeof(tree_file));

    tf->size = 0;
    tf->height = 0;

    tf_insert(tf, TF_ROOT);

    return tf;
}
// delete tree
// insert into tree
int tf_insert(tree_file *tf, char *e) {
    list *p_seg;
    tf_node *curr = tf->root;
    
    // create list of path segments including the root '/'
    p_seg = 

    while
}
// find in tree
// remove from tree
// write tree to disk
// read tree from disk
