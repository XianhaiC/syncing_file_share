#include <stdio.h>
#include <stdlib.h>


// initializes hashtable
ht_file *ht_file_init(int cap, float thresh) {
    int i;
    ht_file *ht = (ht_file *) malloc(sizeof(ht_file));
    ht->cap = cap;
    ht->size = 0;
    ht->thresh = thresh;
    ht->list = (h_node **) calloc(cap, sizeof(h_node *));

    return ht;
}

// free hashtable
void ht_file_free(ht_file *ht) {
    int i;
    h_node *node_curr;
    h_node *node_tmp;

    // free the nodes in the list
    for (i = 0; i < ht->cap; i++) {
        // free linked list
        node_curr = (*htp)->list[i];
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
void ht_file_insert(ht_file *ht, uuid_t key, unsigned int val) {
    unsigned int hash = (*ht->hash_func)(key) % ht->size;
    h_node *node_new;
    h_node *node_head = ht->list[hash];
    h_node *node_tmp = node_head;
    
    // check if node for key already exists
    // if so, replace the old val
    while (node_tmp) {
        // if the key already has an entry replace val
        if ((*ht->key_comp)(node_tmp->key, key)) {
            node_tmp->val = val;
            return;
        }
        node_tmp = node_tmp->next;
    }

    // if the hashtable has surpassed expansion threshold then relocate elements
    // to a larger one
    if (ht->size > (unsigned int) (ht->cap * ht->thresh)) {
        hashtable_expand(&ht); 
    }

    // initialize the new node with contents
    node_new = (h_node *) malloc(sizeof(h_node));
    node_new->key = key;
    node_new->val = val;

    // set it to be the new head
    node_new->next = node_head;
    ht->list[hash] = node_new;
    
    (ht->size)++;
}

void *ht_file_lookup(ht_file *ht, uuid_t key) {
    unsigned int hash = (*ht->hash_func)(key) % ht->size;
    h_node *node_curr = ht->list[hash];

    // search through the linked list for key
    while (node_curr) {
        // check if the key is exists within ht
        if ((*ht->key_comp)(node_tmp->key, key)) {
            node_curr->val = val;
            return;
        }
        node_curr = node_curr->next;
    }

    return -1;
}

int *ht_file_remove(ht_file *ht, uuid_t key) {
    unsigned int hash = (*ht->hash_func)(key) % ht->size;
    h_node *node_curr = ht->list[hash];
    // node pointer pointer that tracks the previous node's next
    h_node **nodep_prev = &(ht->list[hash]);

    // search through the linked list for key
    while (node_curr) {
        if (node_curr->key == key) {
            // link previous node with next
            *nodep_prev = node_curr->next;

            // free the node
            free(node_curr);
            return 0;
        }
        // update node pointer pointer
        nodep_prev = &node_curr->next;
        node_curr = node_curr->next;
    }

    return -1;
}

// creates a new hashtable of double size and relocates elements in the old one
int ht_file_expand(ht_file **htp) {
    int i;

    h_node *node_curr;
    h_node *node_tmp;
    hashtable *ht_new = hashtable_init(
            (*htp)->cap * 2, 
            (*htp)->thresh, 
            (*htp)->data_free);
    
    // loop through each list index and relocate the items in each linked list
    for (i = 0; i < (*htp)->cap; i++) {
        node_curr = (*htp)->list[i];
        
        // go through the linked list
        while (node_curr) {
            hashtable_insert(ht_new, node_curr->key, node_curr->val);
            node_curr = node_curr->next;
        }
    }

    // deallocate old hashtable
    hashtable_free(*htp);

    // update the ht reference to the new hashtable
    *htp = ht_new;
}

// string hash function found here:
// https://stackoverflow.com/questions/7666509/hash-function-for-string
// TODO: test
unsigned int hash_uuid(uuid_t key)
{
    unsigned long n = 5381;
    uuid_t id = (uuid_t) key;
    int i;
    
    for (i = 0; i < LEN_UUDI_; i++) {
        hash = ((hash << 5) + hash) + ((unsigned int) id[i]);
    }

    return hash;
}
