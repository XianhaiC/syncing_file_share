#include "ht_file.h"

// TODO: ERROR HANDLING WITH MEMORY

// initializes hashtable
ht_file *ht_file_init(int cap, float thresh) {
    int i;
    ht_file *ht = (ht_file *) malloc(sizeof(ht_file));
    ht->cap = cap;
    ht->size = 0;
    ht->thresh = thresh;
    ht->list = (ht_node **) calloc(cap, sizeof(ht_node *));

    return ht;
}

// free hashtable
void ht_file_free(ht_file *ht) {
    int i;
    ht_node *node_curr;
    ht_node *node_tmp;

    // free the nodes in the list
    for (i = 0; i < ht->cap; i++) {
        // free linked list
        node_curr = ht->list[i];
        while (node_curr) {
            node_tmp = node_curr;
            node_curr = node_curr->next;
            free(node_tmp);
        }
    }

    free(ht->list);
    free(ht);
}
 
// inserts element into hashtable
// returns 0 upon replacement and 1 upon insertion
int ht_file_insert(ht_file *ht, uuid_t key, unsigned int val) {
    unsigned int hash = hash_uuid(key) % ht->cap;
    ht_node *node_new;
    ht_node *node_head = ht->list[hash];
    ht_node *node_tmp = node_head;
    
    // check if node for key already exists
    // if so, replace the old val
    while (node_tmp) {
        // if the key already has an entry replace val
        if (!strncmp(key, node_tmp->key, LEN_UUID_T)) {
            node_tmp->val = val;
            return 0;
        }
        node_tmp = node_tmp->next;
    }

    // if the hashtable has surpassed expansion threshold then relocate elements
    // to a larger one
    if (ht->size > (unsigned int) (ht->cap * ht->thresh)) {
        ht_file_expand(&ht); 
    }

    // initialize the new node with contents
    node_new = (ht_node *) malloc(sizeof(ht_node));
    strncpy(node_new->key, key, LEN_UUID_T);
    node_new->val = val;

    // set it to be the new head
    node_new->next = node_head;
    ht->list[hash] = node_new;
    
    (ht->size)++;

    return 1;
}

// looks up an item in the hashtable
// returns a pointer to the val, null if not found
void *ht_file_lookup(ht_file *ht, uuid_t key) {
    unsigned int hash = hash_uuid(key) % ht->cap;
    ht_node *node_curr = ht->list[hash];

    // search through the linked list for key
    while (node_curr) {
        // check if the key is exists within ht
        if (!strncmp(key, node_curr->key, LEN_UUID_T)) {
            return &(node_curr->val);
        }
        node_curr = node_curr->next;
    }

    return NULL;
}

// removes a key val from the hashtable
// returns 1 upon removal, 0 otherwise
int ht_file_remove(ht_file *ht, uuid_t key) {
    unsigned int hash = hash_uuid(key) % ht->cap;
    ht_node *node_curr = ht->list[hash];
    // node pointer pointer that tracks the previous node's next
    ht_node **nodep_prev = &(ht->list[hash]);

    // search through the linked list for key
    while (node_curr) {
        if (!strncmp(key, node_curr->key, LEN_UUID_T)) {
            // link previous node with next
            *nodep_prev = node_curr->next;

            // free the node
            free(node_curr);
            (ht->size)--;
            return 1;
        }
        // update node pointer pointer
        nodep_prev = &node_curr->next;
        node_curr = node_curr->next;
    }

    return 0;
}

// creates a new hashtable of double size and relocates elements in the old one
// returns the capacity of the new hashtable
int ht_file_expand(ht_file **htp) {
    int i;
    ht_node *node_curr;
    ht_node *node_tmp;
    ht_file *ht_new = ht_file_init(
            (*htp)->cap * 2, 
            (*htp)->thresh);
    
    // loop through each list index and relocate the items in each linked list
    for (i = 0; i < (*htp)->cap; i++) {
        node_curr = (*htp)->list[i];
        
        // go through the linked list
        while (node_curr) {
            ht_file_insert(ht_new, node_curr->key, node_curr->val);
            node_curr = node_curr->next;
        }
    }

    // deallocate old hashtable
    ht_file_free(*htp);

    // update the ht reference to the new hashtable
    *htp = ht_new;

    return (*htp)->cap;
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
