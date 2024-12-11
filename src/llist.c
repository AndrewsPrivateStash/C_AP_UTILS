/*
 *  generic list
 *      > data is void pointer
 *      
 *      ToDo
 *          - sort
 *          - reverse
 */

#include "../include/aputils.h"


APUTIL_LList *aputil_llist_new(
    void (*free)(void*),                // free data, can be null, used when preserve = false in free function
    void *(*copydata)(const void*),     // copy data, can be null, and copies will be shallow
    int (*compare)(const void*, const void*),   // compare function, can be null, used to sort or otherwise compare two data elements
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
    new_list->copydata = copydata;
    new_list->compare = compare;
    strncpy(new_list->desc, desc, sizeof(new_list->desc)-1);

    return new_list;

}


void aputil_llist_free(APUTIL_LList *lst, bool preserve) {
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
        if (lst->free && !preserve) lst->free(prev->data);
        free(prev);
    }
    free(lst);

}


void aputil_llist_print_node(const APUTIL_Node *node, FILE *f, void (*data_print)(FILE*, void*)) {
    if (!node || !f || !data_print) return;

    fprintf(f, "cur: %p\nnext: %p\nprev: %p\n", node, node->next, node->prev);
    data_print(f, node->data);
}

void aputil_llist_print_all(const APUTIL_LList *lst, FILE *f, void (*data_print)(FILE*, void*)) {
    if (!lst || !f || !data_print) return;

    fprintf(f, "list: %p\t%s: cnt:%lu\nhead:%p\ttail:%p\n\n", lst, lst->desc, lst->cnt, lst->head, lst->tail);
    APUTIL_Node *cur = lst->head;
    while (cur) {
        aputil_llist_print_node(cur, f, data_print);
        fprintf(f, "\n");
        cur = cur->next;
    }
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
    // doesn't free data when popped, caller must free if allocated
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


UTIL_ERR aputil_llist_delete(APUTIL_LList *lst, APUTIL_Node *n, bool preserve) {
    // frees data at node if free function defined and not preserved
    if (!lst) return E_EMPTY_OBJ;
    if (!n) return E_EMPTY_ARG;
    if (!aputil_llist_node_exists(lst, n)) return E_DOESNT_EXIST;
    
    APUTIL_Node *prev_node = n->prev, *next_node = n->next;
    
    if (n == lst->head) {
        lst->head = next_node;
        if (next_node) next_node->prev = NULL;
        else lst->tail = NULL;

    } else if (n == lst->tail) {
        lst->tail = prev_node;
        if (prev_node) prev_node->next = NULL;

    } else {
        prev_node->next = next_node;
        next_node->prev = prev_node;
    }

    if (lst->free && !preserve) lst->free(n->data);
    free(n);
    lst->cnt--;

    return E_SUCCESS;
}


static void copy_node_values(APUTIL_Node *n_dest, const APUTIL_Node *n_src) {
    if (!n_dest || !n_src) return;
    n_dest->data = n_src->data;
    n_dest->next = n_src->next;
    n_dest->prev = n_src->prev;
}

APUTIL_Node *aputil_llist_copy_node(const APUTIL_LList *lst, const APUTIL_Node *n, bool deep, UTIL_ERR *e) {
    if (!lst) {
        *e = E_EMPTY_OBJ;
        return (APUTIL_Node*)0;
    }

    if (!n) {
        *e = E_EMPTY_ARG;
        return (APUTIL_Node*)0;
    }

    APUTIL_Node *new_node = make_node();
    if (!new_node) {
        *e = E_BAD_ALLOC;
        return (APUTIL_Node*)0;
    }

    if (lst->copydata && deep) {
        // deep copy
        copy_node_values(new_node, n);
        new_node->data = lst->copydata(n->data);
    } else {
        // shallow copy
        copy_node_values(new_node, n);
    }

    return new_node;
}


APUTIL_LList *aputil_llist_copy(const APUTIL_LList *lst, bool deep, UTIL_ERR *e) {
    if (!lst) {
        *e = E_EMPTY_OBJ;
        return (APUTIL_LList*)0;
    }

    APUTIL_LList *new_list = aputil_llist_new(lst->free, lst->copydata, lst->compare, lst->desc, e);
    if (!new_list || *e) {
        if (new_list) aputil_llist_free(new_list, true);
        if (*e == E_SUCCESS) *e = E_BAD_ALLOC;
        return (APUTIL_LList*)0;
    }

    APUTIL_Node *cur = lst->head;
    while (cur) {
        if (new_list->copydata && deep) {
            // deep copy
            *e = aputil_llist_push_back(new_list, new_list->copydata(cur->data));
        } else {
            // shallow copy
            *e = aputil_llist_push_back(new_list, cur->data);
        }
        
        if (*e) {
            // free the data if newly allocated from deep copy
            bool pres = new_list->copydata && deep ? false: true;
            aputil_llist_free(new_list, pres);
            return (APUTIL_LList*)0;
        }
        cur = cur->next;
    }

    return new_list;
}


UTIL_ERR aputil_llist_clear(APUTIL_LList *lst, bool preserve) {
    if (!lst) return E_EMPTY_OBJ;
    if (!lst->head) return E_SUCCESS;

    APUTIL_Node *cur = lst->head;
    UTIL_ERR del_err;
    bool pres = lst->free && !preserve ? false: true;
    while (cur) {
        del_err = aputil_llist_delete(lst, cur, pres);
        if (del_err) return del_err;
        cur = lst->head;
    }

    return E_SUCCESS;
}


bool aputil_llist_node_exists(const APUTIL_LList *lst, const APUTIL_Node *n) {
    if (!lst) return false;
    if (!n) return false;
    if (!lst->head) return false;

    APUTIL_Node *cur = lst->head;
    while (cur) {
        if (n == cur) return true;
        cur = cur->next;
    }
    
    return false;
}


APUTIL_Node *aputil_llist_in(const APUTIL_LList *lst, const void *elem, bool(*equalfunc)(const void*, const void*), UTIL_ERR *e) {
    if (!lst) {
        *e = E_EMPTY_OBJ;
        return (APUTIL_Node*)0;
    }

    if (!elem) {
        *e = E_EMPTY_ARG;
        return (APUTIL_Node*)0;
    }

    if (!equalfunc) {
        *e = E_EMPTY_FUNC;
        return (APUTIL_Node*)0;
    }

    APUTIL_Node *cur = lst->head;
    while (cur) {
        if (equalfunc(elem, cur->data)) return cur;
        cur = cur->next;
    }
   
    return (APUTIL_Node*)0;    
}

UTIL_ERR aputil_llist_map(APUTIL_LList *lst, void(*mapfunc)(void*)) {
    if (!lst) return E_EMPTY_OBJ;
    if (!mapfunc) return E_EMPTY_FUNC;
    if (!lst->head) return E_NODATA;

    APUTIL_Node *cur = lst->head;
    while (cur) {
        mapfunc(cur->data);
        cur = cur->next;
    }

    return E_SUCCESS;
}


APUTIL_LList *aputil_llist_map_new(const APUTIL_LList *lst, void(*mapfunc)(void*), UTIL_ERR *e) {
    if (!lst) {
        *e = E_EMPTY_OBJ;
        return (APUTIL_LList*)0;
    }

    if (!lst->head) {
        *e = E_NODATA;
        return (APUTIL_LList*)0;
    }

    if (!mapfunc || !lst->copydata) {
        *e = E_EMPTY_FUNC;
        return (APUTIL_LList*)0;
    }

    APUTIL_LList *new_list = aputil_llist_copy(lst, true, e);
    if (*e) {
        if (new_list) aputil_llist_free(new_list, false);
        return (APUTIL_LList*)0;
    }
    *e = aputil_llist_map(new_list, mapfunc);
    if (*e) {
        aputil_llist_free(new_list, false);
        return (APUTIL_LList*)0;
    }

    return new_list;
}

static APUTIL_LList *copy_list_container(const APUTIL_LList *lst, const char* str_app) {
    UTIL_ERR e = E_SUCCESS;

    char buff[256] = {0};
    strcpy(buff, lst->desc);
    if (strlen(str_app)) {
        strcat(buff, ": ");
        strcat(buff, str_app);
    }

    APUTIL_LList *new_list = aputil_llist_new(
        lst->free,
        lst->copydata,
        lst->compare,
        buff,
        &e
    );
    if (!new_list || e) return (APUTIL_LList*)0;

    return new_list;
}

APUTIL_LList *aputil_llist_filter(const APUTIL_LList *lst, bool(*filterfunc)(void*), bool copy, UTIL_ERR *e) {
    if (!lst) {
        *e = E_EMPTY_OBJ;
        return (APUTIL_LList*)0;
    }

    if (!lst->head) {
        *e = E_NODATA;
        return (APUTIL_LList*)0;
    }

    if (!filterfunc) {
        *e = E_EMPTY_FUNC;
        return (APUTIL_LList*)0;
    }

    if (copy && !lst->copydata) {
        *e = E_EMPTY_FUNC;
        return (APUTIL_LList*)0;
    }

    APUTIL_LList *new_list = copy_list_container(lst, "filtered");
    if (!new_list) {
        *e = E_BAD_ALLOC;
        return (APUTIL_LList*)0;
    }
    
    APUTIL_Node *cur = lst->head;
    while (cur) {
        if (filterfunc(cur->data)) {
            if (copy) {
                aputil_llist_push_back(new_list, new_list->copydata(cur->data));
            } else {
                aputil_llist_push_back(new_list, cur->data);
            }
        }
        cur = cur->next;
    }

    return new_list;
}


UTIL_ERR aputil_llist_nodeswap(APUTIL_Node *n1, APUTIL_Node *n2) {
    if (!n1 || !n2) return E_EMPTY_ARG;

    void *tmp = n1->data;
    n1->data = n2->data;
    n2->data = tmp;

    return E_SUCCESS;
}


bool aputil_llist_is_sorted(APUTIL_LList *lst, UTIL_ERR *e) {
    if (!lst) {
        *e = E_EMPTY_OBJ;
        return true;
    };
    if (!lst->head) {
        *e = E_NODATA;
        return true;
    }
    if (!lst->compare) {
        *e = E_EMPTY_FUNC;
        return false;
    }
    APUTIL_Node *cur = lst->head;
    while (cur) {
        if (cur->next) {
            if (lst->compare(cur->data, cur->next->data) > 0) return false;
        }
        cur = cur->next;
    }
    return true;
}

