#ifndef LIST_H
#define LIST_H

#include<stdio.h>
#include<stdlib.h>

#include "macros.h"
#include "tracker.h"

// forward declaration for externally defined structs
typedef struct _sync_file_update sync_file_update;

typedef struct _list list;

struct _list {
    int size;
    int capacity;
    void **data; // array of void pointers
    void (*data_free)(void *);
    int (*data_comp)(void *, void *);
};

list *list_init(int cap, void (*data_free)(void *),
        int (*data_comp)(void *, void *));
void list_append(list *lp, void *value);
void *list_get(list *lp, int index);
void list_remove_at(list *lp, int index);
void list_remove(list *lp, void *element);
void list_set(list *lp, int index, void *value);
void list_double_capacity(list *lp);
void list_free(list *lp);

void data_free_string(void *str);
void data_free_sync_file_update(void *sfu);
void data_free_tf_node(void *tf_node);

int data_comp_str(void *str1,  void *str2);
int data_comp_tf_node(void *n1,  void *n2);

#endif
