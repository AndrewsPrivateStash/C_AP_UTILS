/*
    test the various vector functions
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
    Vector *tstvec = vector_new(sizeof(size_t), 1);
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
    Vector *tstvec = vector_new(sizeof(size_t), 1);
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
    Vector *tstvec = vector_new(sizeof(size_t), 1);
    vector_add_back(tstvec, &tmp[0]);
    vector_add_back(tstvec, &tmp[1]);

    TEST_ASSERT_EQUAL_INT32(tmp[0], *(size_t*)vector_get(tstvec, 0));
    TEST_ASSERT_EQUAL_INT32(tmp[1], *(size_t*)vector_get(tstvec, 1));
    TEST_ASSERT_NULL((size_t*)vector_get(tstvec, 100));

    vector_free(tstvec);

}


void test_function_vector_delete_idx(void) {

    int tmp[] = {5, 1000, 20, 0, 42, 777};

    Vector *tstvec = vector_new(sizeof(size_t), 1);
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

    Vector *tstvec = vector_new(sizeof(size_t), 1);
    for (int i = 0; i<6; i++) {
        vector_add_back(tstvec, &tmp[i]);
    }

    vector_print(tstvec, stdout, print_ints);
    printf("\n");

    TEST_PASS();


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

//################ i32 Vector ################

int main(void) {

    
    UNITY_BEGIN();

    // Generic Vecor
    RUN_TEST(test_function_vector_new);
    RUN_TEST(test_function_vector_add_back);
    RUN_TEST(test_function_vector_add_front);
    RUN_TEST(test_function_vector_get);
    RUN_TEST(test_function_vector_clear);
    RUN_TEST(test_function_vector_delete_idx);
    RUN_TEST(test_function_vector_print);

    // i32 vector
    RUN_TEST(test_function_vec_i32_new);
    RUN_TEST(test_function_vec_i32_add_back);
    RUN_TEST(test_function_vec_i32_add_front);
    RUN_TEST(test_function_vec_i32_get);
    RUN_TEST(test_function_vec_i32_clear);
    RUN_TEST(test_function_vec_i32_delete_idx);
    RUN_TEST(test_function_vec_i32_print);




    return UNITY_END();
}
