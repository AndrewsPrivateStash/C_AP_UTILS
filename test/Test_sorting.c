/*
 *    test src/sorting.c
 */

#include <unity/unity.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../include/aputils.h"
#include "../include/sorting.h"


void setUp(void) {
    /* This is run before EACH TEST */
}

void tearDown(void) {}



static int list_comp(const void *d1, const void* d2) {
    return *(int*)d1 - *(int*)d2;
}

static void llist_print(FILE *f, void *d) {
    fprintf(f, "%d ", *(int*)d);
}

static void llist_print_addr(FILE *f, void *d) {
    fprintf(f, "%p", d);
}

static void llist_part_print(FILE *f, void *lst) {
    APUTIL_Node *cur = ((APUTIL_LList*)(lst))->head;
    fprintf(f, "[");
    while (cur) {
        if (!cur->next) {
            fprintf(f, "%d", *(int*)(cur->data));
        } else {
            fprintf(f, "%d, ", *(int*)(cur->data));
        }
        cur = cur->next;
    }
    fprintf(f, "]");
}

void test_function_sort_run_stop(void) {
    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(NULL, NULL, list_comp, "merge_sort run_stop test", &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    int vals[] = {1, 1, 3, 7, 2, 4, 5, 6, 2, 3};

    for (int i = 0; i<10; i++) {
        e = aputil_llist_push_back(lst, &vals[i]);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }

    APUTIL_Node *stop = run_stop(lst->head, lst->compare);
    TEST_ASSERT_EQUAL_INT32(2, *(int*)(stop->data));
    TEST_ASSERT_EQUAL_INT32(7, *(int*)(stop->prev->data));

    aputil_llist_free(lst, true);
}


void test_function_sort_make_run_list(void) {
    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(NULL, NULL, list_comp, "merge_sort make_run test", &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    int vals[] = {1, 1, 3, 7, 2, 4, 5, 6, 2, 3};

    for (int i = 0; i<10; i++) {
        e = aputil_llist_push_back(lst, &vals[i]);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    APUTIL_Node *stop = run_stop(lst->head, lst->compare);
    APUTIL_LList *run = make_run_list(lst->head, stop, lst->compare);
    TEST_ASSERT_EQUAL_INT32(4, run->cnt);
    aputil_llist_print_all(run, stdout, llist_print);


    aputil_llist_free(run, true);
    aputil_llist_free(lst, true);
}


void test_function_sort_partition(void) {
    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(NULL, NULL, list_comp, "merge_sort partition test", &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    int vals[] = {1, 1, 3, 7, 2, 4, 5, 6, 2, 3};

    for (int i = 0; i<10; i++) {
        e = aputil_llist_push_back(lst, &vals[i]);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }

    APUTIL_LList *part = partition(lst);
    TEST_ASSERT_NOT_NULL(part);

    aputil_llist_print_all(part, stdout, llist_print_addr);

    APUTIL_Node *cur = part->head;  
    fprintf(stdout, "[ ");
    while (cur) {
        llist_part_print(stdout, cur->data);
        if (cur->next) fprintf(stdout, ", ");
        cur = cur->next;
    }
    fprintf(stdout, " ]\n");


    // free lists
    cur = part->head;
    while (cur) {
        aputil_llist_free((APUTIL_LList*)cur->data, true);
        cur = cur->next;
    }
    aputil_llist_free(part, false);
    aputil_llist_free(lst, false);

    TEST_PASS();   
}



static void print_int_list(const APUTIL_LList *lst) {
    APUTIL_Node *cur = lst->head;
    fprintf(stdout, "[ ");
    while (cur) {
        if (cur->next) {
            fprintf(stdout, "%d, ", *(int*)cur->data);
        } else {
            fprintf(stdout, "%d", *(int*)cur->data);
        }
        cur = cur->next;
    }
    fprintf(stdout, " ]\n");
}

static void print_int_list_trunc(const APUTIL_LList *lst, int cnt) {
    APUTIL_Node *cur = lst->head;
    int i = 0;
    fprintf(stdout, "[ ");
    while (cur && i < cnt) {
        if (cur->next) {
            fprintf(stdout, "%d, ", *(int*)cur->data);
        } else {
            fprintf(stdout, "%d", *(int*)cur->data);
        }
        i++;
        cur = cur->next;
    }
    fprintf(stdout, " ]\n");
}

void test_function_sort_merge(void) {
    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *left_lst = aputil_llist_new(NULL, NULL, list_comp, "merge_sort merge left test", &e);
    APUTIL_LList *right_lst = aputil_llist_new(NULL, NULL, list_comp, "merge_sort merge right test", &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    int left_vals[] = {1, 1, 3, 7};
    int right_vals[] = {2, 4, 5, 6, 9, 9};

    for (int i = 0; i<4; i++) {
        e = aputil_llist_push_back(left_lst, &left_vals[i]);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }

    for (int i = 0; i<6; i++) {
        e = aputil_llist_push_back(right_lst, &right_vals[i]);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }

    APUTIL_LList *merged = merge(left_lst, right_lst);
    print_int_list(merged);

    aputil_llist_clear(merged, true);
    for (int i = 0; i<4; i++) {
        e = aputil_llist_push_back(left_lst, &left_vals[i]);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    merged = merge(left_lst, NULL);
    print_int_list(merged);

    aputil_llist_clear(merged, true);
    merged = merge(NULL, NULL);
    TEST_ASSERT_NULL(merged);


    aputil_llist_free(left_lst, true);
    aputil_llist_free(right_lst, true);
    aputil_llist_free(merged, true);

    TEST_PASS();

}

// static void make_large_rnd_list(APUTIL_LList *lst, int cnt) {
//     aputil_llist_clear(lst, true);
//     int tmp[cnt];
//     for (int i = 0; i<cnt; i++) {
//         tmp[i] = rand() % cnt;
//         aputil_llist_push_back(lst, &tmp[i]);
//     }
    
// }

static bool is_sorted(APUTIL_LList *lst) {
    if (!lst) return false;
    if (!lst->head) return true;
    APUTIL_Node *cur = lst->head;
    while (cur) {
        if (cur->next) {
            if (lst->compare(cur->data, cur->next->data) > 0) return false;
        }
        cur = cur->next;
    }
    return true;
}

void test_function_sort_merge_sort(void) {
    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *lst = aputil_llist_new(NULL, NULL, list_comp, "merge_sort test", &e);
    TEST_ASSERT_TRUE(e == E_SUCCESS);
    int vals[] = {1, 1, 3, 7, 2, 4, 5, 6, 2, 3};
    int cnt = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i<cnt; i++) {
        e = aputil_llist_push_back(lst, &vals[i]);
        TEST_ASSERT_TRUE(e == E_SUCCESS);
    }
    int lst_cnt = lst->cnt;
    printf("before sort..\n");
    print_int_list(lst);
    merge_sort(lst);
    printf("after sort..\n");
    print_int_list(lst);
    TEST_ASSERT_EQUAL_INT32(lst_cnt, lst->cnt);

    // large list sort
    clock_t start, stop;
    cnt = 10000;
    aputil_llist_clear(lst, true);
    int *tmp = malloc(sizeof(int) * cnt);
    for (int i = 0; i<cnt; i++) {
        tmp[i] = rand() % 10000;
        aputil_llist_push_back(lst, &tmp[i]);
    }
    print_int_list_trunc(lst, 50);
    
    start = clock();
    merge_sort(lst);
    stop = clock();
    fprintf(stdout, "merge-sort: %f s\n", ((double) (stop - start)) / CLOCKS_PER_SEC);

    print_int_list_trunc(lst, 50);
    TEST_ASSERT_EQUAL_INT32(cnt, lst->cnt);
    TEST_ASSERT_TRUE(is_sorted(lst));
    
    free (tmp);
    aputil_llist_free(lst, true);

}



int main(void) {

    srand( time(NULL) );
    
    UNITY_BEGIN();

    // merge sort
    RUN_TEST(test_function_sort_run_stop);
    RUN_TEST(test_function_sort_make_run_list);
    RUN_TEST(test_function_sort_partition);
    RUN_TEST(test_function_sort_merge);
    RUN_TEST(test_function_sort_merge_sort);
    

    return UNITY_END();
}
