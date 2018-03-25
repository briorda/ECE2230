/* table.c               
 * Austin Johnson 
 * agjohns       
 * ECE 2230 Fall 2016
 * MP7
 *
 * Purpose: The purpose of this file is to provide functions
 *          for the Table ADT in lab7.c 
 *
 * Assumptions: I assume the probe types used are Linear, Double and
 *		Chain. I also assume the addresses are decremented
 *              rather than incremented when searching. 
 *
 *   
 * Bugs: No bugs from the tests I have ran against it. 
 *
 *
 */

#define TRUE 1
#define FALSE 0

#define EMPTY -1
#define OCCUPIED -2
#define DELETED -3 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "table.h"


/*  The empty table is created.  The table must be dynamically allocated and
 *  have a total size of table_size.  The maximum number of (K, I) entries
 *  that can be stored in the table is table_size-1.  For open addressing, 
 *  the table is filled with a special empty key distinct from all other 
 *  nonempty keys (e.g., NULL).  
 *
 *  the probe_type must be one of {LINEAR, DOUBLE, CHAIN}
 *
 *  Do not "correct" the table_size or probe decrement if there is a chance
 *  that the combinaion of table size or probe decrement will not cover
 *  all entries in the table.  Instead we will experiment to determine under
 *  what conditions an incorrect choice of table size and probe decrement
 *  results in poor performance.
 */
table_t *table_construct(int table_size, int probe_type) {

	table_t *T = (table_t *) malloc(sizeof(table_t));
	T->table_size = table_size;
	T->probe_type = probe_type;
	T->num_keys = 0;
	T->num_probes = 0;
	T->oa = (table_entry_t *) malloc(table_size*sizeof(table_entry_t));
	T->sc = (sep_chain_t **) malloc(table_size*sizeof(sep_chain_t *));
	int i;
	for (i = 0; i < table_size; i++) {
		T->oa[i].key = NULL;
		T->oa[i].data_ptr = NULL;
		T->oa[i].deleted = FALSE;		
		T->sc[i] = NULL;
	}
	return T;
}

/*
 * This is a hash function that implements the Shift-Add-XOR Hash
 * Reference: http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 */
unsigned hash(table_t *T, hashkey_t key) {
	hashkey_t p = key;
	unsigned hash_index = 0;
	int i, length = strlen(key);

	for (i = 0; i < length; i++) 
	    hash_index ^= (hash_index << 5) + (hash_index >> 2) + p[i];
	return hash_index % T->table_size;
}

/*
 * This is a hash function that implements the FNV Hash 
 * Reference: http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 *
 * This function may result in an infinite loop because of the following note:
 *
 * NOTE: Do not "correct" the table_size or probe decrement if there is a chance that 
 *	 the combination of table size and probe decrement will not cover all entries 
 *	 in the table. 
 *
 * If the above statement was not the case, I would use the following function:
 *	unsigned probe_decrement(int hash_index) {
 *		int step_size = 7 - hash_index % 7;
 *		if (step_size <= 0) return 1; //step_size must be greater than 0
 *		else if (step_size % 2 == 0) return step_size + 1; //must be odd
 *		else return step_size;
 *	}
 *
 */
unsigned probe_decrement(table_t *T, hashkey_t key) {
        hashkey_t p = key;
        unsigned step_size = 2166136261;
        int i, length = strlen(key);

        for (i = 0; i < length; i++)
            step_size = (step_size * 16777619) ^ p[i];
	
	step_size %= T->table_size;
	if (step_size > 0) return step_size;
	else return 1;        
}

/* Sequentially remove each table entry (K, I) and insert into a new
 * empty table with size new_table_size.  Free the memory for the old table
 * and return the pointer to the new table.  The probe type should remain
 * the same.
 *
 * Do not rehash the table during an insert or delete function call.  Instead
 * use drivers to verify under what conditions rehashing is required, and
 * call the rehash function in the driver to show how the performance
 * can be improved.
 */
