/* soc_support.c
 * Austin Johnson 
 * agjohns
 * ECE 2230 Fall 2016 
 * MP2
 *
 * Purpose: The purpose of this file is to provide functions for the lab2.c file. The 
 *          reason it is separate from the main file is for information hiding and to 
 *          allow the programmer to use different data structures with the lab2.c file 
 *          without changing it.  
 *
 * Assumptions: Suggested structure definitions and public functions 
 *              as defined for assignmnet.  
 *
 *              
 *
 * Bugs: No bugs from the tests that I have ran against it. 
 *
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "datatypes.h"
#include "list.h"
#include "soc_support.h"

#define MAXLINE 256

/* soc_compare is required by the list ADT for sorted lists. 
 *
 * This function returns 
 *     1 if rec_a should be closer to the head than rec_b,
 *    -1 if rec_b is to be considered closer to the head, and 
 *     0 if the records are equal.
 *
 * For the socket data we want to sort from lowest ID up, so
 * closer to the front means a smaller ID.
 *
 * The function expects pointers to two record structures, and it is an error
 * if either is NULL
 *
 * THIS FUNCTION SHOULD NOT BE CHANGED
 */
int soc_compare(const socket_t *soc_a, const socket_t *soc_b)
{
    assert(soc_a != NULL && soc_b != NULL);
    if (soc_a->socket_id < soc_b->socket_id)
	return 1;
    else if (soc_a->socket_id > soc_b->socket_id) 
	return -1;
    else
	return 0;
}

/* This creates a list and it can be either a sorted or unsorted list
 *
 * This function provides an example of how to use function pointers.  
 */
list_t *soc_create(void) {
    return list_construct(soc_compare); 
}

/* this function frees the memory for either a sorted or unsorted list.
 */
void soc_cleanup(list_t *list_ptr) {
    list_destruct(list_ptr);
} 

/* Return the number of items in the list. It can be either 
 * a sorted or unsorted list.
 */
int soc_number_entries(list_t *list_ptr) {

    int entries = 0;
    entries = list_entries(list_ptr); 
    return entries;
}

/* This function adds the structure to the list in sorted order.
 * If the new structure has the same socket_id as an
 * element in the list, then the new structure is placed after 
 * all sockets with the same id.
 */
void soc_sorted_insert(list_t *list_ptr, socket_t *socket) {
    list_insert_sorted(list_ptr, socket); 
}

/* This function adds the structure to the list in unsorted order.
 *
 */
void soc_unsorted_insert(list_t *list_ptr, socket_t *socket, int pos_index) {
    list_insert(list_ptr, socket, pos_index);
}

/* This function looks for the structure at the given position in the 
 * unsorted list.  Or, null is returned if the element is not found 
 * in the list.
 */
socket_t *soc_access(list_t *list_ptr, int pos_index) {
    socket_t *access_ptr = NULL;
    access_ptr = list_access(list_ptr, pos_index); 
    return access_ptr;
}

/* This function looks for the first structure with the matching socket_id 
 * in the sorted list.  Or, null is returned if the element is not found 
 * in the list.
 */
socket_t *soc_sorted_lookup(list_t *list_ptr, int id) {

    int position = -9876;
    data_t template;
    template.socket_id = id;
    data_t *foundp = list_elem_find(list_ptr, &template, &position); 
    return foundp;
}

/* This function removes the first structure from the sorted list with the 
 * matching socket_id
 */
socket_t *soc_sorted_remove(list_t *list_ptr, int id) {

    int position = -9876;
    data_t template;
    template.socket_id = id;
    data_t *foundp = list_elem_find(list_ptr, &template, &position);

    if (foundp == NULL) 
        return NULL;
    else {
        foundp = list_remove(list_ptr, position);
        return foundp;
    }
}

/* This function removes the structure from the unsorted list with the 
 * given position
 */
socket_t *soc_unsorted_remove(list_t *list_ptr, int pos_index) {

    if (pos_index < 0 || pos_index > soc_number_entries(list_ptr) - 1)
        return NULL;

    else {
        socket_t *old_rec_ptr = NULL;
        old_rec_ptr = list_remove(list_ptr, pos_index); 
        return old_rec_ptr;
    }
}

/* This function replaces the structure from the unsorted list with the 
 * matching position. 
 */
socket_t *soc_unsorted_replace(list_t *list_ptr, int pos_index, socket_t *new_rec_ptr) {

    if (pos_index < 0 || pos_index > soc_number_entries(list_ptr) - 1) 
        return NULL;

    else { 
        socket_t *old_rec_ptr = NULL;
        old_rec_ptr = soc_unsorted_remove(list_ptr, pos_index);
        soc_unsorted_insert(list_ptr, new_rec_ptr, pos_index);
        return old_rec_ptr;   
    }
}


/* This function replaces the structure from the sorted list with the matching
 * socket_id.  If there are multiple records with the same socket_id, 
 * the first record is the one updated with the new information.
 */
socket_t *soc_sorted_update(list_t *list_ptr, socket_t *socket) {
    socket_t *old_rec_ptr = NULL, temp;
    old_rec_ptr = soc_sorted_lookup(list_ptr, socket->socket_id);

    if (old_rec_ptr == NULL) 
        return NULL;

    else {
        temp = *old_rec_ptr; 
        *old_rec_ptr = *socket;
        *socket = temp;
        return socket;    
    } 
}

/* Prompts user for socket ID and then calls another function
 * to fill in the remaining details of the record.
 *
 * The input to the function assumes that the structure has already been
 * created.  
 *
 * There is no output.
 */
void soc_get_socket_id(socket_t *new)
{
    char line[MAXLINE];
    assert(new != NULL);

    printf("Socket ID:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->socket_id);

    soc_fill_record(new);
}

/* Prompts user for record input starting with the Socket ID.
 * The input is not checked for errors but will default to an acceptable value
 * if the input is incorrect or missing.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 */
void soc_fill_record(socket_t *new)
{
    char line[MAXLINE];
    assert(new != NULL);

    printf("Dest IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->dest_ip_addr);
    printf("Source IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->src_ip_addr);
    printf("Dest port number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->dest_port_num);
    printf("Source port number:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->src_port_num);
    printf("Protocol (TCP|UDP|SSL|RTP):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->protocol);
    printf("Address family (IPv4|IPv6):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->address_family);
    printf("State (EST|CLOSE_WAIT|CLOSED):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->state);
    printf("Data rate:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%f", &new->data_rate);
    printf("Time received (int):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->time_received);
    printf("\n");
}

/* print the information for a particular Socket record 
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 */
void soc_print_socket_rec(socket_t *socket)
{
    assert(socket != NULL);
    printf("Socket id: %d", socket->socket_id);
    printf(" Dest: %d, Src: %d, Dport: %d,", socket->dest_ip_addr, 
            socket->src_ip_addr, socket->dest_port_num);
    printf(" Src: %d,", socket->src_port_num); 
    printf(" Prot: %d, Fam: %d, St: %d", socket->protocol, 
            socket->address_family, socket->state);
    printf(" Rt: %g", socket->data_rate);
    printf(" Tm: %d\n", socket->time_received);
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
