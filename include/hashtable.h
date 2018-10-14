#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _h_node h_node;
typedef struct _hashtable hashtable;
struct _h_node {
    int key;
    void *val;
    h_node *next;
};

struct _hashtable {
    int cap;
    int size;
    float thresh;
    h_node **list;
    unsigned int (*hash_func)(void *);
    void (*data_free)(void *);
}

hashtable *hashtable_init(int cap, float thresh, void (*data_free)(void *));
void hashtable_free(hashtable *ht, int free_content);
void hashtable_insert(hashtable *ht, unsigned int key, void *val);
void *hashtable_lookup(hashtable *ht, unsigned int key);
int *hashtable_remove(hashtable *ht, int key);
int hashtable_expand(hashtable **htp);

unsigned int hash_code(void *key);
