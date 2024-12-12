/*
 *    aputils
 *    a collection of general functions and structures
 *
 */

#ifndef _APUTILS_H
#define _APUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>



// if added to, update common.c
enum _UTILERR {
    E_SUCCESS = 0,
    E_EMPTY_OBJ = -1,
    E_EMPTY_ARG = -2,
    E_BAD_ALLOC = -3,
    E_MEMCOPY = -4,
    E_OUTOFBOUNDS = -5,
    E_EMPTY_FUNC = -6,
    E_NOOP = -7,                // no operation to perform
    E_NODATA = -8,
    E_DOESNT_EXIST = -9,        // element doesn't exist
    E_BAD_TYPE = -10,
};
typedef enum _UTILERR UTIL_ERR;
const char *UTIL_ERR_PRINT(UTIL_ERR);



// ########################### VECTORS ###########################

enum vec_type {
    vector = 0,
    vec_i32 = 1,
    vec_char = 2,
};
typedef enum vec_type VECTYPE;

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
// return a shallow copy of the vector
// if data contains pointers, only the pointers are copied
Vector *vector_copy(const Vector *v);

// add an element to the back of the vector (Vector, address of element)
UTIL_ERR vector_add_back(Vector *v, void *elem);
// add an element to the front of the vector (Vector, address of element)
UTIL_ERR vector_add_front(Vector *v, void *elem);
// insert an element at the provided index (shifts others down)
UTIL_ERR vector_insert(Vector *v, void *elem, size_t idx);
// return the address of the element at index
void *vector_get(const Vector*, size_t, UTIL_ERR *e);
// memset the bytes in range v->size to 0 and set v->size to 0
UTIL_ERR vector_clear(Vector*);
// remove element at index, and shift remaning elements up one
UTIL_ERR vector_delete_idx(Vector*, size_t);
// print the vector to stream using passed print function
UTIL_ERR vector_print(const Vector *v, FILE *f, void(*print)(void*, FILE*));

// map the supplied function pointer over the vector elements (in place)
UTIL_ERR vector_map(Vector *v, void(*mapfunc)(void*));
// map the supplied function pointer over the vector elements (return shallow copy)
// if data contains pointers, only the pointers are copied
Vector *vector_map_new(const Vector *v, void(*mapfunc)(void*), UTIL_ERR *e);
// return new vector with elements filtered based on passed function pointer
Vector *vector_filter(const Vector *v, bool(*filter)(void*), UTIL_ERR *e);
// check for an element in the vector and return idx if found, otherwise -1
intmax_t vector_in(const Vector *v, void *elem, bool(*equal)(void*, void*), UTIL_ERR *e);
// swap the two index data
UTIL_ERR vector_swap(Vector *v, size_t idx1, size_t idx2);
// reverse the vector
UTIL_ERR vector_reverse(Vector *v);

//////////////////// generic vector ////////////////////


//////////////////// int32 vector ////////////////////
typedef struct {
    int32_t *data;
    size_t size;
    size_t cap;
} Vec_i32;

// make a new i32 vector (starting capacity)
Vec_i32 *vec_i32_new(size_t);
// free the vector and its data
void vec_i32_free(Vec_i32*);
// return a copy of the vector
Vec_i32 *vec_i32_copy(const Vec_i32 *v);

// add an element to the back of the vector (Vector, int32 element)
UTIL_ERR vec_i32_add_back(Vec_i32 *v, int32_t elem);
// add an element to the front of the vector (Vector, int32 element)
UTIL_ERR vec_i32_add_front(Vec_i32 *v, int32_t elem);
// insert an element at the provided index (shifts others down)
UTIL_ERR vec_i32_insert(Vec_i32 *v, int32_t elem, size_t idx);
// return the element at index (errors handled through UTIL_ERR pointer)
int32_t vec_i32_get(const Vec_i32 *v, size_t idx, UTIL_ERR *e);
// memset the bytes in range v->size to 0 and set v->size to 0
void vec_i32_clear(Vec_i32*);
// remove element at index, and shift remaning elements up one
UTIL_ERR vec_i32_delete_idx(Vec_i32*, size_t);
// print the vector to stream using passed print function
UTIL_ERR vec_i32_print(const Vec_i32 *v, FILE *f, void(*print)(int32_t, FILE*));

// map the supplied function pointer over the vector elements (in place)
UTIL_ERR vec_i32_map(Vec_i32 *v, void(*mapfunc)(int32_t*));
// map the supplied function pointer over the vector elements (return copy)
Vec_i32 *vec_i32_map_new(const Vec_i32 *v, void(*mapfunc)(int32_t*), UTIL_ERR *e);
// return new vector with elements filtered based on passed function pointer
Vec_i32 *vec_i32_filter(const Vec_i32 *v, bool(*filter)(int32_t), UTIL_ERR *e);
// check for an element in the vector and return idx if found, otherwise -1
intmax_t vec_i32_in(const Vec_i32 *v, int32_t elem, bool(*equal)(int32_t, int32_t), UTIL_ERR *e);
// swap the two index data
UTIL_ERR vec_i32_swap(Vec_i32 *v, size_t idx1, size_t idx2);
// reverse the vector
UTIL_ERR vec_i32_reverse(Vec_i32 *v);

//////////////////// int32 vector ////////////////////


//////////////////// char vector ////////////////////
typedef struct {
    char *data;
    size_t size;
    size_t cap;
} Vec_char;

// make a new generic vector (starting capacity)
Vec_char *vec_char_new(size_t cap);
// free the vector and its data
void vec_char_free(Vec_char*);
// return a copy of the vector
Vec_char *vec_char_copy(const Vec_char *v);

