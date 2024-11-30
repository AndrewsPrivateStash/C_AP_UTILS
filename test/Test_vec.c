/*
    test the various pawnvec functions
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
    Vector *tstvec = vector_new(sizeof(size_t), 100);
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

void test_function_vector_add_back(void) {

    int tmp[] = {5, 1000};

    // initial capacity and size check
    Vector *tstvec = vector_new(sizeof(size_t), 100);
    UTIL_ERR e = vector_add_back(tstvec, &tmp[0]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);

    e = vector_add_back(tstvec, &tmp[1]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(2, tstvec->size);
    TEST_ASSERT_EQUAL_INT32(tmp[0], *((size_t*)tstvec->data) );
    TEST_ASSERT_EQUAL_INT32(tmp[1], *((size_t*)(
        (char*)tstvec->data + 1 * tstvec->elem_size
    )) );

    vector_free(tstvec);

}


void test_function_vector_add_front(void) {

    int tmp[] = {5, 1000};

    // initial capacity and size check
    Vector *tstvec = vector_new(sizeof(size_t), 100);
    UTIL_ERR e = vector_add_front(tstvec, &tmp[0]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);

    e = vector_add_front(tstvec, &tmp[1]);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(2, tstvec->size);
    TEST_ASSERT_EQUAL_INT32(tmp[1], *((size_t*)tstvec->data) );
    TEST_ASSERT_EQUAL_INT32(tmp[0], *((size_t*)(
        (char*)tstvec->data + 1 * tstvec->elem_size
    )) );

    vector_free(tstvec);

}


void test_function_vector_get(void) {

    int tmp[] = {5, 1000};

    // initial capacity and size check
    Vector *tstvec = vector_new(sizeof(size_t), 100);
    vector_add_back(tstvec, &tmp[0]);
    vector_add_back(tstvec, &tmp[1]);

    TEST_ASSERT_EQUAL_INT32(tmp[0], *(size_t*)vector_get(tstvec, 0));
    TEST_ASSERT_EQUAL_INT32(tmp[1], *(size_t*)vector_get(tstvec, 1));
    TEST_ASSERT_NULL((size_t*)vector_get(tstvec, 100));

    vector_free(tstvec);

}


void test_function_vector_delete_idx(void) {

    int tmp[] = {5, 1000, 20, 0, 42, 777};

    Vector *tstvec = vector_new(sizeof(size_t), 100);
    for (int i = 0; i<6; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    UTIL_ERR e = vector_delete_idx(tstvec, 1);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    for (int i = 2; i<6; i++) {
        TEST_ASSERT_EQUAL_INT32(tmp[i], *(size_t*)vector_get(tstvec, i-1));
    }

    TEST_ASSERT_EQUAL_INT32(5, tstvec->size);


    vector_free(tstvec);

}



static void print_ints(void *e, FILE *f) {
    fprintf(f, "%d ", *(int*)e);
}

void test_function_vector_print(void) {

    int tmp[] = {5, 1000, 20, 0, 42, 777};

    Vector *tstvec = vector_new(sizeof(size_t), 100);
    for (int i = 0; i<6; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    vector_print(tstvec, stdout, print_ints);
    printf("\n");

    TEST_PASS();


    vector_free(tstvec);

}

int main(void) {

    
    UNITY_BEGIN();

    RUN_TEST(test_function_vector_new);
    RUN_TEST(test_function_vector_add_back);
    RUN_TEST(test_function_vector_add_front);
    RUN_TEST(test_function_vector_get);
    RUN_TEST(test_function_vector_clear);
    RUN_TEST(test_function_vector_delete_idx);
    RUN_TEST(test_function_vector_print);


    return UNITY_END();
}
