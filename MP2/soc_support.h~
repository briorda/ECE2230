/* soc_support.h
 * Austin Johnson 
 * agjohns
 * ECE 2230 Fall 2016 
 * MP2
 *
 * Propose: A template for MP2 
 *
 * Assumptions: suggested structure definitions and public functions 
 *              as defined for assignmnet.  

 *
 * Bugs: No bugs from the tests I have ran against it. 
 *
 *
 */

/* prototype function definitions */

int soc_compare(const socket_t *soc_a, const socket_t *soc_b);

list_t *soc_create(void);
void soc_cleanup(list_t *);
int soc_number_entries(list_t *);

void soc_sorted_insert(list_t *, socket_t *);
socket_t *soc_sorted_lookup(list_t *, int);
socket_t *soc_sorted_remove(list_t *, int);
socket_t *soc_sorted_update(list_t *, socket_t *);

void soc_get_socket_id(socket_t *);
void soc_fill_record(socket_t *);
void soc_print_socket_rec(socket_t *);

//my functions
void soc_unsorted_insert(list_t *list_ptr, socket_t *socket, int pos_index);
socket_t *soc_access(list_t *list_ptr, int pos_index);
socket_t *soc_unsorted_remove(list_t *list_ptr, int pos_index);
socket_t *soc_unsorted_replace(list_t *list_ptr, int pos_index, socket_t *new_rec_ptr);

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
