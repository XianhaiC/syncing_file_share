#ifndef HT_FILE_H
#define HT_FILE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <uuid/uuid.h>

#include "macros.h"

typedef struct _ht_node ht_node;
typedef struct _ht_file ht_file;

// key: uuit_t
// val: unsigned int
struct _ht_node {
    uuid_t key;
    unsigned int val;
    ht_node *next;
};

struct _ht_file {
    unsigned int cap;
    unsigned int size;
    float thresh;
    ht_node **list;
};

ht_file *ht_file_init(int cap, float thresh);
void ht_file_free(ht_file *ht);
int ht_file_insert(ht_file *ht, uuid_t key, unsigned int val);
void *ht_file_lookup(ht_file *ht, uuid_t key);
int ht_file_remove(ht_file *ht, uuid_t key);
int ht_file_expand(ht_file **htp);

unsigned int hash_uuid(uuid_t key);

#endif
