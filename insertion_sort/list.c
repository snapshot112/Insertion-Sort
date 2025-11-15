/*
 * Created by snapshot112 on 10/11/2025
 *
 * Naam: Jeroen Boxhoorn
 * UvAnetID: 16333969
 * Studie: BSC Informatica
 *
 * An implementation of the list interface provided in list.h
 * This is a doubly linked list.
 */

#include "list.h"

struct list {
    struct node *head;
    struct node *tail;
};

struct node {
    struct node *next;
    struct node *prev;
    int value;
};

struct list *list_init(void) {
    struct list *list = malloc(sizeof(struct list));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

struct node *list_new_node(const int num) {
    if (num < 0) {
        return NULL;
    }

    struct node *node = malloc(sizeof(struct node));

    if (node == NULL) {
        return NULL;
    }

    node->prev = NULL;
    node->next = NULL;
    node->value = num;

    return node;
}

struct node *list_head(const struct list *l) {
    if (l == NULL) {
        return NULL;
    }
    return l->head;
}

struct node *list_next(const struct node *n) {
    if (n == NULL) {
        return NULL;
    }
    return n->next;
}

int list_add_front(struct list *l, struct node *n) {
    if (n == NULL || l == NULL || list_node_present(l, n) == 1) {
        return 1;
    }

    if (l->head == NULL) {
        l->head = n;
        l->tail = n;
        return 0;
    }

    l->head->prev = n;
    n->next = l->head;
    n->prev = NULL;
    l->head = n;

    return 0;
}

struct node *list_tail(const struct list *l) {
    if (l == NULL) {
        return NULL;
    }
    return l->tail;
}

struct node *list_prev(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL || list_node_present(l, n) != 1) {
        return NULL;
    }

    return n->prev;
}

int list_add_back(struct list *l, struct node *n) {
    if (l == NULL || n == NULL || list_node_present(l, n) == 1) {
        return 1;
    }

    if (l->tail == NULL) {
        l->head = n;
        l->tail = n;
        return 0;
    }

    l->tail->next = n;
    n->prev = l->tail;
    n->next = NULL;
    l->tail = n;

    return 0;
}

int list_node_get_value(const struct node *n) {
    if (n == NULL) {
        return 0;
    }
    return n->value;
}

int list_node_set_value(struct node *n, const int value) {
    if (n == NULL || value < 0) {
        return 1;
    }

    n->value = value;
    return 0;
}

int list_unlink_node(struct list *l, struct node *n) {
    if (l == NULL || n == NULL || list_node_present(l, n) != 1) {
        // No link in the first place.
        return 1;
    }

    // Handle head == tail
    // Handle head
    // Handle tail
    // Handle middle

    if (l->head == l->tail) {
        l->head = NULL;
        l->tail = NULL;
    } else if (n->prev == NULL) {
        l->head = n->next;
        n->next->prev = NULL;
    } else if (n->next == NULL) {
        l->tail = n->prev;
        n->prev->next = NULL;
    } else {
        n->next->prev = n->prev;
        n->prev->next = n->next;
    }

    n->prev = NULL;
    n->next = NULL;

    return 0;
}

void list_free_node(struct node *n) {
    free(n);
}

int list_cleanup(struct list *l) {
    if (l == NULL) {
        return 1;
    }

    if (l->head == NULL) {
        free(l);
        return 0;
    }

    struct node *n = l->head;

    while (n->next != NULL) {
        n = n->next;
        free(n->prev);
    }

    free(n);
    free(l);

    return 0;
}

int list_node_present(const struct list *l, const struct node *n) {
    if (l == NULL || n == NULL) {
        return -1;
    }

    if (l->head == NULL) {
        return 0;
    }

    if (l->head == n) {
        return 1;
    }

    for (const struct node *ln = l->head; ln->next != NULL; ln = ln->next) {
        if (ln->next == n) {
            return 1;
        }
    }

    return 0;
}

int list_insert_after(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL

        || list_node_present(l, m) != 1 || list_node_present(l, n) != 0) {
        return 1;
    }

    // Handle m == tail
    // Handle m == head/middle
    if (m->next == NULL) {
        n->prev = m;
        n->next = NULL;
        m->next = n;
        l->tail = n;
    } else {
        n->next = m->next;
        n->prev = m;
        m->next->prev = n;
        m->next = n;
    }

    return 0;
}

int list_insert_before(struct list *l, struct node *n, struct node *m) {
    if (l == NULL || n == NULL || m == NULL
            || list_node_present(l, m) != 1 || list_node_present(l, n) != 0) {
        return 1;
    }

    // Handle m == head
    // Handle m == tail/middle
    if (m->prev == NULL) {
        n->next = m;
        n->prev = NULL;
        m->prev = n;
        l->head = n;
    } else {
        n->next = m;
        n->prev = m->prev;
        m->prev->next = n;
        m->prev = n;
    }

    return 0;
}

size_t list_length(const struct list *l) {
    if (l == NULL || l->head == NULL) {
        return 0;
    }

    size_t length = 1;

    for (const struct node *n = l->head; n->next != NULL; n = n->next) {
        length++;
    }

    return length;
}

struct node *list_get_ith(const struct list *l, size_t i) {
    if (l == NULL) {
        return NULL;
    }

    struct node *n = l->head;

    while (i > 0 && n != NULL) {
        n = n->next;
        i--;
    }

    return n;
}

struct list *list_cut_after(struct list *l, struct node *n) {
    if (l == NULL || n == NULL || list_node_present(l, n) != 1) {
        return NULL;
    }

    struct list *new_list = malloc(sizeof(struct list));

    if (new_list == NULL) {
        return NULL;
    }

    if (n->next == NULL) {
        new_list->head = NULL;
        new_list->tail = NULL;
        return new_list;
    }

    new_list->head = n->next;
    new_list->head->prev = NULL;
    new_list->tail = new_list->head;
    n->next = NULL;
    l->tail = n;

    for (/* void */; new_list->tail->next != NULL; new_list->tail = new_list->tail->next) {
        /* void */
    }

    return new_list;
}
