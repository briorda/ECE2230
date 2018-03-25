/* socketlist.c
 * Austin Johnson 
 * agjohns
 * ECE 2230 Fall 2016
 * MP1
 *
 *
 * 
 *
 * Purpose: The purpose of this file is to provide functions for the lab1.c file.
 *          The reason it is separate from the main file is for information hiding
 *          and to allow the programmer to use different data structures with the 
 *          lab1.c file without changing it. 
 *
 *
 * Assumptions: I assume this program will work with MINSOCKETS set to 4. However, 
 *              it should work from 2 to several thousand.  
 *
 *
 * Bugs: No bugs from the tests that I have ran against it. 
 *
 *
 *
 */


#include <stdlib.h>

#include "socketlist.h"

#define MINSOCKETS 4



/*
  Function: socketlist_construct
  Purpose: The purpose of this function is to construct a structure of type socketlist_t 
	   that includes an array with intial size set to MINSOCKETS.
  Parameters: None
  Return: A pointer to the structure 
*/
struct socketlist_t *socketlist_construct() {

	//variables
	struct socketlist_t *socketlist_ptr = NULL;
	int i;

	//initialize array equal to size MINSOCKETS
	socketlist_ptr = (struct socketlist_t *) calloc(1, sizeof(struct socketlist_t));
	socketlist_ptr->vector_capacity = MINSOCKETS;

	//allocate memory for pointers 
	socketlist_ptr->vector_ptr = (struct socket_info_t **) malloc(MINSOCKETS*sizeof(struct socket_info_t *));
	for (i = 0; i < MINSOCKETS; i++) {
		socketlist_ptr->vector_ptr[i] = NULL;
	}

	return socketlist_ptr;
}

/*
  Function: socketlist_destruct 
  Purpose: The purpose of this function is to free all socket_info_t memory blocks in the list, 
	   free array of type socket_info_t *, and free the memory block of type socketlist_t. 
  Parameters: struct socketlist_t *ptr - a pointer to the list
  Return: void  
*/
void socketlist_destruct(struct socketlist_t *ptr) {

	//variables
	int i;

	//free memory blocks in the list
	for (i = 0; i < ptr->vector_size; i++) {  
		free(ptr->vector_ptr[i]);
		ptr->vector_ptr[i] = NULL; //prevent dangling pointers
	}

	//free pointer to struct socket_info_t
	free(ptr->vector_ptr);
	ptr->vector_ptr = NULL;

	//free memory block of type socketlist_t
	free(ptr);
	ptr = NULL;
}

/*
  Function: socketlist_add
  Purpose: The purpose of this function is to insert a socket_info_t memory block into the
	   given position and increment the pointers that are already stored in the array. 
	   The function also resizes the vector if it is not large enough for another record. 
  Parameters: 1. struct socketlist_t *list_ptr - a pointer to the list
	      2. int pos - the position to store the info
      	      3. struct socket_info_t *rec_ptr - a pointer to the socket info
  Return: The function returns a 0 if the record was inserted and a -1 if not.
*/
int socketlist_add(struct socketlist_t *list_ptr, int pos, struct socket_info_t *rec_ptr) {

	//variables
	int i = 0, num_recs = list_ptr->vector_size;

	//check if x is not between 0:N
	if ((pos < 0) || (pos > list_ptr->vector_size)) {
		return -1;
	}

	else {

		//check if array of pointers can accomodate another record
		if (list_ptr->vector_size == list_ptr->vector_capacity) {
			list_ptr->vector_ptr = realloc(list_ptr->vector_ptr, (2*list_ptr->vector_capacity)*sizeof(struct socket_info_t *)); 
			list_ptr->vector_capacity = 2 * list_ptr->vector_capacity;

			//initialize new pointers to NULL
			for (i = list_ptr->vector_size; i < list_ptr->vector_capacity; i++) {
				list_ptr->vector_ptr[i] = NULL;	
			}
		}

		//pushback command --> check if x is equal to N 
		if (pos == list_ptr->vector_size) {

			//insert new record
		    	list_ptr->vector_ptr[list_ptr->vector_size] = rec_ptr; 
		}

		else {
			
			//shift records + 1
			for (i = num_recs - 1; i > pos - 1; i--) {
				list_ptr->vector_ptr[i+1] = list_ptr->vector_ptr[i];
			}

			//insert new record
			list_ptr->vector_ptr[pos] = rec_ptr;
		}
	}

	//check if inserted properly
	if (list_ptr->vector_ptr[pos] == NULL) {
		return -1;
	}

	else { 
		list_ptr->vector_size += 1;
		return 0;
	}
}

