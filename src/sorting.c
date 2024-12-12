/*
 *  sorting algorithms used in lib
 *
 */

#include "../include/aputils.h"

// ############## BUBBLE SORT LLIST ##############
void bubble_sort(APUTIL_LList *lst) {
    // just for fun, not useful unless very small list
    if (!lst) return;
    if (!lst->head) return;
    
    APUTIL_Node *cur;
    bool swapped;
    do {
        swapped = false;
        cur = lst->head;
        while (cur) {
            if (cur->next) {
                if (lst->compare(cur->data, cur->next->data) > 0) {
                    aputil_llist_nodeswap(cur, cur->next);
                    swapped = true;
                }
            }
            cur = cur->next;
        }
    } while (swapped);
}

// ############## BUBBLE SORT LLIST ##############

// ############## MERGE SORT LLIST ##############

    /*

     partition list into runs then merge bottom up
     
     data := [ 1, 1, 3, 7, 2, 4, 5, 6, 2, 3 ]
     
     partition pass (single pass creating lists of runs):
     [ [ 1, 1, 3, 7], [2, 4, 5, 6], [2, 3] ]
    
     merge sublists in pairs until only one subllist

    */


// produce list of run [lwr - upr)
APUTIL_LList *make_run_list(APUTIL_Node *lwr, APUTIL_Node *upr, int (*compare)(const void*, const void*)) {
    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *run_list = aputil_llist_new(NULL, NULL, compare, "run", &e);
    APUTIL_Node *cur = lwr;
    while (cur != upr) {
        aputil_llist_push_back(run_list, cur->data);
        cur = cur->next;
    }

    return run_list;
}


// find the node that ends (exclusive) the run. if no run then next node
APUTIL_Node *run_stop(APUTIL_Node *n_start, int (*compare)(const void*, const void*)) {
    APUTIL_Node *cur = n_start;
    while (cur->next) {
        if (compare(cur->data, cur->next->data) > 0) {
            return cur->next;
        }
        cur = cur->next;
    }
    return cur->next; // no run or end of list
}


// returns a list of list pointers containing run lists
APUTIL_LList *partition(const APUTIL_LList *lst) {
    if (!lst) return (APUTIL_LList*)0;
    if (!lst->head) return (APUTIL_LList*)0;

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *partitions = aputil_llist_new(NULL, NULL, lst->compare, "partitions", &e);
    if (!partitions || e) {
        return (APUTIL_LList*)0;    // expand later after working alg
    }

    APUTIL_Node *cur = lst->head, *upr=NULL;
    while (cur) {
        
        upr = run_stop(cur, lst->compare);
        APUTIL_LList *run = make_run_list(cur, upr, lst->compare);
        aputil_llist_push_back(partitions, run);
        cur = upr;
    }

    return partitions;
}


static void fill_list(APUTIL_LList *dest_lst, const APUTIL_LList *src_lst) {
    if (!src_lst) return;
    if (!src_lst->head) return;
    APUTIL_Node *cur = src_lst->head;
    while (cur) {
        aputil_llist_push_back(dest_lst, cur->data);
        cur = cur->next;
    }
}


APUTIL_LList *merge(APUTIL_LList *left_lst, APUTIL_LList *right_lst) {
    // [ 1, 1, 3, 7], [2, 4, 5, 6] -> [1, 1, 2, 3, 4, 5, 6, 7]
    // pull first element from each, compare, push back based on compare
    //  > put the other back from whence it came
    // repeat. If left or right runs out, empty the remainder of the other list to back

    // left and right lists are always sorted, if one is null directly return other
    if (!left_lst & !right_lst) return (APUTIL_LList*)0;
    if (!left_lst) return right_lst;
    if (!right_lst) return left_lst;

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *merged = aputil_llist_new(NULL, NULL, left_lst->compare, "merged", &e);
    if (!merged || e) {
        return (APUTIL_LList*)0;
    }

    void *left_value, *right_value;
    while (left_lst->head || right_lst->head) {
       
        // both
        if (left_lst->head && right_lst->head) {
            left_value = aputil_llist_pop(left_lst, &e);
            right_value = aputil_llist_pop(right_lst, &e);
            if (left_lst->compare(left_value, right_value) < 0 ) {
                aputil_llist_push_back(merged, left_value);
                aputil_llist_push(right_lst, right_value);
            } else {
                aputil_llist_push_back(merged, right_value);
                aputil_llist_push(left_lst, left_value);
            }

        // left only
        } else if(left_lst->head) {
            fill_list(merged, left_lst);
            break;

        // right only
        } else {
            fill_list(merged, right_lst);
            break;
        }

    }

    return merged;
}


void merge_sort(APUTIL_LList *lst) {
    if (!lst) return;
    if (!lst->head) return;

    UTIL_ERR e = E_SUCCESS;
    APUTIL_LList *output = aputil_llist_new(NULL, NULL, lst->compare, "merge sorted", &e);
    if (!output || e) {
        return;
    }
    
    APUTIL_LList *ps = partition(lst);

    // cycle over the partitions in pairs
    // [ [a], [b], [c], [d], [e] ]
    // [ [ab], [cd], [e] ]
    // [ [abcd], [e] ]
    // [ [abcde] ]

    // top loop iterating until we reduce the partition to a single element list
    APUTIL_LList *left = NULL, *right = NULL;
    do {
        
        aputil_llist_clear(output, true);
        while (ps->cnt > 0) {

            // grab pairs
            left = aputil_llist_pop(ps, &e);
            right = aputil_llist_pop(ps, &e);

            // push merged result to head
            aputil_llist_push_back(output, merge(left, right));

        }

        // transfer stage back to ps
        fill_list(ps, output);
        

    } while (output->cnt > 1);

    aputil_llist_clear(lst, true);
    fill_list(lst, output->head->data);
    aputil_llist_free(output, true);
    aputil_llist_free(ps, true);

}

// ############## MERGE SORT LLIST ##############
