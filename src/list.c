/*
 *  dynamically expanding generic list to hold void pointers
 */

#include "list.h"

/*
 *  initializes a list with default capacity and the particular data free'ing
 *  function
 *  lp - the pointer to the list struct
 *  data_free - the content specific free function
 */
void list_init(list *lp, void (*data_free)(void *)) {
    lp->size = 0;
    lp->capacity = LIST_INIT_LEN;
    lp->data = malloc(sizeof(void *) * lp->capacity);
    lp->data_free = data_free;
}

/*
 *  appends an element to the list
 *  lp - the pointer to the list struct
 *  element - the element to append
 */
void list_append(list *lp, void *element) {
    // double the list capacity if currenly full before appending value
    if (lp->size == lp->capacity) {
        list_double_capacity(lp);
    }

    // set the value
    list_set(lp, lp->size, element);
    (lp->size)++;
}

/*
 *  retrieves the element at a given index
 *  lp - the pointer to the list struct
 *  index - the index at which to get an element
 */
void *list_get(list *lp, int index) {
    return lp->data[index];
}

/*
 * removes the element at the given index and moves all subsequent elements down
 * to fill the gap
 * lp - the pointer to the list struct
 * index - the index at which to remove an element
 */
void list_remove(list *lp, int index) {
    int i;
    
    // return if the item at index is NULL
    if (lp->data[index] == NULL) {
        return;
    }

    // free the data stored at the pointer and set the pointer to null
    (*lp->data_free)(lp->data[index]);
    lp->data[index] = NULL;

    // move all subsequent items in the list down to fill in the gap
    for (i = index; i < lp->size - 1; i++) {
        list_set(lp, i, list_get(lp, i + 1));
    }

    (lp->size)--;
}

/*
 * sets the location of index with the new element
 * lp - the pointer to the list struct
 * element - the element to be set
 */
void list_set(list *lp, int index, void *element) {
    lp->data[index] = element;
}

/*
 * doubles the capacity of the list by realloc'ing space for new elements
 * lp - the pointer to the list struct
 */
void list_double_capacity(list *lp) {
    lp->capacity *= 2;
    lp->data = realloc(lp->data, lp->capacity);
}

/*
 * frees the list and elements within the list
 * does not free the list struct itself, given that it's been dynamically
 * allocated as well
 * lp - the pointer to the list struct
 */
void list_free(list *lp) {
    int i;
    
    // loop through all elements and call the content specific free function on
    // them
    for (i = 0; i < lp->size; i++) {
        if (lp->data[i] != NULL) {
            (*lp->data_free)(lp->data[i]);
        }
    }

    // free the data pointer
    free(lp->data);
}

/*
 * free string content
 * str - the string to free
 */
void data_free_string(void *str) {
    free((char *) str);
}
