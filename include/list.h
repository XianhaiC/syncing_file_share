#ifndef LIST_H
#define LIST_H

typedef struct {
    int size;
    int capacity;
    void **data; // array of void pointers
    void data_free(void *);
} list;

void list_init(list *lp) {
    lp->size = 0;
    lp->capacity = LIST_INIT_LEN;
    lp->data = malloc(sizeof(void *) * list->capacity);
}

void list_append(list *lp, void *value) {
    if (lp->size == lp->capacity) {
        list_double_capacity(lp);
    }
    lp->data[lp->size] = value;
    lp->size++;
}

void *list_get(list *lp, int index) {
    return lp->data[index];
}

void list_set(list *lp, int index, void *value) {
    lp->data[index] = value;
}

void list_double_capacity(list *lp) {
    lp->capacity *= 2;
    lp->data = realloc(lp->data, lp->capacity);
}

// TODO:
// make this genericl enough to handle freeing the data contents
// perhaps add a function pointer into the struct to do so
void list_free(list *lp) {
    int i;
    
    for (i = 0; i < lp->size; i++) {
        if (lp->data[i] != NULL) {
            lp->data_free(lp->data[i]);
        }
    }

    free(lp->data);
}
