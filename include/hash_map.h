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
    hash_map_n **list;

    unsigned int (*hash)(void *);
    void (*data_free)(void *);
    int (*key_comp)(void *, void *);
};

hash_map *hash_map_init(int cap, float thresh,
    unsigned int (*hash)(void *),
void hash_map_free(hash_map *hm, int free_val);
void hash_map_free_list(hash_map *hm, hash_map_n **list,
    unsigned int cap, int free_val);
int hash_map_insert(hash_map *hm, void *key, unsigned int val);
void *hash_map_lookup(hash_map *hm, void *key);
int hash_map_remove(hash_map *hm, void *key);
int hash_map_expand(hash_map *hm);

unsigned int hash_uuid(void *key);
unsigned int hash_int(void *p_key)

void data_free_sync_info(void *data)
int key_comp_int(void *x, void *y)

#endif