table_t *table_rehash(table_t *T, int new_table_size) {
	table_t *new_table = table_construct(new_table_size, T->probe_type);
	int i, entry_class;
	if (T->probe_type != 2) {
		for (i = 0; i < T->table_size; i++) {	
				entry_class = classify_entry(T->oa[i]);
				if (entry_class == OCCUPIED) {
					hashkey_t key = T->oa[i].key;
					data_t data = T->oa[i].data_ptr;
					table_insert(new_table, key, data); 
				}	
	
		}
	}
	else {
		for (i = 0; i < T->table_size; i++) {
			sep_chain_t *R = T->sc[i];
			while (R != NULL) {
				hashkey_t key = R->key;
				data_t data = R->data_ptr;
				table_insert(new_table, key, data); 
				R = R->next;
			}
		}
		//destruct sc array
		for (i = 0; i < T->table_size; i++) {
		    	sep_chain_t *R = T->sc[i];
		    	sep_chain_t *temp = NULL;
			while (R != NULL) {
				temp = R;
				R = R->next;
				free(temp);
			}
		}
   	}
	//destruct old table
	free(T->oa);
	free(T->sc);
	free(T);
	return new_table;
} 


/* returns number of entries in the table */
int table_entries(table_t *T) {	
	return T == NULL ? 0 : T->num_keys;
}


/* returns 1 if table is full and 0 if not full. 
 * For separate chaining the table is never full
 */
int table_full(table_t *T) {
	if (T == NULL || T->num_keys < T->table_size - 1) return 0;
	return 1;
}


/* returns the number of table entries marked as deleted */
int table_deletekeys(table_t *T) {
	int i, deleted_keys = 0;
	for (i = 0; i < T->table_size; i++) {
		if (T->oa[i].deleted == TRUE) 
			deleted_keys++;
	}
	return deleted_keys;
}

/* classifies the entry in the Open Addressing array and  
   returns it as EMPTY, DELETED, or OCCUPIED. NA for Seperate Chaining */ 
int classify_entry (table_entry_t entry) { 
	if (entry.key == NULL && entry.deleted == FALSE) return EMPTY;
	else if (entry.key == NULL && entry.deleted == TRUE) return DELETED;
	else return OCCUPIED;
}

/* Insert a new table entry (K, I) into the table provided the table is not
 * already full.  
 * Return:
 *      0 if (K, I) is inserted, 
 *      1 if an older (K, I) is already in the table (update with the new I), or 
 *     -1 if the (K, I) pair cannot be inserted.
 *
 * Note that both K and I are pointers to memory blocks created by malloc()
 */
int table_insert(table_t *T, hashkey_t K, data_t I) {

	//get hash table index
	int hash_index = hash(T, K);
	int step_size = probe_decrement(T, K);
	T->num_probes = 1;

	//Open Addressing
	if (T->probe_type == 0 || T->probe_type == 1) {
		int curr_index = hash_index;
		int entry_class = classify_entry(T->oa[hash_index]);
		//search for an empty or deleted spot
		do {
			//if found an empty or deleted spot
			if (entry_class != OCCUPIED) 
				break;
			//if duplicate key is found update info
			if (strcmp(T->oa[curr_index].key, K) == 0) {
				free(K); 
				free(T->oa[curr_index].data_ptr);
				T->oa[curr_index].data_ptr = I;
				return 1;
			}
			//move to another cell if current cell is full		
			if (T->probe_type == 0) 
				curr_index--;
			else 
				curr_index -= step_size;
			//if reached the end of the table
			if (curr_index < 0) 
				curr_index += T->table_size; //wrap around		
			entry_class = classify_entry(T->oa[curr_index]);
			T->num_probes++;
		} while (curr_index != hash_index);

		if (table_full(T) || entry_class == OCCUPIED) {
			free(I);		
			return -1;
		}
		T->oa[curr_index].key = K;
		T->oa[curr_index].data_ptr = I;
		T->oa[curr_index].deleted = FALSE;
		T->num_keys++;
		return 0;	
	}
	//Separate Chaining
	else if (T->probe_type == 2) {
		T->num_probes = 0;
		//check for a duplicate key
		if (T->sc[hash_index] != NULL) {
			sep_chain_t *R = T->sc[hash_index];
			while (R != NULL) {
				//if duplicate key is found update info
				if (strcmp(R->key, K) == 0) {
					free(K);
					free(R->data_ptr);
					R->data_ptr = I;
					return 1;
				}
				R = R->next;
				T->num_probes++;
			}
		}
		sep_chain_t *link = (sep_chain_t *) malloc(sizeof(sep_chain_t));
		link->key = K;
    		link->data_ptr = I;
    		link->next = T->sc[hash_index]; //insert at the head
		T->sc[hash_index] = link;	
		T->num_keys++;
	}
	return 0;
}


