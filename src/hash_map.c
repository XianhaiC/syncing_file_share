#include "hash_map.h"

// TODO: ERROR HANDLING WITH MEMORY
// TODO: generalize so it can take anything as a key and store pointers
// TODO: retest after generalization update

// initializes hashtable
hash_map *hash_map_init(int cap, float thresh,
    void (*data_free)(void *)) {
    int i;
    hash_map *hm = (hash_map *) malloc(sizeof(hash_map));
    hm->cap = cap;
    hm->size = 0;
    hm->thresh = thresh;
    hm->list = (hash_map_n **) calloc(cap, sizeof(hash_map_n *));

    hm->data_free = data_free;

    return hm;
}

// free hashtable
void hash_map_free(hash_map *hm, int free_val) {
    // free the nodes in the list
    hash_map_free_list(hm, hm->list, hm->cap, free_val);

    free(hm);
}

// free_data determines if the data held in nodes should be freed
// hash_map is passed in to have access to provided free function
// for memory management, keep track of whether content data needs to be manually freed or not
void hash_map_free_list(hash_map *hm, hash_map_n **list, unsigned int cap, 
    int free_val) {
    int i;
    hash_map_n *n_tmp;
    hash_map_n *n_curr;

    if (free_val) {
        // free the nodes in the list
        for (i = 0; i < cap; i++) {
            // free linked list
            n_curr = list[i];
            while (n_curr) {
                n_tmp = n_curr;
                n_curr = n_curr->next;

                // free the data in the node as well
                (*hm->data_free)(n_tmp->val);
                free(n_tmp);
            }
        }
    }
    else {
        // free the nodes in the list
        for (i = 0; i < cap; i++) {
            // free linked list
            n_curr = list[i];
            while (n_curr) {
                n_tmp = n_curr;
                n_curr = n_curr->next;

                // only free the done
                free(n_tmp);
            }
        }
    }

    free(list);
}
 
// inserts element into hashtable
// returns 0 upon replacement and 1 upon insertion
int hash_map_insert(hash_map *hm, int key, void *val) {
    int i;
    unsigned int hash;
    hash_map_n *node_new;
 
    // place the node at the head as we don't care about dupes here

    // if the hashtable has surpassed expansion threshold then relocate elements
    // to a larger one
    if (hm->size >= (unsigned int) (hm->cap * hm->thresh)) {
        hash_map_expand(hm); 
 
        // calculate the new hash in the larger hashtable
    }

    hash = hash_int(key) % hm->cap;

    // initialize the new node with contents
    node_new = (hash_map_n *) malloc(sizeof(hash_map_n));

    node_new->key = key;
    node_new->val = val;

    // set it to be the new head
    node_new->next = hm->list[hash];
    hm->list[hash] = node_new;
    
    (hm->size)++;

    return 0;
}

// looks up an item in the hashtable
// returns a pointer to the val, null if not found
void *hash_map_lookup(hash_map *hm, int key) {
    unsigned int hash = hash_int(key) % hm->cap;
    hash_map_n *n_curr = hm->list[hash];

    // search through the linked list for key
    while (n_curr) {
        // check if the key exists within hm
	// int comparison
        if (key == n_curr->key) {
            return &(n_curr->val);
        }
        n_curr = n_curr->next;
    }

    return NULL;
}

// removes a key val from the hashtable
// returns 1 upon removal, 0 otherwise
int hash_map_remove(hash_map *hm, int key) {
    unsigned int hash = hash_int(key) % hm->cap;
    hash_map_n *n_curr = hm->list[hash];
    // node pointer pointer that tracks the previous node's next
    hash_map_n **nodep_prev = &(hm->list[hash]);

    // search through the linked list for key
    while (n_curr) {
        if ((key == n_curr->key)) {
            // link previous node with next
            *nodep_prev = n_curr->next;

            // free the node
            free(n_curr);
            (hm->size)--;
            
            // set to next node
            n_curr = *nodep_prev;
        }
        else {
            // update node pointer pointer
            nodep_prev = &n_curr->next;
            n_curr = n_curr->next;
        }
    }

    return 0;
}

// creates a new hashtable of double size and relocates elements in the old one
// returns the capacity of the new hashtable
int hash_map_expand(hash_map *hm) {
    int i;
    hash_map_n *n_curr;
    hash_map_n **list_old = hm->list;
    unsigned int cap_old = hm->cap; 

    // reset the hashtable's contents 
    hm->cap *= 2;
    hm->size = 0; 
    hm->list = (hash_map_n **) calloc(hm->cap, sizeof(hash_map_n *));
    
    // loop through each list index and relocate the items in each linked list
    for (i = 0; i < cap_old; i++) {
        n_curr = list_old[i];
        
        // go through the linked list
        while (n_curr) {
            hash_map_insert(hm, n_curr->key, n_curr->val);
            n_curr = n_curr->next;
        }
    }

    // deallocate old list, and do not free contents
    hash_map_free_list(hm, list_old, cap_old, 0);

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

// taken from
// https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
unsigned int hash_int(int key) {
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return key;
}

void data_free_sync_info(void *data) {
    sync_info *info = (sync_info *) data;

    free(info);
}
