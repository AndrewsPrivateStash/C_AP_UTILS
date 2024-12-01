/*
    aputils

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

//////////////////// generic vector ////////////////////
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
//////////////////// generic vector ////////////////////


//////////////////// int32 vector ////////////////////
typedef struct {
    int32_t *data;
    size_t size;
    size_t cap;
} Vec_i32;

// make a new generic vector (starting capacity)
Vec_i32 *vec_i32_new(size_t);
// free the vector and its data
void vec_i32_free(Vec_i32*);

// add an element to the back of the vector (Vector, int32 element)
UTIL_ERR vec_i32_add_back(Vec_i32 *v, int32_t elem);
// add an element to the front of the vector (Vector, int32 element)
UTIL_ERR vec_i32_add_front(Vec_i32 *v, int32_t elem);
// return the element at index (errors handled through UTIL_ERR pointer)
int32_t vec_i32_get(Vec_i32 *v, size_t idx, UTIL_ERR *e);
// memset the bytes in range v->size to 0 and set v->size to 0
void vec_i32_clear(Vec_i32*);
// remove element at index, and shift remaning elements up one
UTIL_ERR vec_i32_delete_idx(Vec_i32*, size_t);
// frint the vector to stream using passed print function
void vec_i32_print(Vec_i32 *v, FILE *f, void(*print)(int32_t, FILE*));
//////////////////// int32 vector ////////////////////





// ########################### VEC ###########################






// ########################### Linked Lists ###########################
// ########################### Linked Lists ###########################

// ########################### Has Table ###########################
// ########################### Has Table ###########################




#endif