/* Delete the table entry (K, I) from the table.  Free the key in the table.
 * Return:
 *     pointer to I, or
 *     null if (K, I) is not found in the table.  
 *
 * Be sure to free(K)
 *
 * See the note on page 490 in Standish's book about marking table entries for
 * deletions when using open addressing.
 */
data_t table_delete(table_t *T, hashkey_t K) {

	int hash_index = hash(T, K);
	int step_size = probe_decrement(T, K);
	data_t data = NULL;
	T->num_probes = 1;

	//Open Addressing
	if (T->probe_type == 0 || T->probe_type == 1) {
		int entry_class = classify_entry(T->oa[hash_index]);			
		//if entry does not exist
		if (entry_class == EMPTY)
			data = NULL;
		//search for key
		else {
			int curr_index = hash_index;
			entry_class = classify_entry(T->oa[curr_index]);
			do {
				//found key
				if (entry_class == OCCUPIED && 
				          strcmp(T->oa[curr_index].key, K) == 0)
					break;
				if (T->probe_type == 0) 
					curr_index--; //linear
				else 
					curr_index -= step_size; //double 
				//if reached the end of the table
				if (curr_index < 0) 
					curr_index += T->table_size; //wrap around
				entry_class = classify_entry(T->oa[curr_index]);
				T->num_probes++;	
			}
			while (entry_class != EMPTY && curr_index != hash_index); 
			//if found key --> return I
			if (entry_class == OCCUPIED && strcmp(T->oa[curr_index].key, K) == 0) {
				data = T->oa[curr_index].data_ptr;
				free(T->oa[curr_index].key);
				T->oa[curr_index].key = NULL;
				T->oa[curr_index].deleted = TRUE;
				T->num_keys--;
			}
		}
	}
	//Separate Chaining 
	else {
		T->num_probes = 0;
		sep_chain_t *prev = NULL;
		sep_chain_t *curr = T->sc[hash_index];
		while (curr != NULL) {
			if (strcmp(curr->key, K) == 0) 
				break;
			prev = curr;
			curr = curr->next;
			T->num_probes++;
		}
		//if entry does not exist
		if (curr == NULL && (prev == NULL || strcmp(prev->key, K) != 0)) 
			return NULL;
		//if head
		else if (curr == T->sc[hash_index]) {
			data = curr->data_ptr;
			T->sc[hash_index] = curr->next;
			free(curr->key);
			free(curr);
		}
		//if tail
		else if (prev->next == NULL) {
			data = prev->data_ptr;
			free(prev->key);
			free(prev);
		}
		else {
			data = curr->data_ptr;
			prev->next = curr->next;
			free(curr->key);
			free(curr);
		}
		T->num_keys--;
	}
	return data;
} 


/* Given a key, K, retrieve the pointer to the information, I, from the table,
 * but do not remove (K, I) from the table.  Return NULL if the key is not
 * found.
 */
