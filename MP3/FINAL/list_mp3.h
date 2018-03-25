/* list.h 
 *
 * Public functions for two-way linked list
 *
 * You should not need to change anything in this file.  If you do you
 * must get permission from the instructor.
 */

#define LISTPOS_HEAD -98765
#define LISTPOS_TAIL -23456

typedef struct list_node_tag {
    // private members for list.c only
    data_t *data_ptr;
    struct list_node_tag *ll_prev;
    struct list_node_tag *ll_next;
} list_node_t;

typedef struct list_tag {
    // private members for list.c only
    list_node_t *ll_head;
    list_node_t *ll_tail;
    int ll_entries;
    int ll_sorted_state;
    // Private method for list.c only
    int (*comp_proc)(const data_t *, const data_t *);
    int (*qsort_comp)(const void *p_a, const void *p_b);
} list_t;

/* prototype definitions for functions in list.c */
data_t * list_access(list_t *list_ptr, int pos_index);
list_t * list_construct(int (*fcomp)(const data_t *, const data_t *),
        int (*qcomp)(const void *, const void *));
void     list_destruct(list_t *list_ptr);
data_t * list_elem_find(list_t *list_ptr, data_t *elem_ptr, int *pos_index);
int      list_entries(list_t *list_ptr);
void     list_insert(list_t *list_ptr, data_t *elem_ptr, int pos_index);
void     list_insert_sorted(list_t *list_ptr, data_t *elem_ptr);
data_t * list_remove(list_t *list_ptr, int pos_index);
void insertion_sort (list_t *list_ptr);
data_t *FindMin (list_t *list_ptr, list_node_t *curr_node, list_node_t *tail);
void recursive_sort (list_t *list_ptr, list_node_t *curr_node, list_node_t *tail);
void iterative_sort (list_t *list_ptr, list_node_t *curr_node, list_node_t *tail);
void split_list (list_t *list_ptr, list_t **front_half, list_t **back_half);
void merge_two_lists (list_t **merged_list, list_t *L, list_t *R);
void merge_sort (list_t **list_ptr);
void quick_sort (list_t **list_ptr);
void list_sort(list_t *list_ptr, int sort_type);

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
