/*
 *  generic list
 *      > data is void pointer
 *      
 *      ToDo
 *          - delete node
 *          - swap nodes
 *          - clear
 *          - in
 *          - map, map_new
 *          - filter
 *          - sort
 */

#include "../include/aputils.h"


APUTIL_LList *aputil_llist_new(
    void (*free)(void*),    // free data, can be null, and won't free data
    const char *desc,
    UTIL_ERR *e
) {
    APUTIL_LList *new_list = malloc(sizeof(*new_list));
    if (!new_list) {
        *e = E_BAD_ALLOC;
        return (APUTIL_LList*)0;
    }

    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->cnt = 0;
    new_list->free = free;
    strncpy(new_list->desc, desc, sizeof(new_list->desc)-1);

    return new_list;

}


void aputil_llist_free(APUTIL_LList *lst) {
    if (!lst) return;
    if (!lst->head) {
        free(lst);
        return;
    }

    APUTIL_Node *cur = lst->head;
    APUTIL_Node *prev = NULL;
    while (cur) {
        prev = cur;
        cur = cur->next;
        if (lst->free) lst->free(prev->data);
        free(prev);
    }
    free(lst);

}


static APUTIL_Node *make_node(void) {
    APUTIL_Node * new_node = malloc(sizeof(*new_node));
    if (!new_node) return (APUTIL_Node*)0;
    new_node->data = new_node->next = new_node->prev = NULL;
    return new_node;
}


UTIL_ERR aputil_llist_push(APUTIL_LList *lst, void *elem) {
    if (!lst) return E_EMPTY_OBJ;
    if (!elem) return E_EMPTY_ARG;

    APUTIL_Node *new_node = make_node();
    if (!new_node) return E_BAD_ALLOC;

    new_node->data = elem;
    new_node->next = lst->head;
    if (lst->head) lst->head->prev = new_node;
    lst->head = new_node;

    lst->cnt++;
    if (lst->cnt == 1) lst->tail = lst->head;
    
    return E_SUCCESS;

}


void *aputil_llist_pop(APUTIL_LList *lst, UTIL_ERR *e) {
    // doesn't free data when popped, caller must free if alloced
    if (!lst) {
        *e =  E_EMPTY_OBJ;
        return (APUTIL_Node*)0;
    }
    if (!lst->head) {
        *e = E_NODATA;
        return (APUTIL_Node*)0;
    }

    APUTIL_Node *popped = lst->head;
    lst->head = popped->next;
    if (lst->head) lst->head->prev = NULL;

    lst->cnt--;
    if (lst->cnt == 0) lst->tail = NULL;
    
    void *data = popped->data;
    free(popped);   // node not data
    return data;

}


UTIL_ERR aputil_llist_push_back(APUTIL_LList *lst, void *elem) {
    if (!lst) return E_EMPTY_OBJ;
    if (!elem) return E_EMPTY_ARG;

    APUTIL_Node *new_node = make_node();
    if (!new_node) return E_BAD_ALLOC;

    new_node->data = elem;

    new_node->prev = lst->tail;
    if (lst->tail) lst->tail->next = new_node;
    lst->tail = new_node;

    lst->cnt++;
    if (lst->cnt == 1) lst->head = new_node;
    
    return E_SUCCESS;

}


void *aputil_llist_pop_back(APUTIL_LList *lst, UTIL_ERR *e) {
    // doesn't free data when popped, caller must free if alloced
    if (!lst) {
        *e =  E_EMPTY_OBJ;
        return (APUTIL_Node*)0;
    }
    if (!lst->head) {
        *e = E_NODATA;
        return (APUTIL_Node*)0;
    }

    APUTIL_Node *popped = lst->tail;
    lst->tail = popped->prev;
    if (lst->tail) lst->tail->next = NULL;

    lst->cnt--;
    if (lst->cnt == 0) lst->head = NULL;
    
    void *data = popped->data;
    free(popped);   // node not data
    return data;

}




void aputil_llist_print_node(APUTIL_Node *node, FILE *f, void (*data_print)(FILE*, void*)) {
    if (!node || !f || !data_print) return;

    fprintf(f, "cur: %p\nnext: %p\nprev: %p\n", node, node->next, node->prev);
    data_print(f, node->data);
}

void aputil_llist_print_all(APUTIL_LList *lst, FILE *f, void (*data_print)(FILE*, void*)) {
    if (!lst || !f || !data_print) return;

    fprintf(f, "list: %p\t%s\n\n", lst, lst->desc);
    APUTIL_Node *cur = lst->head;
    while (cur) {
        aputil_llist_print_node(cur, f, data_print);
        fprintf(f, "\n");
        cur = cur->next;
    }
}
