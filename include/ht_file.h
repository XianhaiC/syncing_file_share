#ifndef HT_FILE_H
#define HT_FILE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uuid/uuid.h>

#include "macros.h"

// TODO: make list and hash_map's struct vars consitent!

typedef struct _hash_map_n hash_map_n;
typedef struct _hash_map hash_map;

// key: uuit_t
// val: unsigned int
struct _hash_map_n {
    void *key;
    void *val;
    hash_map_n *next;
};

struct _hash_map {
    unsigned int size;
    unsigned int cap;
    float thresh;
    ht_node **list;

    unsigned int (*hash)(void *);
    void (*data_free)(void *);
    int (*data_comp)(void *, void *);
};

ht_file *ht_file_init(int cap, float thresh);
void ht_file_free(ht_file *ht);
void ht_file_free_list(ht_node **list, unsigned int cap);
int ht_file_insert(ht_file *ht, uuid_t key, unsigned int val);
void *ht_file_lookup(ht_file *ht, uuid_t key);
int ht_file_remove(ht_file *ht, uuid_t key);
int ht_file_expand(ht_file *ht);

unsigned int hash_uuid(uuid_t key);

#endif
