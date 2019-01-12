#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uuid/uuid.h>

#include "macros.h"
#include "sync_info.h"

// TODO: make list and hash_map's struct vars consitent!

typedef struct _hash_map_n hash_map_n;
typedef struct _hash_map hash_map;

// key: uuit_t
// val: unsigned int
struct _hash_map_n {
    int key;
    void *val;
    hash_map_n *next;
};

struct _hash_map {
    unsigned int size;
    unsigned int cap;
    float thresh;
    hash_map_n **list;

    void (*data_free)(void *);
};

hash_map *hash_map_init(int cap, float thresh,
    void (*data_free)(void *));
void hash_map_free(hash_map *hm, int free_val);
void hash_map_free_list(hash_map *hm, hash_map_n **list,
    unsigned int cap, int free_val);
int hash_map_insert(hash_map *hm, int key, void *val);
void *hash_map_lookup(hash_map *hm, int key);
int hash_map_remove(hash_map *hm, int key);
int hash_map_expand(hash_map *hm);

unsigned int hash_uuid(uuid_t key);
unsigned int hash_int(int key);

void data_free_sync_info(void *data);

#endif
