#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "list.h"

// For older versions of the check library
#ifndef ck_assert_ptr_nonnull
#define ck_assert_ptr_nonnull(X) _ck_assert_ptr(X, !=,NULL)
#endif
#ifndef ck_assert_ptr_null
#define ck_assert_ptr_null(X) _ck_assert_ptr(X, ==,NULL);
#endif

#define LEN 10

START_TEST (test_node_present)
{
    struct list* l = list_init();
    struct node* n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_back(l, n);
    }

    ck_assert_int_eq(list_node_present(l, n), 1);

    n = list_head(l);
    ck_assert_int_eq(list_node_present(l, n), 1);

    n  = list_new_node(0);
    ck_assert_int_eq(list_node_present(l, n), 0);
    list_free_node(n);

    n  = list_new_node(123);
    ck_assert_int_eq(list_node_present(l, n), 0);
    list_free_node(n);

    list_cleanup(l);
}
END_TEST

START_TEST (test_insert_after)
{
    struct list* l = list_init();
    struct node* n;
    struct node* m;
    struct node* k;

    for (int i = 0; i < LEN; i+=2) {
        n = list_new_node(i);
        list_add_back(l, n);
    }

    n = list_head(l);
    for (int i = 1; i < LEN; i+=2) {
        m = list_next(n);
        k = list_new_node(i);

        list_insert_after(l, k, n);
        n = m;
    }

    n = list_head(l);
    for (int i = 0; i < LEN; i++) {
        ck_assert_int_eq(list_node_get_value(n), i);
        n = list_next(n);
    }

    list_cleanup(l);
}
END_TEST

START_TEST (test_insert_before)
{
    struct list* l = list_init();
    struct node* n;
    struct node* m;
    struct node* k;

    for (int i = 1; i < LEN; i+=2) {
        n = list_new_node(i);
        list_add_back(l, n);
    }

    n = list_head(l);
    for (int i = 0; i < LEN; i+=2) {
        m = list_next(n);
        k = list_new_node(i);

        list_insert_before(l, k, n);
        n = m;
    }

    n = list_head(l);
    for (int i = 0; i < LEN; i++) {
        ck_assert_int_eq(list_node_get_value(n), i);
        n = list_next(n);
    }

    list_cleanup(l);
}
END_TEST

START_TEST (test_list_length)
{
    struct list* l = list_init();
    struct node* n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_back(l, n);
    }

    ck_assert(list_length(l) == LEN);

    n = list_head(l);
    list_unlink_node(l, n);
    list_free_node(n);

    n = list_head(l);
    list_unlink_node(l, n);
    list_free_node(n);

    ck_assert(list_length(l) == LEN-2);

    list_cleanup(l);
}
END_TEST

START_TEST (test_ith_node)
{
    struct list* l = list_init();
    struct node* n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_back(l, n);
    }

    for (unsigned int i = 0; i < LEN; i++) {
        n = list_get_ith(l, i);
        ck_assert_int_eq(list_node_get_value(n), i);
    }

    list_cleanup(l);
}
END_TEST

START_TEST (test_cut_list)
{
    struct list* l = list_init();
    struct node* n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_back(l, n);
    }

    unsigned int j = LEN/2;
    n = list_get_ith(l, j - 1);

    struct list* k = list_cut_after(l, n);

    n = list_head(l);
    for (unsigned int i = 0; i < j; i++) {
        ck_assert_int_eq(list_node_get_value(n), i);
        n = list_next(n);
    }
    ck_assert_ptr_null(n);

    n = list_head(k);
    for (unsigned int i = j; i < LEN; i++) {
        ck_assert_int_eq(list_node_get_value(n), i);
        n = list_next(n);
    }
    ck_assert_ptr_null(n);

    list_cleanup(l);
    list_cleanup(k);
}
END_TEST

// Harder tests

START_TEST (test_prev_fail)
{
    struct list* l = list_init();
    struct node* n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_front(l, n);
    }

    n = list_new_node(0);
    ck_assert_ptr_null(list_prev(l, n));
    list_free_node(n);

    list_cleanup(l);
}
END_TEST

START_TEST (test_unlink_twice)
{
    struct list* l = list_init();
    struct node* n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_front(l, n);
    }

    for (int i = 0; i < LEN; i++) {
        n = list_head(l);

        ck_assert_int_eq(list_unlink_node(l, n), 0);
        ck_assert_int_eq(list_unlink_node(l, n), 1);

        list_free_node(n);
    }

    list_cleanup(l);
}
END_TEST

START_TEST (test_unlink_next)
{
    struct list* l = list_init();
    struct node* n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_front(l, n);
    }

    n = list_next(list_head(l));
    list_unlink_node(l, n);
    ck_assert_ptr_null(list_next(n));
    ck_assert_ptr_null(list_prev(l, n));

    list_free_node(n);
    list_cleanup(l);
}
END_TEST

START_TEST (test_unlink_null)
{
    struct list* l = list_init();
    struct node* n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_front(l, n);
    }

    ck_assert_int_eq(list_unlink_node(NULL, n), 1);
    ck_assert_int_eq(list_unlink_node(l, NULL), 1);

    list_cleanup(l);
}
END_TEST

START_TEST (test_insert_null)
{
    struct list* l = list_init();
    struct node* n = list_new_node(5);

    ck_assert_int_eq(list_add_front(NULL, n), 1);
    ck_assert_int_eq(list_add_front(l, NULL), 1);

    list_free_node(n);
    list_cleanup(l);
}
END_TEST

START_TEST (test_insert_invalid)
{
    struct list* l = list_init();
    struct node* n;
    struct node* m;
    struct node* k;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_back(l, n);
    }

    n = list_head(l);
    while (n != NULL) {
        m = list_next(n);
        ck_assert_int_eq(list_insert_after(l, m, n), 1);

        k = list_new_node(3);
        n = list_new_node(5);
        ck_assert_int_eq(list_insert_after(l, k, n), 1);

        list_free_node(k);
        list_free_node(n);

        n = m;
    }

    list_cleanup(l);
}
END_TEST

Suite * list_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("List");
    /* Core test case */
    tc_core = tcase_create("Core");

	/* Extra tests. */
    tcase_add_test(tc_core, test_node_present);
    tcase_add_test(tc_core, test_insert_after);
    tcase_add_test(tc_core, test_insert_before);
    tcase_add_test(tc_core, test_list_length);
    tcase_add_test(tc_core, test_ith_node);
    tcase_add_test(tc_core, test_cut_list);

    tcase_add_test(tc_core, test_prev_fail);
    tcase_add_test(tc_core, test_unlink_twice);
    tcase_add_test(tc_core, test_unlink_next);
    tcase_add_test(tc_core, test_unlink_null);
    tcase_add_test(tc_core, test_insert_null);
    tcase_add_test(tc_core, test_insert_invalid);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    Suite *s = list_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    srunner_free(sr);
    return 0;

}