data_t table_retrieve(table_t *T, hashkey_t K) {
	
	data_t data = NULL;
	int hash_index = hash(T, K);
	int step_size = probe_decrement(T, K);
	T->num_probes = 1;

	//Open Addressing
	if (T->probe_type == 0 || T->probe_type == 1) {
		int entry_class = classify_entry(T->oa[hash_index]);
		//if key does not exist in table --> return NULL
		if (entry_class == EMPTY) 
			data = NULL;
		//search for key	
		else {
			int curr_index = hash_index;
			entry_class = classify_entry(T->oa[curr_index]);
			do {
				//found key
				if (entry_class == OCCUPIED && 
				          strcmp(T->oa[curr_index].key, K) == 0)
					break;
				if (T->probe_type == 0) 
					curr_index--; //linear
				else 
					curr_index -= step_size; //double 
				//if reached the end of the table
				if (curr_index < 0) 
					curr_index += T->table_size; //wrap around
				entry_class = classify_entry(T->oa[curr_index]);
				T->num_probes++;
			} while (entry_class != EMPTY && curr_index != hash_index); 
			//if found key --> return I
			if (entry_class == OCCUPIED && strcmp(T->oa[curr_index].key, K) == 0) 
				data = T->oa[curr_index].data_ptr; 
		}
	}
	//Separate Chaining 
	else {
		T->num_probes = 0;
		sep_chain_t *R = T->sc[hash_index];
		while (R != NULL) {
			if (strcmp(R->key, K) == 0) {
				data = R->data_ptr;
				break;
			}
			R = R->next;
			T->num_probes++;
		}
	}
	return data;
} 


/* Free all information in the table, the table itself, and any additional
 * headers or other supporting data structures.  
 */
void table_destruct(table_t *T) {
	if (T->probe_type == 0 || T->probe_type == 1) {
		int i, entry_class;
		for (i = 0; i < T->table_size; i++) {
			entry_class = classify_entry(T->oa[i]);
			if (entry_class == OCCUPIED) {			
				free(T->oa[i].key);
				free(T->oa[i].data_ptr);
			}
		}
	}
	else {
		int i;
		for (i = 0; i < T->table_size; i++) {
		    	sep_chain_t *R = T->sc[i];
		    	sep_chain_t *temp = NULL;
			while (R != NULL) {
				temp = R;
				R = R->next;
				free(temp->key);
				free(temp->data_ptr);
				free(temp);
			}
		}
   	}
	free(T->oa);
	free(T->sc);
	free(T);
}


/* The number of probes for the most recent call to table_retrieve,
 * table_insert, or table_delete 
 */
int table_stats(table_t *T) {
	return T->num_probes;
}  


/* This function is for testing purposes only.  Given an index position into
 * the hash table return the value of the key if data is stored in this 
 * index position.  If the index position does not contain data, then the
 * return value must be zero.  Make the first
 * lines of this function 
 *       assert(0 <= index && index < table_size); 
 *       assert(0 <= list_position); 
 */
hashkey_t table_peek(table_t *T, int index, int list_position) {
	assert(0 <= index && index < T->table_size); 
	assert(0 <= list_position); 
	
	if (T->probe_type == 0 || T->probe_type == 1) {
		if (T->oa[index].key != NULL)
			return T->oa[index].key;
		else
			return 0;
	}
	else {
		if (T->sc[index] == NULL) return 0;
		else {
			int i = 0;
			sep_chain_t *R = T->sc[index];
			while (R != NULL) {
				if (i == list_position) 	
					return R->key;
				i++;
				R = R->next;
			}
			return 0;
		}
	}
	return 0;		
} 

/* Print the table position and keys in a easily readable and compact format.
 * Only useful when the table is small.
 */
void table_debug_print(table_t *T) {
	int i, j, entry_class;
	//if the table is empty
	if (T->num_keys == 0) 
		printf("\nThe table is empty.\n");
	//Open Addressing
	else if (T->probe_type == 0 || T->probe_type == 1) {
		for (i=0, j = 1; i < T->table_size; i++, j++) {
			entry_class = classify_entry(T->oa[i]);
			if (entry_class == DELETED) 
				printf("*"); 
			printf("%d: %s\t", i, T->oa[i].key);
			if (j == 3) {
				printf("\n");
				j = 0;
			}
		}
		printf("\n* indicates an entry is deleted\n");  
	}	
	//Separate Chaining
	else {
		for (i = 0; i < T->table_size; i++) {
		    sep_chain_t *R = T->sc[i];
		    if (R == NULL) continue;
		    printf("%d: ", i);
		    while (R->next != NULL) {
		        printf("%s -> ", R->key);
		        R = R->next;
		    }
		    printf("%s\n", R->key);
		}
	}
}