/*
  Function: socketlist_replace
  Purpose: The purpose of this function is to replace the socket_info_t memory block at the given position with 
	   the new record. 
  Parameters: 1. struct socketlist_t *list_ptr - a pointer to the list
	      2. int pos - the position to replace
	      3. struct socket_info_t *new_rec_ptr - a pointer to the socket info
  Return: A pointer to the old socket info
*/
struct socket_info_t *socketlist_replace(struct socketlist_t *list_ptr, int pos, struct socket_info_t *new_rec_ptr) {

	//variables
	struct socket_info_t *old_rec_ptr = NULL;

	//get old record
	old_rec_ptr = list_ptr->vector_ptr[pos];

	//insert new rec_ptr
	list_ptr->vector_ptr[pos] = new_rec_ptr;

        return old_rec_ptr;
}

/*
  Function: socketlist_access
  Purpose: The purpose of this function is to return a pointer to the socket_info_t memory block
	   that is found at the given position.
  Parameters: 1. struct socketlist_t *list_ptr - a pointer to the list
	      2. int pos - the position that contains the information
  Return: If the index is within the bounds it returns a pointer to the memory block. Else 
	  it returns NULL.
*/
struct socket_info_t *socketlist_access(struct socketlist_t *list_ptr, int pos) {

	if (pos >= 0 && pos < list_ptr->vector_size) {
		return list_ptr->vector_ptr[pos]; 
	}

	else {
    		return NULL;
	}
}

/*
  Function: socketlist_remove
  Purpose: The purpose of this function is to remove a record at the given position and 
	   decrement all of the higher index values by one. If a record is removed and 
	   the number of remaining records is less than a fourth of the array size, it 
	   resizes the array to half its value or MINSOCKETS, whichever is larger. 
  Parameters: 1. struct socketlist_t *list_ptr - a pointer to the list
	      2. int pos - the position to remove the record
  Return: Return a pointer to the removed record if the position is valid. If the position
	  is invalid it returns NULL.
*/
struct socket_info_t *socketlist_remove(struct socketlist_t *list_ptr, int pos) {

	//variables
	int i = 0, num_recs = list_ptr->vector_size;
	struct socket_info_t *removed_rec_ptr = NULL;

	//check if x is not between 0:N-1
	if ((pos < 0) || (pos >= list_ptr->vector_size) || (list_ptr->vector_size == 0)) {
		return NULL;
	}

	else {

		//get socket_id
		removed_rec_ptr = list_ptr->vector_ptr[pos];
 
		//popback command --> check if x is equal to N-1 
		if (pos == list_ptr->vector_size - 1) {
		
			//erase record at N-1
			list_ptr->vector_ptr[pos] = NULL;
			list_ptr->vector_size -= 1;
			
		}

		else {
			//erase record at postition
			list_ptr->vector_ptr[pos] = NULL;
			list_ptr->vector_size -= 1;
			

			//shift records - 1
			for (i = pos; i < num_recs - 1; i++) { 
				list_ptr->vector_ptr[i] = list_ptr->vector_ptr[i + 1]; 
			}
		}

		//check if remaining records are less than 1/4 of the array size
		if (list_ptr->vector_size < 0.25*list_ptr->vector_capacity) {

			//if current records are greater than MINSOCKETS, half the array size
			if (list_ptr->vector_capacity > MINSOCKETS) {

				list_ptr->vector_ptr = realloc(list_ptr->vector_ptr, (0.5*list_ptr->vector_capacity)*sizeof(struct socket_info_t *)); 
				list_ptr->vector_capacity = 0.5 * list_ptr->vector_capacity;

				//initialize new pointers to NULL
				for (i = list_ptr->vector_size; i < list_ptr->vector_capacity; i++) {
					list_ptr->vector_ptr[i] = NULL;	
				}
	
			}
		
			//else reset array size to MINSOCKETS
			else {
				
				list_ptr->vector_ptr = realloc(list_ptr->vector_ptr, MINSOCKETS*sizeof(struct socket_info_t *)); 
				list_ptr->vector_capacity = MINSOCKETS;			
			}
		}

		return removed_rec_ptr;
	}
}

/*
  Function: socketlist_capacity
  Purpose: The purpose of this function is to return the current size of the
	   array. It is used for information hiding and to allow main to access
	   the size of the array without using members from struct socketlist_t.
  Parameters: struct socketlist_t *list_ptr - a pointer to the list
  Return: The current size of the array. 
*/
int socketlist_capacity(struct socketlist_t *list_ptr) {
	
	return list_ptr->vector_capacity;
}

/*
  Function: socketlist_number_entries
  Purpose: The purpose of this function is to return the current number of socket
	   records stored in the list. It is used for information hiding and to 
	   allow main to access the number of records without using members from 
	   struct socketlist_t.
  Parameters: struct socketlist_t *list_ptr - a pointer to the list
  Return: The current number of records stored in the list. 
*/
int socketlist_number_entries(struct socketlist_t *list_ptr) {
  
	return list_ptr->vector_size;
}

