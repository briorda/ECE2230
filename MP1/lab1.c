/* lab1.c
 * Austin Johnson 
 * agjohns
 * ECE 2230 Fall 2016
 * MP1
 *
 *
 * 
 *
 * Purpose: The purpose of this program is to be able to organize information in a 
 *          resizable vector. The user should be able to store, access, and print 
 *          information about socket records. 
 *
 * Assumptions: I assume this program will work with MINSOCKETS set to 4. However, 
 *              it should work from 2 to several thousand. I also assume it will
 *              work as long as the user inputs the correct commands: insert x, 
 *              replace x, pushback, erase x, popback, at x, front, back, print, 
 *              stats or quit. 
 *
 *
 * Bugs: No bugs from the tests that I have ran against it. 
 *
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "socketlist.h"

#define MAXLINE 256

// function prototypes for lab1.c
void fill_socket_record(struct socket_info_t *new);
void print_socket_rec(struct socket_info_t *rec);

int main(int argc, char *argv[])
{
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int num_items;
    int list_position;
    int i;
    struct socketlist_t *soc_ptr = NULL;  // Pointer to the list
    struct socket_info_t *rec_ptr = NULL; // Pointer to socket info

    if (argc != 1) {
        printf("Usage: ./lab1 \n");
        exit(1);
    }
    printf("Welcome to lab1.\n");

    //construct structure
    soc_ptr = socketlist_construct();

    // remember fgets includes newline \n unless line too long 
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s%d%s", command, &list_position, junk);
        if (num_items == 2 && strcmp(command, "insert") == 0) {

            rec_ptr = (struct socket_info_t *) malloc(sizeof(struct socket_info_t));
            fill_socket_record(rec_ptr);

            int added_return = 0;

            added_return = socketlist_add(soc_ptr, list_position, rec_ptr);

            if (added_return == 0) {
                printf("Inserted: %d into position %d\n", 
                        rec_ptr->socket_id, list_position);
            } else if (added_return == -1) {
                printf("Rejected: %d invalid position %d\n", 
                        rec_ptr->socket_id, list_position);
                        free(rec_ptr);
            } else {
                printf("Error with return value!  You code must be fixed\n");
            }
            rec_ptr = NULL;
        }
 
        else if (num_items == 2 && strcmp(command, "replace") == 0) {
            rec_ptr = (struct socket_info_t *) malloc(sizeof(struct socket_info_t));
            fill_socket_record(rec_ptr);

            struct socket_info_t *old_rec_ptr = NULL;

            //get socket_id of old record
            old_rec_ptr = socketlist_replace(soc_ptr, list_position, rec_ptr);

            if (old_rec_ptr == NULL) {
                printf("Did not replace: %d invalid position %d\n", 
                        rec_ptr->socket_id, list_position);
                        free(rec_ptr);
            } else {
                printf("Replaced: %d with %d into position %d\n", 
                        old_rec_ptr->socket_id, rec_ptr->socket_id, 
                        list_position);
            }

            free(old_rec_ptr);
            rec_ptr = old_rec_ptr = NULL;
        } 

        else if (num_items == 1 && strcmp(command, "pushback") == 0) {

            rec_ptr = (struct socket_info_t *) malloc(sizeof(struct socket_info_t));

            fill_socket_record(rec_ptr);

            socketlist_add(soc_ptr, socketlist_number_entries(soc_ptr), rec_ptr);

            printf("Pushed: %d into position %d\n", rec_ptr->socket_id, 
                    socketlist_number_entries(soc_ptr) - 1); 
            rec_ptr = NULL;
        } 
    
        else if (num_items == 2 && strcmp(command, "erase") == 0) {
            
            rec_ptr = socketlist_remove(soc_ptr, list_position);

            if (rec_ptr == NULL) {
                printf("Did not remove: invalid position %d\n", list_position);
            } else {
                printf("Erased: %d from position %d\n", rec_ptr->socket_id, 
                        list_position);
            }
            free(rec_ptr);
            rec_ptr = NULL;
        } 
        
        else if (num_items == 1 && strcmp(command, "popback") == 0) {
            
            rec_ptr = socketlist_remove(soc_ptr, socketlist_number_entries(soc_ptr) - 1);

            if (rec_ptr == NULL) {
                printf("Did not pop: list is empty\n");
            } else {
                printf("Popped: %d from position %d\n", rec_ptr->socket_id, 
                    socketlist_number_entries(soc_ptr));
            }
            free(rec_ptr);
            rec_ptr = NULL;
        } 

        else if (num_items == 2 && strcmp(command, "at") == 0) {

            //access socket info corresponding to position
            rec_ptr = socketlist_access(soc_ptr, list_position);

            if (rec_ptr == NULL) {
                printf("Did not print: invalid position %d\n", list_position);
            } else {
                printf("Record at position: %d\n", list_position);
                print_socket_rec(rec_ptr);
            }
            rec_ptr = NULL;
        } 

        else if (num_items == 1 && strcmp(command, "front") == 0) {

            rec_ptr = socketlist_access(soc_ptr, 0);

            if (rec_ptr == NULL) {
                printf("Did not print front: list is empty\n");
            } else {
                printf("Record at front:\n");
                print_socket_rec(rec_ptr);
            }
            rec_ptr = NULL;
        } 

        else if (num_items == 1 && strcmp(command, "back") == 0) {

            rec_ptr = socketlist_access(soc_ptr, socketlist_number_entries(soc_ptr) - 1);

            if (rec_ptr == NULL) {
                printf("Did not print back: list is empty\n");
            } else {
                printf("Record at back:\n");
                print_socket_rec(rec_ptr);
            }
            rec_ptr = NULL;
        } 

        else if (num_items == 1 && strcmp(command, "print") == 0) {
            int num_in_list = socketlist_number_entries(soc_ptr);
            if (num_in_list == 0) {
                printf("List empty\n");
            } else {
                printf("%d records\n", num_in_list);
                for (i = 0; i < num_in_list; i++) {
                    printf("%4d: ", i);

                    //access socket info corresponding to position
                    rec_ptr = socketlist_access(soc_ptr, i);   
                    print_socket_rec(rec_ptr);
                }
            }
        } 

        else if (num_items == 1 && strcmp(command, "stats") == 0) {
            // get the number in list and size of the list
            int num_in_list = socketlist_number_entries(soc_ptr);
            int size_of_list = socketlist_capacity(soc_ptr);
            printf("Number records: %d, Current capacity: %d\n", 
                    num_in_list, size_of_list);
        } 

        else if (num_items == 1 && strcmp(command, "quit") == 0) {

            //free allocated space
            socketlist_destruct(soc_ptr);

            printf("Goodbye\n");
            break;

        } 

        else {
            printf("# %s", line);
        }
    }

    exit(0);
}

/* Prompts user for record input starting with the Socket ID.
 * The input is not checked for errors but will default to an acceptable value
 * if the input is incorrect or missing.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void fill_socket_record(struct socket_info_t *new) {
    char line[MAXLINE];
    assert(new != NULL);

    printf("Socket ID:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->socket_id);


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
    printf("protocol (TCP|UDP|SSL|RTP):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->protocol);
    printf("address family (IPv4|IPv6):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->address_family);
    printf("state (EST|CLOSE_WAIT|CLOSED):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->state);
    printf("data rate:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%f", &new->data_rate);
    printf("Time received (int):");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%d", &new->time_received);
    printf("\n");
}

/* print the information for a particular record 
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 */
void print_socket_rec(struct socket_info_t *socket) {
    assert(socket != NULL);
    printf("Socket id: %d", socket->socket_id);
    printf(" Dest: %d, Src: %d, Dport: %d,", socket->dest_ip_addr, 
            socket->src_ip_addr, socket->dest_port_num);
    printf(" Src: %d,", socket->src_port_num); 
    printf(" Prot: %d, Fam: %d, St: %d", socket->protocol, 
            socket->address_family, socket->state);
    printf(" Rt: %g\n", socket->data_rate);
    printf(" Tm: %d\n", socket->time_received);
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
