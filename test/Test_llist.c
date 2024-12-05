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
    APUTIL_LList *lst = aputil_llist_new(llist_free, "test list with strings", &e);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_NOT_NULL(lst);
    TEST_ASSERT_NULL(lst->head);
    TEST_ASSERT_NULL(lst->tail);
    TEST_ASSERT_EQUAL_INT32(0, lst->cnt);
    printf("%s\n", lst->desc);

    aputil_llist_free(lst, false);

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
    aputil_llist_free(lst, false);

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

    aputil_llist_free(lst, false);
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
    aputil_llist_free(lst, false);

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

    aputil_llist_free(lst, false);
    free(popped);

}


static bool llist_data_compare(const void *s1, const void *s2) {
    return strcmp((char*)s1, (char*)s2) == 0;
}

void test_function_llist_in(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "llist_in test", &e);

    for (int i = 0; i<20; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push(lst,llist_data(tmp));
    }

    void *fnd = lst->tail->data;
    void *not_fnd = lst->head->data;
    aputil_llist_delete(lst, lst->head);

    TEST_ASSERT_NOT_NULL(aputil_llist_in(lst, fnd, llist_data_compare, &e));
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_NULL(aputil_llist_in(lst, not_fnd, llist_data_compare, &e));
    TEST_ASSERT_TRUE(e==E_SUCCESS);


    aputil_llist_free(lst, false);
}

void test_function_llist_delete(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "delete from front", &e);

    // delete from front
    for (int i = 0; i<5; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push(lst,llist_data(tmp));
    }

    while (lst->head) {
        aputil_llist_print_all(lst, stdout, llist_print);
        e = aputil_llist_delete(lst, lst->head);
        TEST_ASSERT_TRUE(e==E_SUCCESS);
    }
    aputil_llist_print_all(lst, stdout, llist_print);
    TEST_ASSERT_EQUAL_INT32(0, lst->cnt);
    TEST_ASSERT_NULL(lst->head);
    TEST_ASSERT_NULL(lst->tail);

    // delete from back
    strcpy(lst->desc, "delete from back");
    for (int i = 0; i<5; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push(lst,llist_data(tmp));
    }
    
    while (lst->tail) {
        aputil_llist_print_all(lst, stdout, llist_print);
        fflush(stdout);
        e = aputil_llist_delete(lst, lst->tail);
        TEST_ASSERT_TRUE(e==E_SUCCESS);
    }
    aputil_llist_print_all(lst, stdout, llist_print);
    TEST_ASSERT_EQUAL_INT32(0, lst->cnt);
    TEST_ASSERT_NULL(lst->head);
    TEST_ASSERT_NULL(lst->tail);

    // delete from middle
    strcpy(lst->desc, "delete from middle");
    for (int i = 0; i<5; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        if (i%2==0) aputil_llist_push(lst,llist_data(tmp));
        else aputil_llist_push_back(lst,llist_data(tmp));
    }

    while (lst->head->next) {
        aputil_llist_print_all(lst, stdout, llist_print);
        e = aputil_llist_delete(lst, lst->head->next);
        TEST_ASSERT_TRUE(e==E_SUCCESS);
    }
    aputil_llist_print_all(lst, stdout, llist_print);
    e = aputil_llist_delete(lst, lst->head);
    aputil_llist_print_all(lst, stdout, llist_print);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(0, lst->cnt);
    TEST_ASSERT_NULL(lst->head);
    TEST_ASSERT_NULL(lst->tail);


    aputil_llist_free(lst, false);

}

void test_function_llist_clear(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "llist_clear test", &e);

    for (int i = 0; i<20; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push(lst,llist_data(tmp));
    }

    e = aputil_llist_clear(lst);
    if (e) fprintf(stdout, "%s\n", UTIL_ERR_PRINT(e));

    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_EQUAL_INT32(0, lst->cnt);
    TEST_ASSERT_NULL(lst->head);
    TEST_ASSERT_NULL(lst->tail);

    aputil_llist_free(lst, false);
}

void test_function_llist_copy_node(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "llist_clear test", &e);

    for (int i = 0; i<3; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push(lst,llist_data(tmp));
    }

    APUTIL_Node *s_cpy = aputil_llist_copy_node(lst->head, &e);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_TRUE(s_cpy != lst->head);
    TEST_ASSERT_TRUE(s_cpy->data == lst->head->data);
    TEST_ASSERT_TRUE(s_cpy->next == lst->head->next);
    TEST_ASSERT_TRUE(s_cpy->prev == lst->head->prev);

    aputil_llist_free(lst, false);

}

void test_function_llist_copy(void) {

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(llist_free, "llist_copy test", &e);

    for (int i = 0; i<20; i++) {
        char tmp[64] = {0};
        sprintf(tmp, "the %dth string!", i);
        aputil_llist_push(lst,llist_data(tmp));
    }

    APUTIL_LList *lst_cpy = aputil_llist_copy(lst, &e);
    TEST_ASSERT_TRUE(e==E_SUCCESS);
    TEST_ASSERT_NOT_NULL(lst_cpy);    
    TEST_ASSERT_EQUAL_INT32(lst->cnt, lst_cpy->cnt);
    TEST_ASSERT_TRUE(lst->free == lst_cpy->free);
    TEST_ASSERT_NOT_NULL(lst_cpy->head);
    TEST_ASSERT_NOT_NULL(lst_cpy->tail);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(lst->desc, lst_cpy->desc, strlen(lst->desc));

    APUTIL_Node *cur = lst->head, *cur_cpy = lst_cpy->head;
    while (cur) {
        TEST_ASSERT_TRUE(cur->data == cur_cpy->data);
        cur = cur->next;
        cur_cpy = cur_cpy->next;
    }

    aputil_llist_free(lst_cpy, true);
    aputil_llist_free(lst, false);

}







int main(void) {

    
    UNITY_BEGIN();

    // linked list
    RUN_TEST(test_function_llist_new);
    RUN_TEST(test_function_llist_push);
    RUN_TEST(test_function_llist_pop);
    RUN_TEST(test_function_llist_push_back);
    RUN_TEST(test_function_llist_pop_back);
    RUN_TEST(test_function_llist_in);
    RUN_TEST(test_function_llist_delete);
    RUN_TEST(test_function_llist_clear);
    RUN_TEST(test_function_llist_copy_node);
     RUN_TEST(test_function_llist_copy);

    return UNITY_END();
}
