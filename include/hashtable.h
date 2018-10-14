#ifndef HT_FILE_H
#define HT_FILE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _ht_node ht_node;
typedef struct _ht_file ht_file;

// key: uuit_t
// val: unsigned int
struct _ht_node {
    uuid_t key;
    unsigned int val;
    h_node *next;
};

struct _ht_file {
    int cap;
    int size;
    float thresh;
    h_node **list;
}

ht_file *ht_file_init(int cap, float thresh);
void ht_file_free(ht_file *ht);
void ht_file_insert(ht_file *ht, uuid_t key, unsigned int val);
void *ht_file_lookup(ht_file *ht, uuid_t key);
int *ht_file_remove(ht_file *ht, uuid_t key);
int ht_file_expand(ht_file **htp);

unsigned int hash_code(uuid_t key);
