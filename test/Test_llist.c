/*
 *    test the various linked list functions
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


static void *llist_data(const char * str) {
    char * data_str = calloc(256, sizeof(*data_str));
    strcpy(data_str, str);
    return data_str;
}

static void llist_free(void *d) {
    free(d);
}

static void llist_print(FILE *f, void *d) {
    fprintf(f, "%s\n", (char*)d);
}

void test_function_llist_new(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "test list with strings",&e);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_NOT_NULL(lst);
    TEST_ASSERT_NULL(lst->head);
    TEST_ASSERT_NULL(lst->tail);
    TEST_ASSERT_EQUAL_INT32(0, lst->cnt);
    printf("%s\n", lst->desc);

    aputil_llist_free(lst);

}

void test_function_llist_push(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "test list with strings", &e);
    aputil_llist_push(lst,llist_data("the tail!"));
    for (int i = 1; i<10; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push(lst,llist_data(tmp));
    }

    TEST_ASSERT_NOT_NULL(lst->head);
    TEST_ASSERT_EQUAL_INT32(10, lst->cnt);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("the tail!", (char*)lst->tail->data, strlen("the tail!"));

    aputil_llist_print_all(lst, stdout, llist_print);
    aputil_llist_free(lst);

}

void test_function_llist_pop(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "test list with strings", &e);
    char buff[64];

    aputil_llist_push(lst,llist_data("doomed to die!"));
    strcpy(buff,(char*)lst->head->data);
    char * a_str = aputil_llist_pop(lst, &e);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buff, a_str, strlen(buff));
    TEST_ASSERT_EQUAL_INT32(0, lst->cnt);
    TEST_ASSERT_NULL(lst->head);
    TEST_ASSERT_NULL(lst->tail);

    
    
    aputil_llist_push(lst,llist_data("the tail!"));
    for (int i = 1; i<10; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push(lst,llist_data(tmp));
    }

    APUTIL_Node *snd_node = lst->head->next, *tail_node = lst->tail;
    strcpy(buff,(char*)lst->head->data);

    char *popped = aputil_llist_pop(lst, &e);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_NOT_NULL(popped);
    TEST_ASSERT_TRUE(snd_node == lst->head);
    TEST_ASSERT_TRUE(tail_node == lst->tail);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buff, popped, strlen(buff)); 
    TEST_ASSERT_EQUAL_INT32(9, lst->cnt);
    printf("%s\n", popped);   

    aputil_llist_free(lst);
    free(popped);

}

void test_function_llist_push_back(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "test list with strings", &e);
    aputil_llist_push_back(lst,llist_data("the head!"));
    for (int i = 1; i<10; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push_back(lst,llist_data(tmp));
    }

    TEST_ASSERT_NOT_NULL(lst->head);
    TEST_ASSERT_NOT_NULL(lst->tail);
    TEST_ASSERT_EQUAL_INT32(10, lst->cnt);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("the head!", (char*)lst->head->data, strlen("the head!"));

    aputil_llist_print_all(lst, stdout, llist_print);
    aputil_llist_free(lst);

}

void test_function_llist_pop_back(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "test list with strings", &e);
    char buff[64];

    aputil_llist_push_back(lst,llist_data("doomed to die!"));
    strcpy(buff,(char*)lst->head->data);
    char * a_str = aputil_llist_pop_back(lst, &e);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buff, a_str, strlen(buff));
    TEST_ASSERT_EQUAL_INT32(0, lst->cnt);
    TEST_ASSERT_NULL(lst->head);
    TEST_ASSERT_NULL(lst->tail);

    aputil_llist_push_back(lst,llist_data("the head!"));
    for (int i = 1; i<9; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push_back(lst,llist_data(tmp));
    }
    aputil_llist_push_back(lst,llist_data("the tail!"));
    strcpy(buff,(char*)lst->tail->data);

    char *popped = aputil_llist_pop_back(lst, &e);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_NOT_NULL(popped);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buff, popped, strlen(buff)); 
    TEST_ASSERT_EQUAL_INT32(9, lst->cnt);
    printf("%s\n", popped);   

    aputil_llist_free(lst);
    free(popped);

}




int main(void) {

    
    UNITY_BEGIN();

    // linked list
    RUN_TEST(test_function_llist_new);
    RUN_TEST(test_function_llist_push);
    RUN_TEST(test_function_llist_pop);
    RUN_TEST(test_function_llist_push_back);
    RUN_TEST(test_function_llist_pop_back);
     

    return UNITY_END();
}
