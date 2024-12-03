/*
 *    test the various vector functions
 */

#include <unity/unity.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/aputils.h"


void setUp(void) {
    /* This is run before EACH TEST */
}

void tearDown(void) {}



//################ Generic Vector ################
void test_function_vector_new(void) {

    // initial capacity and size check
    Vector *tstvec = vector_new(sizeof(size_t), 100);
    TEST_ASSERT_NOT_NULL(tstvec);
    TEST_ASSERT_NOT_NULL(tstvec->data);
    TEST_ASSERT_EQUAL_INT32(100, tstvec->cap);
    TEST_ASSERT_EQUAL_INT32(0, tstvec->size);
    TEST_ASSERT_EQUAL_INT32(sizeof(size_t), tstvec->elem_size);

    vector_free(tstvec);

}


void test_function_vector_clear(void) {

    int tmp[] = {5, 1000};

    // initial capacity and size check
    Vector *tstvec = vector_new(sizeof(int), 100);
    vector_add_back(tstvec, &tmp[0]);
    vector_add_back(tstvec, &tmp[1]);

    vector_clear(tstvec);
    TEST_ASSERT_EQUAL_INT32(0, tstvec->size);
    for (size_t i = 0; i<tstvec->elem_size *2; i++) {
        fprintf(stdout, "idx:%lu, val:%d\n", i, *((char*)tstvec->data + i));
        TEST_ASSERT_EQUAL_INT32(0, *((char*)tstvec->data + i));
    }
    
    vector_free(tstvec);

}


void test_function_vector_copy(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vector *tstvec = vector_new(sizeof(int), 1);
    for (int i = 0; i<10; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }
    Vector *new_vec = vector_copy(tstvec);
    TEST_ASSERT_EQUAL_INT32(tstvec->cap, new_vec->cap);
    TEST_ASSERT_EQUAL_INT32(tstvec->size, new_vec->size);
    TEST_ASSERT_EQUAL_INT32(tstvec->elem_size, new_vec->elem_size);

    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_INT32(
            *(int*)vector_get(tstvec, i, NULL),
            *(int*)vector_get(new_vec, i, NULL)
        );
    }
    
    vector_free(tstvec);
    vector_free(new_vec);

}


