#include <stdio.h>
#include <stdlib.h>

// integer hash function found here:
// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash_code(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

// initializes hashtable
hashtable *hashtable_init(int cap, float thresh, void (*data_free)(void *)) {
    int i;
    hashtable *ht = (hashtable *) malloc(sizeof(hashtable));
    ht->cap = cap;
    ht->size = 0;
    ht->thresh = thresh;
    ht->list = (h_node **) calloc(cap, sizeof(h_node *));
    ht->data_free = data_free;

    return ht;
}

// frees hashtable
void hashtable_free(hashtable *ht, int free_content) {
    free(ht->list);
    free(ht);
}
 
// inserts element into hashtable
void hashtable_insert(hashtable *ht, unsigned int key, void *val) {
    int hash = hash_code(key) % ht->size;
    h_node *node_new;
    h_node *node_head = ht->list[hash];
    h_node *node_tmp = node_head;
    
    // check if node for key already exists
    // if so, replace the old val
    while (node_tmp) {
        if (node_tmp->key == key) {
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

void *hashtable_lookup(hashtable *ht, unsigned int key) {
    int hash = hash_code(key) % ht->size;
    h_node *node_curr = ht->list[hash];

    // search through the linked list for key
    while (node_curr) {
        if (node_curr->key == key) {
            node_curr->val = val;
            return;
        }
        node_curr = node_curr->next;
    }

    return NULL;
}

int *hashtable_remove(hashtable *ht, int key) {
    int hash = hash_code(key) % ht->size;
    h_node *node_curr = ht->list[hash];
    // node pointer pointer that tracks the previous node's next
    h_node **nodep_prev = &(ht->list[hash]);

    // search through the linked list for key
    while (node_curr) {
        if (node_curr->key == key) {
            // link previous node with next
            *nodep_prev = node_curr->next;

            // free the data in the node
            (*ht->data_free)(node_curr->val);

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
int hashtable_expand(hashtable **htp) {
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

        // free linked list
        // no need to free node vals since they are to be accessed through the
        // new hashtable
        node_curr = (*htp)->list[i];
        while (node_curr) {
            node_tmp = node_curr;
            node_curr = node_curr->next;
            free(node_tmp);
        }
    }

    // deallocate old hashtable
    hashtable_free(*htp, 0);

    // update the ht reference to the new hashtable
    *htp = ht_new;
}
