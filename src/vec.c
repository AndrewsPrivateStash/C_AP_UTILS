/*  vectors
    generic vector
        > processes data as bytes using void pointers
        > assumes al elements are of the same type

*/

#include "../include/aputils.h"


static void vector_fatal(const char* err) {
    fprintf(stderr, "%s\n", err);
    exit(1);
}



// ###################### GENERIC VECTOR ######################

Vector *vector_new(size_t elem_size, size_t cap) {
    if (elem_size < 1 || cap < 1) {
        return (Vector*)0;  // caller checks NULL
    }

    Vector *new_vec = malloc(sizeof(*new_vec));
    if (!new_vec) {
        return (Vector*)0;
    }

    new_vec->data = malloc(elem_size * cap);
    if (!new_vec->data) {
        free(new_vec);
        return (Vector*)0;
    }

    new_vec->cap = cap;
    new_vec->size = 0;
    new_vec->elem_size = elem_size;

    return new_vec;

}


void vector_free(Vector *v) {
    free(v->data);
    free(v);
}


static void vector_resize(Vector *v) {
    if (!v) return;
    if (v->size < v->cap) return;
    
    v->cap *= 2;
    v->data = realloc(v->data, v->cap * v->elem_size);
    if (!v->data) {
        vector_fatal("failed to realloc vector");
    }
}


UTIL_ERR vector_add_back(Vector *v, void *elem) {
    if (!v) return E_EMPTY_VEC;
    if (!elem) return E_EMPTY_ARG;
    if (v->size == v->cap) vector_resize(v);

    if (!memcpy((char*)v->data + v->size * v->elem_size, elem, v->elem_size)) {
        return E_MEMCOPY;
    }
    v->size++;
    
    return E_SUCCESS;
}


UTIL_ERR vector_add_front(Vector *v, void *elem) {
    if (!v) return E_EMPTY_VEC;
    if (!elem) return E_EMPTY_ARG;
    if (v->size == v->cap) vector_resize(v);

    // move all elements down one
    for (size_t i = v->size; i > 0; i--) {
        if (!memcpy((char*)v->data + i * v->elem_size,
                    (char*)v->data + (i-1) * v->elem_size,
                    v->elem_size)) {
            return E_MEMCOPY;
        }
    }

    // place new element at front
    if (!memcpy((char*)v->data, elem, v->elem_size)) {
        return E_MEMCOPY;
    }
    v->size++;
    
    return E_SUCCESS;

}


void *vector_get(Vector *v, size_t idx) {
    if (!v) return NULL;
    if (idx > v->size -1) return NULL;

    return (void*)((char*)v->data + idx * v->elem_size);
}


void vector_clear(Vector *v) {
    if (!v) return;
    if (v->size == 0) return;

    memset(v->data, 0, v->size * v->elem_size);
    v->size = 0;
}


UTIL_ERR vector_delete_idx(Vector *v, size_t idx) {
    if (!v) return E_EMPTY_VEC;
    if (idx > v->size -1) return E_OUTOFBOUNDS;

    // move data below idx up one
    for (size_t i = idx; i < v->size-1; i++) {
        if (!memcpy((char*)v->data + i * v->elem_size,
                    (char*)v->data + (i+1) * v->elem_size,
                    v->elem_size)) {
            return E_MEMCOPY;
        }        
    }

    // clear data from bottom moved element
    memset((char*)v->data + (v->size-1) * v->elem_size, 0, v->elem_size);
    v->size--;

    return E_SUCCESS;

}


void vector_print(Vector *v, FILE *f, void(*print)(void*, FILE*)) {
    if (!v || !f) return;
    if (v->size == 0) return;
    
    for (size_t i = 0; i< v->size; i++) {
        print( (char*)v->data + i * v->elem_size, f );
    }
}


// ###################### GENERIC VECTOR ######################