void test_function_vector_add_back(void) {

    int tmp[] = {5, 1000};

    // initial capacity and size check
    Vector *tstvec = vector_new(sizeof(int), 1);
    UTIL_ERR e = vector_add_back(tstvec, &tmp[0]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);

    e = vector_add_back(tstvec, &tmp[1]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(2, tstvec->size);
    TEST_ASSERT_EQUAL_INT32(tmp[0], *((int*)tstvec->data) );
    TEST_ASSERT_EQUAL_INT32(tmp[1], *((int*)(
        (char*)tstvec->data + 1 * tstvec->elem_size
    )) );

    vector_free(tstvec);

}


void test_function_vector_add_front(void) {

    int tmp[] = {5, 1000};

    Vector *tstvec = vector_new(sizeof(int), 1);
    UTIL_ERR e = vector_add_front(tstvec, &tmp[0]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);

    e = vector_add_front(tstvec, &tmp[1]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(2, tstvec->size);
    TEST_ASSERT_EQUAL_INT32(tmp[1], *((int*)tstvec->data) );
    TEST_ASSERT_EQUAL_INT32(tmp[0], *((int*)(
        (char*)tstvec->data + 1 * tstvec->elem_size
    )) );

    vector_free(tstvec);

}


void test_function_vector_insert(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vector *tstvec = vector_new(sizeof(int), 1);
    for (int i = 0; i<10; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    int ins_val = -100;
    UTIL_ERR e = vector_insert(tstvec, &ins_val, 5);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(ins_val, *((int*)(
        (char*)tstvec->data + 5 * tstvec->elem_size
    )) );
    TEST_ASSERT_EQUAL_INT32(tmp[9], *((int*)(
        (char*)tstvec->data + 10 * tstvec->elem_size
    )) );
    
    vector_free(tstvec);

}


void test_function_vector_get(void) {

    int tmp[] = {5, 1000};
    UTIL_ERR e = E_SUCCESS;

    // initial capacity and size check
    Vector *tstvec = vector_new(sizeof(int), 1);
    vector_add_back(tstvec, &tmp[0]);
    vector_add_back(tstvec, &tmp[1]);

    TEST_ASSERT_EQUAL_INT32(tmp[0], *(int*)vector_get(tstvec, 0, &e));
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(tmp[1], *(int*)vector_get(tstvec, 1, &e));
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_NULL((int*)vector_get(tstvec, 100, &e));
    TEST_ASSERT_TRUE(e = E_OUTOFBOUNDS);

    vector_free(tstvec);

}


void test_function_vector_delete_idx(void) {

    int tmp[] = {5, 1000, 20, 0, 42, 777};

    Vector *tstvec = vector_new(sizeof(int), 1);
    for (int i = 0; i<6; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    UTIL_ERR e = vector_delete_idx(tstvec, 1);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    for (int i = 2; i<6; i++) {
        TEST_ASSERT_EQUAL_INT32(tmp[i], *(int*)vector_get(tstvec, i-1, &e));
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }

    TEST_ASSERT_EQUAL_INT32(5, tstvec->size);


    vector_free(tstvec);

}


static void print_ints(void *e, FILE *f) {
    fprintf(f, "%d ", *(int*)e);
}

void test_function_vector_print(void) {

    int tmp[] = {5, 1000, 20, 0, 42, 777};

    Vector *tstvec = vector_new(sizeof(int), 1);
    for (int i = 0; i<6; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    vector_print(tstvec, stdout, print_ints);
    printf("\n");

    TEST_PASS();


    vector_free(tstvec);

}


static void vec_int_map(void *elem) {
    *(int*)elem *= 2;
}

void test_function_vector_map(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vector *tstvec = vector_new(sizeof(int), 1);
    for (int i = 0; i<10; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    // map a doubling function over the vector
    UTIL_ERR e = vector_map(tstvec, vec_int_map);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_INT32(tmp[i] * 2, *(int*)vector_get(tstvec, i, &e));
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    
    vector_free(tstvec);

}


void test_function_vector_map_new(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vector *tstvec = vector_new(sizeof(int), 1);
    for (int i = 0; i<10; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    UTIL_ERR e = E_SUCCESS;
    Vector *new_vec = vector_map_new(tstvec, vec_int_map, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    
    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_INT32(tmp[i] * 2, *(int*)vector_get(new_vec, i, &e));
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    
    vector_free(tstvec);
    vector_free(new_vec);

}

static bool vector_filter_func(void *elem) {
    return *(int*)elem % 2 == 0;
}

void test_function_vector_filter(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vector *tstvec = vector_new(sizeof(int), 1);
    for (int i = 0; i<10; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    UTIL_ERR e = E_SUCCESS;
    Vector *new_vec = vector_filter(tstvec, vector_filter_func, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    
    vector_print(new_vec, stdout, print_ints); printf("\n");
    for (size_t i = 0; i<new_vec->size; i++) {
        TEST_ASSERT_TRUE(*(int*)vector_get(new_vec, i, &e) % 2 == 0);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    
    vector_free(tstvec);
    vector_free(new_vec);

}

struct vector_in_tst {
    int a;
    char buff[64];
    void *c;
};

static bool vector_in_func(void *e1, void *e2) {
    if (
        ((struct vector_in_tst*)e1)->a == ((struct vector_in_tst*)e2)->a &&
        strcmp(((struct vector_in_tst*)e1)->buff, ((struct vector_in_tst*)e2)->buff) == 0 &&
        ((struct vector_in_tst*)e1)->c == ((struct vector_in_tst*)e2)->c
    ) return true;
    else return false;
}

void test_function_vector_in(void) {
    struct vector_in_tst tst_arr[] = {
        {.a = 0, .c = (void*)0x55555555f2a0, .buff = "some text"},
        {.a = 1, .c = (void*)0x5555555552d0, .buff = "more text"},
        {.a = 2, .c = (void*)0x7fffffffddec, .buff = "even more text"},
        {.a = 3, .c = (void*)0x5555555570d7, .buff = "yep text"},
    };
    struct vector_in_tst find_elem = {2, "even more text", (void*)0x7fffffffddec};
    struct vector_in_tst not_elem = {8, "bad omen", (void*)0x7fffff12ddec};

    Vector *tstvec = vector_new(sizeof(struct vector_in_tst), 1);
    for (int i = 0; i<4; i++) {
        vector_add_back(tstvec, &tst_arr[i]);
    }

    UTIL_ERR e = E_SUCCESS;
    intmax_t is_fnd = vector_in(tstvec, &find_elem, vector_in_func, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_TRUE(is_fnd != -1);
    TEST_ASSERT_EQUAL_INT32(2, is_fnd);

    is_fnd = vector_in(tstvec, &not_elem, vector_in_func, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(-1, is_fnd);

    vector_free(tstvec);

}

//################ Generic Vector ################
//################ i32 Vector ################

void test_function_vec_i32_new(void) {

    // initial capacity and size check
    Vec_i32 *tstvec = vec_i32_new(100);
    TEST_ASSERT_NOT_NULL(tstvec);
    TEST_ASSERT_NOT_NULL(tstvec->data);
    TEST_ASSERT_EQUAL_INT32(100, tstvec->cap);
    TEST_ASSERT_EQUAL_INT32(0, tstvec->size);

    vec_i32_free(tstvec);

}


void test_function_vec_i32_clear(void) {

    int tmp[] = {5, 1000};

    Vec_i32 *tstvec = vec_i32_new(1);
    vec_i32_add_back(tstvec, tmp[0]);
    vec_i32_add_back(tstvec, tmp[1]);

    vec_i32_clear(tstvec);
    TEST_ASSERT_EQUAL_INT32(0, tstvec->size);
    for (size_t i = 0; i<2; i++) {
        fprintf(stdout, "idx:%lu, val:%d\n", i, tstvec->data[i]);
        TEST_ASSERT_EQUAL_INT32(0, tstvec->data[i]);
    }
    
    vec_i32_free(tstvec);

}


void test_function_vec_i32_copy(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};
    UTIL_ERR e = E_SUCCESS;

    Vec_i32 *tstvec = vec_i32_new(1);
    for (int i = 0; i<10; i++) {
        vec_i32_add_back(tstvec, tmp[i]);
    }
    Vec_i32 *new_vec = vec_i32_copy(tstvec);
    TEST_ASSERT_EQUAL_INT32(tstvec->cap, new_vec->cap);
    TEST_ASSERT_EQUAL_INT32(tstvec->size, new_vec->size);

    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_INT32(
            vec_i32_get(tstvec, i, &e),
            vec_i32_get(new_vec, i, &e)
        );
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    
    vec_i32_free(tstvec);
    vec_i32_free(new_vec);

}


void test_function_vec_i32_add_back(void) {

    int tmp[] = {5, 1000};

    Vec_i32 *tstvec = vec_i32_new(1);
    UTIL_ERR e = vec_i32_add_back(tstvec, tmp[0]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);

    e = vec_i32_add_back(tstvec, tmp[1]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(2, tstvec->size);
    TEST_ASSERT_EQUAL_INT32(tmp[0], tstvec->data[0] );
    TEST_ASSERT_EQUAL_INT32(tmp[1], tstvec->data[1] );

    vec_i32_free(tstvec);

}


void test_function_vec_i32_add_front(void) {

    int tmp[] = {5, 1000};

    // initial capacity and size check
    Vec_i32 *tstvec = vec_i32_new(1);
    UTIL_ERR e = vec_i32_add_front(tstvec, tmp[0]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);

    e = vec_i32_add_front(tstvec, tmp[1]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(2, tstvec->size);
    TEST_ASSERT_EQUAL_INT32(tmp[1], tstvec->data[0] );
    TEST_ASSERT_EQUAL_INT32(tmp[0], tstvec->data[1] );

    vec_i32_free(tstvec);

}


void test_function_vec_i32_insert(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vec_i32 *tstvec = vec_i32_new(1);
    for (int i = 0; i<10; i++) {
        vec_i32_add_back(tstvec, tmp[i]);
    }

    int ins_val = -100;
    UTIL_ERR e = vec_i32_insert(tstvec, ins_val, 5);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(ins_val, tstvec->data[5]);
    TEST_ASSERT_EQUAL_INT32(tmp[9], tstvec->data[10]);
    
    vec_i32_free(tstvec);

}


void test_function_vec_i32_get(void) {

    int tmp[] = {5, 1000};
    UTIL_ERR e = E_SUCCESS;

    // initial capacity and size check
    Vec_i32 *tstvec = vec_i32_new(1);
    vec_i32_add_back(tstvec, tmp[0]);
    vec_i32_add_back(tstvec, tmp[1]);

    TEST_ASSERT_EQUAL_INT32(tmp[0], vec_i32_get(tstvec, 0, &e));
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(tmp[1], vec_i32_get(tstvec, 1, &e));
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(0, vec_i32_get(tstvec, 100, &e));
    TEST_ASSERT_TRUE(e == E_OUTOFBOUNDS);

    vec_i32_free(tstvec);

}


void test_function_vec_i32_delete_idx(void) {

    int tmp[] = {5, 1000, 20, 0, 42, 777};

    Vec_i32 *tstvec = vec_i32_new(1);
    for (int i = 0; i<6; i++) {
        vec_i32_add_back(tstvec, tmp[i]);
    }

    UTIL_ERR e = vec_i32_delete_idx(tstvec, 1);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(0, tstvec->data[tstvec->size]);
    for (int i = 2; i<6; i++) {
        int32_t v = vec_i32_get(tstvec, i-1, &e);
        printf("%d ", v);
        TEST_ASSERT_EQUAL_INT32(tmp[i], v);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_INT32(5, tstvec->size);

    vec_i32_free(tstvec);

}


static void print_ints_i32(int32_t e, FILE *f) {
    fprintf(f, "%d ", e);
}

void test_function_vec_i32_print(void) {

    int tmp[] = {5, 1000, 20, 0, 42, 777};

    Vec_i32 *tstvec = vec_i32_new(1);
    for (int i = 0; i<6; i++) {
        vec_i32_add_back(tstvec, tmp[i]);
    }

    vec_i32_print(tstvec, stdout, print_ints_i32);
    printf("\n");

    TEST_PASS();
    vec_i32_free(tstvec);

}


static void vec_i32_mapper(int32_t *elem) {
    *elem *= 2;
}

void test_function_vec_i32_map(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vec_i32 *tstvec = vec_i32_new(1);
    for (int i = 0; i<10; i++) {
        vec_i32_add_back(tstvec, tmp[i]);
    }

    // map a doubling function over the vector
    UTIL_ERR e = vec_i32_map(tstvec, vec_i32_mapper);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_INT32(tmp[i] * 2, vec_i32_get(tstvec, i, &e));
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    
    vec_i32_free(tstvec);

}


void test_function_vec_i32_map_new(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vec_i32 *tstvec = vec_i32_new(1);
    for (int i = 0; i<10; i++) {
        vec_i32_add_back(tstvec, tmp[i]);
    }

    UTIL_ERR e = E_SUCCESS;
    Vec_i32 *new_vec = vec_i32_map_new(tstvec, vec_i32_mapper, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    
    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_INT32(tmp[i] * 2, vec_i32_get(new_vec, i, &e));
    }
    
    vec_i32_free(tstvec);
    vec_i32_free(new_vec);

}


static bool vec_i32_filter_func(int32_t elem) {
    return elem % 2 == 0;
}

void test_function_vec_i32_filter(void) {

    int tmp[] = {1,2,3,4,5,6,7,8,9,10};

    Vec_i32 *tstvec = vec_i32_new(1);
    for (int i = 0; i<10; i++) {
        vec_i32_add_back(tstvec, tmp[i]);
    }

    UTIL_ERR e = E_SUCCESS;
    Vec_i32 *new_vec = vec_i32_filter(tstvec, vec_i32_filter_func, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    
    vec_i32_print(new_vec, stdout, print_ints_i32); printf("\n");
    for (size_t i = 0; i<new_vec->size; i++) {
        TEST_ASSERT_TRUE(vec_i32_get(new_vec, i, &e) % 2 == 0);
    }
    
    vec_i32_free(tstvec);
    vec_i32_free(new_vec);

}

static bool vec_i32_in_func(int32_t e1, int32_t e2) {
    return e1 % 2 == e2;
}

void test_function_vec_i32_in(void) {

    int32_t tmp[] = {1,2,3,4,5,6,7,8,9,10};
    int32_t fnd = 5;
    int32_t nfnd = 0;

    Vec_i32 *tstvec = vec_i32_new(1);
    for (int i = 0; i<10; i++) {
        vec_i32_add_back(tstvec, tmp[i]);
    }    

    UTIL_ERR e = E_SUCCESS;
    intmax_t is_fnd = vec_i32_in(tstvec, fnd, NULL, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_TRUE(is_fnd != -1);
    TEST_ASSERT_EQUAL_INT32(4, is_fnd);

    is_fnd = vec_i32_in(tstvec, fnd, vec_i32_in_func, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_TRUE(is_fnd != -1);
    TEST_ASSERT_EQUAL_INT32(0, is_fnd);

    is_fnd = vec_i32_in(tstvec, nfnd, NULL, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(-1, is_fnd);

    vec_i32_free(tstvec);

}

//################ i32 Vector ################
//################ char Vector ################

void test_function_vec_char_new(void) {

    // initial capacity and size check
    Vec_char *tstvec = vec_char_new(100);
    TEST_ASSERT_NOT_NULL(tstvec);
    TEST_ASSERT_NOT_NULL(tstvec->data);
    TEST_ASSERT_EQUAL_INT32(100, tstvec->cap);
    TEST_ASSERT_EQUAL_INT32(0, tstvec->size);

    vec_char_free(tstvec);

}


void test_function_vec_char_clear(void) {

    char tmp[] = {'a', 'b'};

    Vec_char *tstvec = vec_char_new(1);
    vec_char_add_back(tstvec, tmp[0]);
    vec_char_add_back(tstvec, tmp[1]);

    vec_char_clear(tstvec);
    TEST_ASSERT_EQUAL_INT32(0, tstvec->size);
    for (size_t i = 0; i<2; i++) {
        TEST_ASSERT_EQUAL_CHAR('\0', tstvec->data[i]);
    }
    
    vec_char_free(tstvec);

}


void test_function_vec_char_copy(void) {

    char tmp[] = "abcdefghij";
    UTIL_ERR e = E_SUCCESS;

    Vec_char *tstvec = vec_char_new(1);
    for (int i = 0; i<10; i++) {
        vec_char_add_back(tstvec, tmp[i]);
    }
    Vec_char *new_vec = vec_char_copy(tstvec);
    TEST_ASSERT_EQUAL_INT32(tstvec->cap, new_vec->cap);
    TEST_ASSERT_EQUAL_INT32(tstvec->size, new_vec->size);

    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_CHAR(
            vec_char_get(tstvec, i, &e),
            vec_char_get(new_vec, i, &e)
        );
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    
    vec_char_free(tstvec);
    vec_char_free(new_vec);

}


void test_function_vec_char_add_back(void) {

    char tmp[] = {'a', 'b'};

    Vec_char *tstvec = vec_char_new(1);
    UTIL_ERR e = vec_char_add_back(tstvec, tmp[0]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);

    e = vec_char_add_back(tstvec, tmp[1]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(2, tstvec->size);
    TEST_ASSERT_EQUAL_CHAR(tmp[0], tstvec->data[0] );
    TEST_ASSERT_EQUAL_CHAR(tmp[1], tstvec->data[1] );

    vec_char_free(tstvec);

}


void test_function_vec_char_add_front(void) {

    char tmp[] = {'a', 'b'};

    // initial capacity and size check
    Vec_char *tstvec = vec_char_new(1);
    UTIL_ERR e = vec_char_add_front(tstvec, tmp[0]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);

    e = vec_char_add_front(tstvec, tmp[1]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(2, tstvec->size);
    TEST_ASSERT_EQUAL_CHAR(tmp[1], tstvec->data[0] );
    TEST_ASSERT_EQUAL_CHAR(tmp[0], tstvec->data[1] );

    vec_char_free(tstvec);

}


void test_function_vec_char_insert(void) {

    char tmp[] = "abcdefghij";

    Vec_char *tstvec = vec_char_new(1);
    for (int i = 0; i<10; i++) {
        vec_char_add_back(tstvec, tmp[i]);
    }

    char ins_val = 'z';
    UTIL_ERR e = vec_char_insert(tstvec, ins_val, 5);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_CHAR(ins_val, tstvec->data[5]);
    TEST_ASSERT_EQUAL_CHAR(tmp[9], tstvec->data[10]);
    
    vec_char_free(tstvec);

}


void test_function_vec_char_get(void) {

    char tmp[] = {'a', 'b'};
    UTIL_ERR e = E_SUCCESS;

    // initial capacity and size check
    Vec_char *tstvec = vec_char_new(1);
    vec_char_add_back(tstvec, tmp[0]);
    vec_char_add_back(tstvec, tmp[1]);

    TEST_ASSERT_EQUAL_CHAR(tmp[0], vec_char_get(tstvec, 0, &e));
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_CHAR(tmp[1], vec_char_get(tstvec, 1, &e));
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_CHAR('\0', vec_char_get(tstvec, 100, &e));
    TEST_ASSERT_TRUE(e == E_OUTOFBOUNDS);

    vec_char_free(tstvec);

}


void test_function_vec_char_delete_idx(void) {

    char tmp[] = "a cdef";

    Vec_char *tstvec = vec_char_new(1);
    for (int i = 0; i<6; i++) {
        vec_char_add_back(tstvec, tmp[i]);
    }

    UTIL_ERR e = vec_char_delete_idx(tstvec, 1);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_CHAR('\0', tstvec->data[tstvec->size]);
    for (int i = 2; i<6; i++) {
        char v = vec_char_get(tstvec, i-1, &e);
        printf("%c ", v);
        TEST_ASSERT_EQUAL_CHAR(tmp[i], v);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    printf("\n");

    TEST_ASSERT_EQUAL_INT32(5, tstvec->size);

    vec_char_free(tstvec);

}


static void print_chars(char e, FILE *f) {
    fprintf(f, "%c ", e);
}

void test_function_vec_char_print(void) {

    char tmp[] = "a cdef";

    Vec_char *tstvec = vec_char_new(1);
    for (int i = 0; i<6; i++) {
        vec_char_add_back(tstvec, tmp[i]);
    }

    vec_char_print(tstvec, stdout, print_chars);
    printf("\n");

    TEST_PASS();
    vec_char_free(tstvec);

}


static void vec_char_mapper(char *elem) {
    *elem += 1;
}

void test_function_vec_char_map(void) {

    char tmp[] = "abcdefghij";

    Vec_char *tstvec = vec_char_new(1);
    for (int i = 0; i<10; i++) {
        vec_char_add_back(tstvec, tmp[i]);
    }

    UTIL_ERR e = vec_char_map(tstvec, vec_char_mapper);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_CHAR(tmp[i] + 1, vec_char_get(tstvec, i, &e));
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    
    vec_char_free(tstvec);

}


void test_function_vec_char_map_new(void) {

    char tmp[] = "abcdefghij";

    Vec_char *tstvec = vec_char_new(1);
    for (int i = 0; i<10; i++) {
        vec_char_add_back(tstvec, tmp[i]);
    }

    UTIL_ERR e = E_SUCCESS;
    Vec_char *new_vec = vec_char_map_new(tstvec, vec_char_mapper, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    
    for (int i = 0; i<10; i++) {
        TEST_ASSERT_EQUAL_CHAR(tmp[i] + 1, vec_char_get(new_vec, i, &e));
    }
    
    vec_char_free(tstvec);
    vec_char_free(new_vec);

}


static bool vec_char_filter_func(char elem) {
    return elem % 2 == 0;
}

void test_function_vec_char_filter(void) {

    char tmp[] = "abcdefghij";

    Vec_char *tstvec = vec_char_new(1);
    for (int i = 0; i<10; i++) {
        vec_char_add_back(tstvec, tmp[i]);
    }

    UTIL_ERR e = E_SUCCESS;
    Vec_char *new_vec = vec_char_filter(tstvec, vec_char_filter_func, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    
    vec_char_print(new_vec, stdout, print_chars); printf("\n");
    for (size_t i = 0; i<new_vec->size; i++) {
        TEST_ASSERT_TRUE(vec_char_get(new_vec, i, &e) % 2 == 0);
    }
    
    vec_char_free(tstvec);
    vec_char_free(new_vec);

}

static bool vec_char_in_func(char e1, char e2) {
    return e1 + 32 == e2;
}

void test_function_vec_char_in(void) {

    char tmp[] = "abcdefghij";
    char fnd = 'f';
    char nfnd = '\t';

    Vec_char *tstvec = vec_char_new(1);
    for (int i = 0; i<10; i++) {
        vec_char_add_back(tstvec, tmp[i]);
    }    

    UTIL_ERR e = E_SUCCESS;
    intmax_t is_fnd = vec_char_in(tstvec, fnd, NULL, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_TRUE(is_fnd != -1);
    TEST_ASSERT_EQUAL_INT32(5, is_fnd);

    is_fnd = vec_char_in(tstvec, 'G', vec_char_in_func, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_TRUE(is_fnd != -1);
    TEST_ASSERT_EQUAL_INT32(6, is_fnd);

    is_fnd = vec_char_in(tstvec, nfnd, NULL, &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(-1, is_fnd);

    vec_char_free(tstvec);

}

//################ char Vector ################

int main(void) {

    
    UNITY_BEGIN();

    // Generic vector
    RUN_TEST(test_function_vector_new);
    RUN_TEST(test_function_vector_copy);
    RUN_TEST(test_function_vector_clear);
    RUN_TEST(test_function_vector_add_back);
    RUN_TEST(test_function_vector_add_front);
    RUN_TEST(test_function_vector_insert);
    RUN_TEST(test_function_vector_get);
    RUN_TEST(test_function_vector_delete_idx);
    RUN_TEST(test_function_vector_print);
    RUN_TEST(test_function_vector_map);
    RUN_TEST(test_function_vector_map_new);
    RUN_TEST(test_function_vector_filter);
    RUN_TEST(test_function_vector_in);

    // i32 vector
    RUN_TEST(test_function_vec_i32_new);
    RUN_TEST(test_function_vec_i32_copy);
    RUN_TEST(test_function_vec_i32_clear);
    RUN_TEST(test_function_vec_i32_add_back);
    RUN_TEST(test_function_vec_i32_add_front);
    RUN_TEST(test_function_vec_i32_insert);
    RUN_TEST(test_function_vec_i32_get);
    RUN_TEST(test_function_vec_i32_delete_idx);
    RUN_TEST(test_function_vec_i32_print);
    RUN_TEST(test_function_vec_i32_map);
    RUN_TEST(test_function_vec_i32_map_new);
    RUN_TEST(test_function_vec_i32_filter);
    RUN_TEST(test_function_vec_i32_in);

    // char vector
    RUN_TEST(test_function_vec_char_new);
    RUN_TEST(test_function_vec_char_copy);
    RUN_TEST(test_function_vec_char_clear);
    RUN_TEST(test_function_vec_char_add_back);
    RUN_TEST(test_function_vec_char_add_front);
    RUN_TEST(test_function_vec_char_insert);
    RUN_TEST(test_function_vec_char_get);
    RUN_TEST(test_function_vec_char_delete_idx);
    RUN_TEST(test_function_vec_char_print);
    RUN_TEST(test_function_vec_char_map);
    RUN_TEST(test_function_vec_char_map_new);
    RUN_TEST(test_function_vec_char_filter);
    RUN_TEST(test_function_vec_char_in);

    return UNITY_END();
}
