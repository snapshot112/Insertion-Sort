/*
 * Created by snapshot112 on 13/11/2025
 *
 * Naam: Jeroen Boxhoorn
 * UvAnetID: 16333969
 * Studie: BSC Informatica
 *
 * Insertion sort implementation, fulfilling the following requirements:
 * - reads zero or more numbers from STDIN
 * - reads fully buffered
 * - Prints same numbers in STDOUT
 *   > Prints 1 number per line
 *   > Prints in sorted order
 * - Must use linked lists (doubly or singly) -> Doubly in this case.
 * - Must use a version of insertion sort.
 * - Must end output with a newline.
 *
 * Number specifics:
 * - Non-negative
 * - Separated by *ANY* whitespace, including newlines
 * - Only integers
 *
 * Supporting 4 flags, applied in the following order:
 * 0. Sort
 * 1. -d Start printing at high instead of low.
 * 2. -c Add every successive 2 items together when printing
 *    (no successor = just print that number solo)
 * 3. -o removes all odd numbers from the sorted list
 * 4. -z split and zip sorted list
 *    > Sort first, split sorted list in 2 halves
 *      (uneven means the lower numbers go in the bigger half)
 *    > Then zip items together, first the lowest number in the half with the lower numbers,
 *      then the lowest number in the half with the bigger numbers.
 */

#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#define BUF_SIZE 1024

char buf[BUF_SIZE];

struct config {
    /* You can ignore these options until you implement the
       extra command-line arguments. */

    /* Set to 1 if -d is specified, 0 otherwise. */
    int descending_order;

    // Set to 1 if -c is specified, 0 otherwise.
    int combine;

    // Set to 1 if -o is specified, 0 otherwise.
    int remove_odd;

    /* Set to 1 if -z is specified, 0 otherwise. */
    int zip_alternating;
};

/*
 * Takes the flags out of the STDIN and stores them in the cfg.
 */
