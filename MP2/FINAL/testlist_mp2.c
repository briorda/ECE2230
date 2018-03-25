/* 

Writing and debugging MP2 is challenging, and it is particularly hard if you
try and write all the code before you do any testing. It is much easier to
break the problem into small pieces and test each piece.

For MP2, one approach is to write list_insert first, since it might be the
easiest algorithm and it depends only on list_construct.

   For testing, we use a simple main function in this file.  This file is for
   testing only, and you will NOT submit it as it is for your personal testing
   during development.

Below are a few simple examples that allocates a few structures and inserts 
them into a list.

With this file, you can compile and run list.c on its own using 

   gcc -Wall -g testlist.c list.c soc_support.c -o testlist

Or, I have added the compiling commands to the makefile, so do
   make testlist

Do this testing before you make ANY changes to soc_support.c.  The only 
reason we need to compile soc_support.c is for the soc_compare function.
The linker needs to find the comp_proc function and it is found in the template
   soc_support.c file (with no changes) 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "datatypes.h"
#include "list.h"
#include "soc_support.h"

/* prototype for list debug print */
void list_debug_print(list_t *list_ptr);

int main(void)
{
    data_t *datap = NULL;
    list_t *Lptr = NULL;

    Lptr = list_construct(soc_compare);

    // create one item to test list_insert
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->socket_id = 555;
    list_insert(Lptr, datap, 0);
    datap = NULL;

    // test list_access with one item in list
    datap = list_access(Lptr, LISTPOS_HEAD);
    if (datap != NULL) {
        printf("should find 555 and found (%d)\n", datap->socket_id);
    } else {
        printf("ERROR: list access is broken?\n");
    }
    datap = NULL;

    // add a second item 
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->socket_id = 333;
    list_insert(Lptr, datap, LISTPOS_TAIL);
    datap = NULL;

    // add a third item
    datap = (data_t *) calloc(1, sizeof(data_t));
    datap->socket_id = 444;
    list_insert(Lptr, datap, LISTPOS_TAIL);
    datap = NULL;

    // find all three and print 
    datap = list_access(Lptr, 0);   /* same as LISTPOS_HEAD */
    if (datap != NULL) {
        printf("Position 0 should find 555 and found (%d)\n", datap->socket_id);
        datap = list_access(Lptr, 1);
        printf("Position 1 should find 333 and found (%d)\n", datap->socket_id);
        datap = list_access(Lptr, LISTPOS_TAIL);
        printf("Position 2 should find 444 and found (%d)\n", datap->socket_id);
    } else {
        printf("ERROR: list access is broken?\n");
    }

    //Next try to use list_debug_print 
    printf("\nTest of list debug print function\n\n");
    list_debug_print(Lptr);

    // you should repeat the above tests with differents insertion orders

    // next you may want to work on list_insert_sorted, so replace the above
    // list_insert commands with list_insert_sorted
 
    // after the insertion functions work, you can try the find command
 
    // Uncomment this section to test list_elem_find 
    /*********************************************************************
    int position = -9876;
    data_t template;
    template.socket_id = 444;
    data_t *foundp = list_elem_find(Lptr, &template, &position);
    printf("\nTest of list elem find\n");
    if (foundp != NULL) {
        printf("looked for %d and found %d in position %d\n",
            template.socket_id, foundp->socket_id, position);
        if (template.socket_id != foundp->socket_id) {
            printf("ERROR: returned pointer to wrong record\n");
        }
        if (position < -1) {
            printf("ERROR: did not set position\n");
        }
    } else {
        printf("looked for %d and did not find. Is this an error? \n", 
            template.socket_id);
    }
    */

    list_destruct(Lptr);
    // End of tests with unsorted list
 
    // Uncomment this section to try some tests on a sorted list 
    /*********************************************************************
    list_t *Lsortptr = list_construct(soc_compare);

    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->socket_id = 23;
    list_insert_sorted(Lsortptr, datap);

    // add a second item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->socket_id = 7;
    list_insert_sorted(Lsortptr, datap);

    // add a third item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->socket_id = 15;
    list_insert_sorted(Lsortptr, datap);

    printf("\nHere is the sorted list\n\n");
    list_debug_print(Lsortptr);

    list_destruct(Lsortptr);
    */

    print_list_order(Lptr);

    return 0;
}

/*

Next you will want to write your own list_debug_print function to print a
list. Then you can do "before and after" testing. That is, print the list
before a change, and print the list after the change to verify that the change
worked.

Something like
*/

void list_debug_print(list_t *list_ptr) {
    list_node_t *L;
    int count;

    printf("list size: %d\n", list_ptr->ll_entries);
    L = list_ptr->ll_head;
    count = 0;
    while(L != NULL) {
        printf("Pos: %d contains ID: %d\n", count, L->data_ptr->socket_id);
        L = L->ll_next;
        count++;
    }
    if (count != list_ptr->ll_entries)
        printf("\nERROR: the list size is wrong! count is %d\n", count);
}

