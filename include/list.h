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

void list_init(list *lp, void (*data_free)(void *));
void list_append(list *lp, void *value);
void *list_get(list *lp, int index);
void list_remove(list *lp, int index);
void list_set(list *lp, int index, void *value);
void list_double_capacity(list *lp);
void list_free(list *lp);

void data_free_string(void *str);
void data_free_sync_file_update(void *sfu);

#endif