int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "dcoz")) != -1) {
        switch (c) {
        case 'd':
            cfg->descending_order = 1;
            break;
        case 'c':
            cfg->combine = 1;
            break;
        case 'o':
            cfg->remove_odd = 1;
            break;
        case 'z':
            cfg->zip_alternating = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}

/*
 * Parses incoming integers from stdin and adds them to a linked list.
 *
 * Input:
 * list: The linked list to store the parsed numbers in.
 *
 * Returns:
 * 0 on success.
 * -1 on error.
 *
 * Side Effects:
 * Reads from stdin and writes to the given list.
 */
int parse_input(struct list *list) {
    if (list == NULL) {
        return -1;
    }

    int number_size = 0;
    int current_number = 0;

    for (int c = getchar(); c != EOF; c = getchar()) {
        if ('0' <= c && c <= '9') {
            if (number_size > 5) {
                // Integer can store a max of 5 decimal numbers.
                return -1;
            }

            current_number = current_number * 10 + (c - '0');

            number_size++;
        } else if (c == ' ' || c == '\n') {
            if (number_size == 0) {
                continue;
            }

            struct node *new_node = list_new_node(current_number);

            if (new_node == NULL) {
                return -1;
            }

            if (list_add_back(list, new_node) != 0) {
                return -1;
            }
            current_number = 0;
            number_size = 0;
        } else {
            return -1;
        }
    }

    if (number_size > 5) {
        // Integer can store a max of 5 decimal numbers.
        return -1;
    }

    if (number_size == 0) {
        return 0;
    }

    if (list_add_back(list, list_new_node(current_number)) != 0) {
        return -1;
    }

    return 0;
}

/*
 * Sort a node in a list towards the front of the list.
 *
 * Input:
 * list: The list containing the node.
 * node: The node you want to sort.
 *
 * Returns:
 * 1 if the node was already sorted.
 * 0 if the node needed to be moved.
 * -1 on failure.
 *
 * Side Effects:
 * The node gets moved through the list.
 */
static int sort_node(struct list *list, struct node *node) {
    if (list == NULL || node == NULL) {
        return -1;
    }

    const int value = list_node_get_value(node);
    struct node *prev = list_prev(list, node);


    // If the node is already on the right spot.
    if (prev == NULL || list_node_get_value(prev) <= value) {
        return 1;
    }

    // Else, unlink and find the right spot
    list_unlink_node(list, node);

    while (prev != NULL && list_node_get_value(prev) > value) {
        prev = list_prev(list, prev);
    }

    // If no prev node with a lower or equal value is found, add it to the front instead.
    if (prev == NULL) {
        if (list_add_front(list, node) != 0) {
            return -1;
        }
        return 0;
    }

    // Add it back on the sorted spot.
    if (list_insert_after(list, node, prev) != 0) {
        return -1;
    }
    return 0;
}

/*
 * Sorts a linked list in ascending order.
 *
 * Input:
 * list: Pointer to the list to be sorted.
 *
 * output:
 * 0 on success.
 * -1 on failure.
 *
 * Side Effects:
 * The nodes in the list will be rearranged to have their values in ascending order.
 */
static int sort_list(struct list *list) {
    if (list == NULL) {
        return -1;
    }

    struct node *node = list_head(list);

    if (node == NULL) {
        return 0;
    }

    for (
        struct node *node_to_sort = list_next(node);
        node_to_sort != NULL;
        node_to_sort = list_next(node)
    ) {
        const int status = sort_node(list, node_to_sort);

        if (status == -1) {
            return -1;
        }
        if (status == 1) {
            node = list_next(node);
        }
    }

    return 0;
}

/*
 * Flips the contents of a list. (last item will be the first one and in reverse and so on...)
 *
 * Input:
 * list: The list to flip.
 *
 * Output:
 * 0 on success.
 * -1 on failure.
 *
 * Side-Effects:
 * The contents of the list will be in reverse order from before this function.
 */
static int flip_list(struct list *list) {
    if (list == NULL) {
        return -1;
    }

    struct node * const tail = list_tail(list);
    struct node *prev = list_prev(list, tail);

    while (prev != NULL) {
        if (list_unlink_node(list, prev) != 0) {
            return -1;
        }

        if (list_add_back(list, prev) != 0) {
            return -1;
        }

        prev = list_prev(list, tail);
    }

    return 0;
}

/*
 * Combine the values of each consecutive node in a list.
 *
 * Input:
 * list: The list whose nodes you want to combine.
 *
 * Output:
 * 0 on success
 * -1 on failure.
 *
 * Side Effects:
 * Each pair of consecutive nodes gets squashed together into 1 node with their combined value.
 * If no complete pair is left at the end, the last node is left unchanged.
 */
static int combine_nodes(struct list *list) {
    if (list == NULL) {
        return -1;
    }

    struct node *node = list_head(list);

    while (node != NULL) {
        struct node *next_node = list_next(node);

        if (next_node == NULL) {
            return 0;
        }

        const int new_value = list_node_get_value(node) + list_node_get_value(next_node);

        if (list_node_set_value(node, new_value) != 0) {
            return -1;
        }

        if (list_unlink_node(list, next_node) != 0) {
            return -1;
        }

        list_free_node(next_node);
        node = list_next(node);
    }

    return 0;
}

/*
 * Remove odd valued nodes from a list.
 *
 * Input:
 * list: The list to remove the odd valued nodes from.
 *
 * Output:
 * 0 on success.
 * -1 on failure.
 *
 * Side Effects:
 * All the odd valued nodes get removed from the list.
 * Removed nodes will get freed.
 */
static int remove_odd_valued_nodes(struct list *list) {
    if (list == NULL) {
        return -1;
    }

    struct node *node = list_head(list);

    if (node == NULL) {
        // No nodes to remove so done.
        return 0;
    }

    // Make sure that the first node doesn't have to get removed anymore.
    while (node != NULL && list_node_get_value(node) % 2 != 0) {
        if (list_unlink_node(list, node) != 0) {
            return -1;
        };
        list_free_node(node);
        node = list_head(list);
    }

    // The list might have only contained odd valued nodes.
    if (node == NULL) {
        return 0;
    }

    // Then iterate through the list whilst removing odd nodes.
    for (struct node *next = list_next(node); next != NULL; next = list_next(node)) {
        if (list_node_get_value(next) % 2 != 0) {
            if (list_unlink_node(list, next) != 0) {
                return -1;
            }
            list_free_node(next);
        } else {
            node = next;
        }
    }

    return 0;
}

/*
 * Split a list in 2 (in case of an uneven amount of nodes, the first half is bigger)
 * and zip the 2 halves back together, with the first item in the zipped list being the first item
 * from the first list.
 *
 * Input:
 * List: The list to split and zip.
 *
 * Output:
 * 0 on success.
 * -1 on failure.
 *
 * Side Effects:
 * The list gets split into 2 halves, and then the 2 halves get zipped back together.
 */
static int split_and_zip_list(struct list *list) {
    if (list == NULL) {
        return -1;
    }

    size_t list_size = list_length(list);
    struct node *split_after = list_get_ith(list, list_size / 2 + list_size % 2 - 1);

    struct list *second_list = list_cut_after(list, split_after);

    if (second_list == NULL) {
        return -1;
    }

    struct node *node = list_head(list);
    struct node *node_to_zip = list_head(second_list);

    while (node_to_zip != NULL) {
        if (list_unlink_node(second_list, node_to_zip) != 0) {
            return -1;
        }

        if (list_insert_after(list, node_to_zip, node) != 0) {
            return -1;
        }

        node = list_next(node_to_zip);
        node_to_zip = list_head(second_list);
    }

    free(second_list);

    return 0;
}

/*
 * Prints all the values in a list to stdout, line separated.
 *
 * Input:
 * list: The list whose values you want to print.
 *
 * Side Effects:
 * Lines with numbers get printed to stdout.
 */
static void print_list(const struct list *list) {
    for (const struct node *node = list_head(list); node != NULL; node = list_next(node)) {
        printf("%d\n", list_node_get_value(node));
    }
}

/*
 * Insertion sort numbers inputted using stdin.
 *
 * Sorted numbers will be printed line separated in stdout.
 *
 * Sorting program requirements:
 * - reads zero or more numbers from STDIN
 * - reads fully buffered
 * - Prints same numbers in STDOUT
 *   > Prints 1 number per line
 *   > Prints in sorted order
 * - Must use linked lists (doubly or singly)
 * - Must use a version of insertion sort.
 * - Must end output with a newline.
 *
 * Number specifics:
 * - Non-negative
 * - Separated by *ANY* whitespace, including newlines
 * - Only integers
 */
int main(int argc, char *argv[]) {
    struct config cfg;
    if (parse_options(&cfg, argc, argv) != 0) {
        return 1;
    }

    struct list *list = list_init();

    if (list == NULL || parse_input(list) != 0) {
        list_cleanup(list);
        return EXIT_FAILURE;
    }

    // Sort the list.
    if (sort_list(list) != 0) {
        // list_cleanup(list);
        // return EXIT_FAILURE;
    }

    // Sorted here, start handling flags.

    if (cfg.descending_order) {
        // Flip the list.
        if (flip_list(list) != 0) {
            list_cleanup(list);
            return EXIT_FAILURE;
        }
    }

    if (cfg.combine) {
        if (combine_nodes(list) != 0) {
            list_cleanup(list);
            return EXIT_FAILURE;
        }
    }

    if (cfg.remove_odd) {
        if (remove_odd_valued_nodes(list) != 0) {
            list_cleanup(list);
            return EXIT_FAILURE;
        }
    }

    if (cfg.zip_alternating) {
        if (split_and_zip_list(list) != 0) {
            list_cleanup(list);
            return EXIT_FAILURE;
        }
    }

    // Done with the flags, print the results.
    print_list(list);

    list_cleanup(list);

    return EXIT_SUCCESS;
}
