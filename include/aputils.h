/*
    (C) aputils

    a collection of general functions and structures

    - vector
    - linked list
    - hash table

*/

#ifndef _APUTILS_H
#define _APUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



enum _UTILERR {
    E_SUCCESS = 0,
    E_EMPTY_VEC = -1,
    E_EMPTY_ARG = -2,
    E_BAD_ALLOC = -3,
    E_MEMCOPY = -4,
    E_OUTOFBOUNDS = -5,
};
typedef enum _UTILERR UTIL_ERR;


// ########################### VEC ###########################

// generic vector
typedef struct {
    void * data;
    size_t size;
    size_t cap;
    size_t elem_size;
} Vector;

// make a new generic vector (element size, starting capacity)
Vector *vector_new(size_t, size_t);
// free the vector and its data
void vector_free(Vector*);

// add an element to the back of the vector (Vector, address of element)
UTIL_ERR vector_add_back(Vector *v, void *elem);
// add an element to the front of the vector (Vector, address of element)
UTIL_ERR vector_add_front(Vector *v, void *elem);
// return the address of the element at index
void *vector_get(Vector*, size_t);
// memset the bytes in range v->size to 0 and set v->size to 0
void vector_clear(Vector*);
// remove element at index, and shift remaning elements up one
UTIL_ERR vector_delete_idx(Vector*, size_t);
// frint the vector to stream using passed print function
void vector_print(Vector *v, FILE *f, void(*print)(void*, FILE*));



// ########################### VEC ###########################



// ########################### Linked Lists ###########################
// ########################### Linked Lists ###########################

// ########################### Has Table ###########################
// ########################### Has Table ###########################




#endif