// add an element to the back of the vector (Vector, int32 element)
UTIL_ERR vec_char_add_back(Vec_char *v, char elem);
// add an element to the front of the vector (Vector, int32 element)
UTIL_ERR vec_char_add_front(Vec_char *v, char elem);
// insert an element at the provided index (shifts others down)
UTIL_ERR vec_char_insert(Vec_char *v, char elem, size_t idx);
// return the element at index (errors handled through UTIL_ERR pointer)
char vec_char_get(const Vec_char *v, size_t idx, UTIL_ERR *e);
// memset the bytes in range v->size to 0 and set v->size to 0
void vec_char_clear(Vec_char*);
// remove element at index, and shift remaning elements up one
UTIL_ERR vec_char_delete_idx(Vec_char*, size_t idx);
// print the vector to stream using passed print function
UTIL_ERR vec_char_print(const Vec_char *v, FILE *f, void(*print)(char, FILE*));

// map the supplied function pointer over the vector elements (in place)
UTIL_ERR vec_char_map(Vec_char *v, void(*mapfunc)(char*));
// map the supplied function pointer over the vector elements (return copy)
Vec_char *vec_char_map_new(const Vec_char *v, void(*mapfunc)(char*), UTIL_ERR *e);
// return new vector with elements filtered based on passed function pointer
Vec_char *vec_char_filter(const Vec_char *v, bool(*filter)(char), UTIL_ERR *e);
// check for an element in the vector and return idx if found, otherwise -1
intmax_t vec_char_in(const Vec_char *v, char elem, bool(*equal)(char, char), UTIL_ERR *e);
// swap the two index data
UTIL_ERR vec_char_swap(Vec_char *v, size_t idx1, size_t idx2);
// reverse the vector
UTIL_ERR vec_char_reverse(Vec_char *v);

//////////////////// char vector ////////////////////
UTIL_ERR vector_sort(void *vec, VECTYPE type, int (*compare)(const void*, const void*));

// ########################### VECTORS ###########################


// ########################### Linked Lists ###########################
typedef struct aputil_node {
    void *data;
    struct aputil_node *next;
    struct aputil_node *prev;
} APUTIL_Node;

typedef struct {
    APUTIL_Node *head;
    APUTIL_Node *tail;
    size_t cnt;
    void (*free)(void*);                        // data free function
    void *(*copydata)(const void*);             // copy data fucntion
    int (*compare)(const void*, const void*);   // compare function
    char desc[128];
} APUTIL_LList;

// make new list with optional data free and copy functions
APUTIL_LList *aputil_llist_new(void (*free)(void*), void *(*copydata)(const void*), int (*compare)(const void*, const void*), const char *desc, UTIL_ERR*);
// free the list and optionally free data
void aputil_llist_free(APUTIL_LList*, bool preserve);
// print node using provided data element function
void aputil_llist_print_node(const APUTIL_Node *node, FILE *f, void (*data_print)(FILE*, void*));
// print container and entire list using provided data element function
void aputil_llist_print_all(const APUTIL_LList *lst, FILE *f, void (*data_print)(FILE*, void*));

// add node to front (new head)
UTIL_ERR aputil_llist_push(APUTIL_LList*, void*);
// return data from head node and remove from list
void *aputil_llist_pop(APUTIL_LList*, UTIL_ERR*);
// add node to back (new tail)
UTIL_ERR aputil_llist_push_back(APUTIL_LList*, void*);
// return data from tail node and remove from list
void *aputil_llist_pop_back(APUTIL_LList*, UTIL_ERR*);
// delete provded node
UTIL_ERR aputil_llist_delete(APUTIL_LList*, APUTIL_Node*, bool preserve);
// shallow copy node and return new node allocation
APUTIL_Node *aputil_llist_copy_node(const APUTIL_LList *, const APUTIL_Node*, bool deep, UTIL_ERR*);
// copy list and return new allocation
APUTIL_LList *aputil_llist_copy(const APUTIL_LList*, bool deep, UTIL_ERR*);
// clear the list of nodes and free data memory if free-func set
UTIL_ERR aputil_llist_clear(APUTIL_LList*, bool preserve);

// does node address exist in list
bool aputil_llist_node_exists(const APUTIL_LList*, const APUTIL_Node*);
// does data element exist in list, returns the node if so
APUTIL_Node *aputil_llist_in(const APUTIL_LList*, const void *elem, bool(*equalfunc)(const void*, const void*), UTIL_ERR *e);
// map the supplied function over the list elements (in place)
UTIL_ERR aputil_llist_map(APUTIL_LList *lst, void(*mapfunc)(void*));
// map the supplied function over the list elements (return deep copy)
APUTIL_LList *aputil_llist_map_new(const APUTIL_LList *lst, void(*mapfunc)(void*), UTIL_ERR *e);
// return new list with elements filtered based on passed function (option to copy data)
APUTIL_LList *aputil_llist_filter(const APUTIL_LList *lst, bool(*filterfunc)(void*), bool copy, UTIL_ERR *e);
// swap two nodes in list (does not preserve node address-data association)
UTIL_ERR aputil_llist_nodeswap(APUTIL_Node *n1, APUTIL_Node *n2);
// is the list sorted
bool aputil_llist_is_sorted(APUTIL_LList *lst, UTIL_ERR *e);
// reverse list in place
UTIL_ERR aputil_llist_reverse(APUTIL_LList *lst);

// sort the list in-place using iterative merge sort with runs
void merge_sort(APUTIL_LList *lst);

// ########################### Linked Lists ###########################

// ########################### Hash Table ###########################
// ########################### Hash Table ###########################



#endif
