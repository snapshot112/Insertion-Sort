#include "list.h"

/*
 * TODO: A lot of code missing here. You will need to add implementations for
 * all the functions described in list.h here.
 *
 * Start by adding the definitions for the list and node structs. You may
 * implement any of the Linked List versions discussed in the lecture, which
 * have some different trade-offs for the functions you will need to write.
 *
 * Note: The function prototypes in list.h assume the most basic Singly Linked
 * List. If you implement some other version, you may not need all of the function
 * arguments for all of the described functions. This will produce a warning,
 * which you can suppress by adding a simple if-statement to check the value
 * of the unused parameter.
 *
 * Also, do not forget to add any required includes at the top of your file.
 */

struct node {
    /* ... SOME CODE MISSING HERE ... */
};

struct list {
    /* ... SOME CODE MISSING HERE ... */
};


struct list *list_init(void) {
    /* ... SOME CODE MISSING HERE ... */
}

struct node *list_new_node(int num) {
    /* ... SOME CODE MISSING HERE ... */
}

struct node *list_head(const struct list *l) {
    /* ... SOME CODE MISSING HERE ... */
}

struct node *list_next(const struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_add_front(struct list *l, struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

struct node *list_tail(const struct list *l) {
    /* ... SOME CODE MISSING HERE ... */
}

struct node *list_prev(const struct list *l, const struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_add_back(struct list *l, struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_node_get_value(const struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_node_set_value(struct node *n, int value) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_unlink_node(struct list *l, struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

void list_free_node(struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_cleanup(struct list *l) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_node_present(const struct list *l, const struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_insert_after(struct list *l, struct node *n, struct node *m) {
    /* ... SOME CODE MISSING HERE ... */
}

int list_insert_before(struct list *l, struct node *n, struct node *m) {
    /* ... SOME CODE MISSING HERE ... */
}

size_t list_length(const struct list *l) {
    /* ... SOME CODE MISSING HERE ... */
}

struct node *list_get_ith(const struct list *l, size_t i) {
    /* ... SOME CODE MISSING HERE ... */
}

struct list *list_cut_after(struct list *l, struct node *n) {
    /* ... SOME CODE MISSING HERE ... */
}
