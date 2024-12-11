/*
 *    aputils
 *    a collection of general functions and structures
 *
 */

#ifndef _SORTING_H
#define _SORTING_H

#include "aputils.h"


// ############################# MERGE SORT #############################
// find stop node exclusive from starting node based on compare function
APUTIL_Node *run_stop(APUTIL_Node *n_start, int (*compare)(const void*, const void*));
// produce list of run [lwr - upr)
APUTIL_LList *make_run_list(APUTIL_Node *lwr, APUTIL_Node *upr, int (*compare)(const void*, const void*));
// returns a list of list pointers
APUTIL_LList *partition(APUTIL_LList *lst);
// merge two lists and priduce a new sorted list
APUTIL_LList *merge(APUTIL_LList *left_lst, APUTIL_LList *right_lst);
// merge-sort a list in-place
void merge_sort(APUTIL_LList *lst);
// ############################# MERGE SORT #############################

// ############################# OTHER SORT #############################
void bubble_sort(APUTIL_LList *lst);

// ############################# OTHER SORT #############################
#endif