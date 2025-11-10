#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

// For older versions of the check library
#ifndef ck_assert_ptr_nonnull
#define ck_assert_ptr_nonnull(X) _ck_assert_ptr(X, !=, NULL)
#endif
#ifndef ck_assert_ptr_null
#define ck_assert_ptr_null(X) _ck_assert_ptr(X, ==, NULL);
#endif

#define LEN 10

/* test init & cleanup */
START_TEST(test_list_init) {
    struct list *l = list_init();
    ck_assert_ptr_nonnull(l);
    ck_assert_int_eq(list_cleanup(l), 0);
}
END_TEST

/* test add & head */
START_TEST(test_add_front) {
    struct list *l = list_init();

    struct node *n = list_new_node(5);
    ck_assert_ptr_nonnull(n);
    ck_assert_int_eq(list_add_front(l, n), 0);

    struct node *m = list_head(l);
    ck_assert_ptr_eq(m, n);

    list_cleanup(l);
}
END_TEST

/* test add multiple & next */
START_TEST(test_next_multiple) {
    struct list *l = list_init();
    struct node *n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        ck_assert_int_eq(list_add_front(l, n), 0);
    }

    struct node *m = list_head(l);
    ck_assert_ptr_eq(m, n);

    for (int i = 0; i < LEN - 1; i++) {
        n = list_next(m);
        ck_assert_ptr_nonnull(n);
        ck_assert_ptr_ne(m, n);

        m = n;
    }

    ck_assert_ptr_null(list_next(m));
    list_cleanup(l);
}
END_TEST

/* test append & tail */
START_TEST(test_add_back) {
    struct list *l = list_init();

    struct node *n = list_new_node(5);
    ck_assert_ptr_nonnull(n);
    ck_assert_int_eq(list_add_back(l, n), 0);

    struct node *m = list_tail(l);
    ck_assert_ptr_eq(m, n);

    list_cleanup(l);
}
END_TEST

/* test append multiple & prev */
START_TEST(test_prev_multiple) {
    struct list *l = list_init();
    struct node *n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        ck_assert_int_eq(list_add_back(l, n), 0);
    }

    struct node *m = list_tail(l);
    ck_assert_ptr_eq(m, n);

    for (int i = 0; i < LEN - 1; i++) {
        n = list_prev(l, m);
        ck_assert_ptr_nonnull(n);
        ck_assert_ptr_ne(m, n);

        m = n;
    }

    ck_assert_ptr_null(list_prev(l, m));
    list_cleanup(l);
}
END_TEST

/* test node value */
START_TEST(test_node_value) {
    struct list *l = list_init();
    struct node *n = list_new_node(123);

    ck_assert_int_eq(list_node_get_value(n), 123);
    list_add_front(l, n);
    ck_assert_int_eq(list_node_get_value(list_head(l)), 123);

    list_cleanup(l);
}
END_TEST

/* test value multiple */
START_TEST(test_value_multiple) {
    struct list *l = list_init();
    struct node *n;

    for (int i = LEN - 1; i >= 0; i--) {
        n = list_new_node(i);
        list_add_front(l, n);

        n = list_new_node(i);
        list_add_front(l, n);
    }

    n = list_head(l);
    for (int i = 0; i < LEN; i++) {
        ck_assert_int_eq(list_node_get_value(n), i);
        n = list_next(n);

        ck_assert_int_eq(list_node_get_value(n), i);
        n = list_next(n);
    }

    n = list_tail(l);
    for (int i = LEN - 1; i >= 0; i--) {
        ck_assert_int_eq(list_node_get_value(n), i);
        n = list_prev(l, n);

        ck_assert_int_eq(list_node_get_value(n), i);
        n = list_prev(l, n);
    }

    list_cleanup(l);
}
END_TEST

/* test unlink head */
START_TEST(test_unlink_head) {
    struct list *l = list_init();
    struct node *n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_front(l, n);
    }

    for (int i = 0; i < LEN; i++) {
        n = list_head(l);
        ck_assert_int_eq(list_unlink_node(l, n), 0);
        list_free_node(n);
    }

    ck_assert_ptr_null(list_head(l));
    list_cleanup(l);
}
END_TEST

/* test unlink any */
START_TEST(test_unlink_multiple) {
    struct list *l = list_init();
    struct node *n;
    struct node *m;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_back(l, n);
    }

    n = list_next(list_head(l));
    for (int i = 1; i < LEN; i += 2) {
        m = list_next(list_next(n));
        ck_assert_int_eq(list_unlink_node(l, n), 0);
        ck_assert_int_eq(list_node_get_value(n), i);

        list_free_node(n);
        n = m;
    }

    n = list_head(l);
    for (int i = 0; i < LEN; i += 2) {
        m = list_next(n);
        ck_assert_int_eq(list_unlink_node(l, n), 0);
        ck_assert_int_eq(list_node_get_value(n), i);

        list_free_node(n);
        n = m;
    }

    ck_assert_ptr_null(list_head(l));
    list_cleanup(l);
}
END_TEST

/* test list cleanup */
START_TEST(test_list_cleanup) {
    struct list *l = list_init();
    struct node *n;

    for (int i = 0; i < LEN; i++) {
        n = list_new_node(i);
        list_add_front(l, n);
    }

    ck_assert_int_eq(list_cleanup(l), 0);
}
END_TEST

Suite *list_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("List");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_list_init);
    tcase_add_test(tc_core, test_add_front);
    tcase_add_test(tc_core, test_next_multiple);
    tcase_add_test(tc_core, test_add_back);
    tcase_add_test(tc_core, test_prev_multiple);
    tcase_add_test(tc_core, test_node_value);
    tcase_add_test(tc_core, test_value_multiple);
    tcase_add_test(tc_core, test_unlink_head);
    tcase_add_test(tc_core, test_unlink_multiple);
    tcase_add_test(tc_core, test_list_cleanup);

    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s = list_suite();
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);

    srunner_free(sr);
    return number_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
