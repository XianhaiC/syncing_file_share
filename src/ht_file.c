#include "hash_map.h"

// TODO: ERROR HANDLING WITH MEMORY
// TODO: generalize so it can take anything as a key and store pointers

// initializes hashtable
hash_map *hash_map_init(int cap, float thresh) {
    int i;
    hash_map *hm = (hash_map *) malloc(sizeof(hash_map));
    hm->cap = cap;
    hm->size = 0;
    hm->thresh = thresh;
    hm->list = (hm_node **) calloc(cap, sizeof(hm_node *));

    return hm;
}

// free hashtable
void hash_map_free(hash_map *hm) {
    // free the nodes in the list
    hash_map_free_list(hm->list, hm->cap);

    free(hm);
}

void hash_map_free_list(hm_node **list, unsigned int cap) {
    int i;
    hm_node *node_tmp;
    hm_node *node_curr;

    // free the nodes in the list
    for (i = 0; i < cap; i++) {
        // free linked list
        node_curr = list[i];
        while (node_curr) {
            node_tmp = node_curr;
            node_curr = node_curr->next;
            free(node_tmp);
        }
    }

    free(list);
}
 
// inserts element into hashtable
// returns 0 upon replacement and 1 upon insertion
int hash_map_insert(hash_map *hm, void *key, void *val) {
    int i;
    unsigned int hash;
    hm_node *node_new;
 
    // place the node at the head as we don't care about dupes here

    // if the hashtable has surpassed expansion threshold then relocate elements
    // to a larger one
    if (hm->size >= (unsigned int) (hm->cap * hm->thresh)) {
        hash_map_expand(hm); 
 
        // calculate the new hash in the larger hashtable
    }

    hash = (*hm->hash)(key) % hm->cap;

    // initialize the new node with contents
    node_new = (hm_node *) malloc(sizeof(hm_node));

    node_new->key = key;
    node_new->val = val;

    // set it to be the new head
    node_new->next = hm->list[hash]
    hm->list[hash] = node_new;
    
    (hm->size)++;

    return 0;
}

// looks up an item in the hashtable
// returns a pointer to the val, null if not found
void *hash_map_lookup(hash_map *hm, uuid_t key) {
    unsigned int hash = (*hm->hash)(key) % hm->cap;
    hm_node *n_curr = hm->list[hash];

    // search through the linked list for key
    while (n_curr) {
        // check if the key exists within hm
        if ((*hm->data_comp)(key, n_curr->key)) {
            return &(n_curr->val);
        }
        n_curr = n_curr->next;
    }

    return NULL;
}

// removes a key val from the hashtable
// returns 1 upon removal, 0 otherwise
int hash_map_remove(hash_map *hm, uuid_t key) {
    unsigned int hash = (*hm->hash)(key) % hm->cap;
    hm_node *n_curr = hm->list[hash];
    // node pointer pointer that tracks the previous node's next
    hm_node **nodep_prev = &(hm->list[hash]);

    // search through the linked list for key
    while (n_curr) {
        if ((*hm->data_comp)(key, n_curr->key)) {
            // link previous node with next
            *nodep_prev = node_curr->next;

            // free the node
            free(node_curr);
            (hm->size)--;
            
            // set to next node
            n_curr = *nodep_prev;

            continue;
        }
        // update node pointer pointer
        nodep_prev = &node_curr->next;
        node_curr = node_curr->next;
    }

    return 0;
}

// creates a new hashtable of double size and relocates elements in the old one
// returns the capacity of the new hashtable
int hash_map_expand(hash_map *hm) {
    int i;
    hm_node *node_curr;
    hm_node ** list_old = hm->list;
    unsigned int cap_old = hm->cap; 

    // reset the hashtable's contents 
    hm->cap *= 2;
    hm->size = 0; 
    hm->list = (hm_node **) calloc(hm->cap, sizeof(hm_node *));
    
    // loop through each list index and relocate the items in each linked list
    for (i = 0; i < cap_old; i++) {
        node_curr = list_old[i];
        
        // go through the linked list
        while (node_curr) {
            hash_map_insert(hm, node_curr->key, node_curr->val);
            node_curr = node_curr->next;
        }
    }

    // deallocate old list
    hash_map_free_list(list_old, cap_old);

    return hm->cap;
}

// string hash function found here:
// https://stackoverflow.com/questions/7666509/hash-function-for-string
// TODO: test
unsigned int hash_uuid(uuid_t key)
{
    unsigned long hash = 5381;
    int i;
    
    for (i = 0; i < LEN_UUID_T; i++) {
        hash = ((hash << 5) + hash) + ((unsigned int) key[i]);
    }
    return hash;
}
