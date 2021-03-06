/* mem.c A template
 *
 * Lab4: Dynamic Memory Allocation
 * Version: 1
 * ECE 2230, Fall 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "mem.h"

// Global variables required in mem.c only
static chunk_t Dummy = {&Dummy, 0};
static chunk_t *Rover = &Dummy;
static int C = 0;
static int P = 0;

// private function prototypes
void mem_validate(void);

/* function to request 1 or more pages from the operating system.
 *
 * new_bytes must be the number of bytes that are being requested from
 *           the OS with the sbrk command.  It must be an integer 
 *           multiple of the PAGESIZE
 *
 * returns a pointer to the new memory location.  If the request for
 * new memory fails this function simply returns NULL, and assumes some
 * calling function will handle the error condition.  Since the error
 * condition is catastrophic, nothing can be done but to terminate 
 * the program.
 */
chunk_t *morecore(int new_bytes) {

    char *cp;
    chunk_t *new_p;
    // preconditions
    assert(new_bytes % PAGESIZE == 0 && new_bytes > 0);
    assert(PAGESIZE % sizeof(chunk_t) == 0);
    cp = sbrk(new_bytes);
    if (cp == (char *) -1)  // no space available
        return NULL;
    new_p = (chunk_t *) cp;
    C++;
    P += new_bytes/PAGESIZE;
    return new_p;
}

/* deallocates the space pointed to by return_ptr; it does nothing if
 * return_ptr is NULL.  
 *
 * This function assumes that the Rover pointer has already been 
 * initialized and points to some memory block in the free list.
 */
void Mem_free(void *return_ptr) {

    // precondition
    assert(Rover != NULL && Rover->next != NULL);

    // not coalescing
    chunk_t *free_block = (chunk_t *) return_ptr;
    free_block--; //decrement pointer to get header block
    free_block->next = Rover->next;
    Rover->next = free_block;
}

/* returns a pointer to space for an object of size nbytes, or NULL if the
 * request cannot be satisfied.  The memory is uninitialized.
 *
 * This function assumes that there is a Rover pointer that points to
 * some item in the free list.  The first time the function is called,
 * Rover is null, and must be initialized with a dummy block whose size
 * is one, but set the size field to zero so this block can never be 
 * removed from the list.  After the first call, the Rover can never be null
 * again.
 */
void *Mem_alloc(const int nbytes) {

    chunk_t *prev = NULL;
    chunk_t *avail_block = NULL;
    chunk_t *split_block = NULL;   
    chunk_t *temp = NULL;
    int nunits = nbytes / sizeof(chunk_t) + 1;

    //precondition
    assert(nbytes > 0);
    assert(Rover != NULL && Rover->next != NULL);

    //first fit --> find block of memory >= nunits
    temp = Rover;
    while (Rover->next != temp) {
        if (Rover->size >= nunits)
            break;
        prev = Rover; 
        Rover = Rover->next;
    }

    //if no memory block available -> allocate memory block
    if (Rover == temp) {
            chunk_t *p, *q;
            int num_pages = nbytes/PAGESIZE + 1;
            int new_bytes = PAGESIZE * num_pages;
            p = morecore(new_bytes);
            p->next = NULL;
            p->size = new_bytes / sizeof(chunk_t);
            q = p + 1;
            Mem_free(q);
            avail_block = Rover->next;
            Rover = Rover->next;
            assert(q == p + 1);
            assert(avail_block->size*sizeof(chunk_t) == new_bytes);
            assert(avail_block == Rover);
    }
        
    //if NOT an exact fit -> split block in two
    if (Rover->size >= nunits) {
        int block_size = Rover->size;
        if (Rover->size == nunits) {
            avail_block = Rover;
            Rover = Rover->next;
            prev->next = Rover;
        }
        else {
            split_block = Rover;
            avail_block = split_block + (block_size - nunits);
            split_block->size = block_size - nunits;
            avail_block->size = nunits;
            assert(avail_block->size + split_block->size == block_size);
            assert(avail_block->size == nbytes / sizeof(chunk_t) + 1);
        }
    }

    // here are possible post-conditions, depending on your design
    //
    // assume p is a pointer to memory block that will be given to the user
    // and q is the address given to the user
    // assert(p + 1 == q);
    // the minus one in the next two tests accounts for the header
    // assert((p->size - 1)*sizeof(chunk_t) >= nbytes);
    // assert((p->size - 1)*sizeof(chunk_t) < nbytes + sizeof(chunk_t));
    // assert(p->next == NULL);  // saftey first!
    // return q;

    return avail_block + 1;
}

/* prints stats about the current free list
 *
 * -- number of items in the linked list including dummy item
 * -- min, max, and average size of each item (in bytes)
 * -- total memory in list (in bytes)
 * -- number of calls to sbrk and number of pages requested
 *
 * A message is printed if all the memory is in the free list
 */
void Mem_stats(void)
{
    chunk_t *temp = NULL;
    int min = Rover->size, max = Rover->size;
    int num_items = 0, avg = 0, M = 0; 

     if (Rover->size == 0)
        Rover = Rover->next;
    temp = Rover;
    while (Rover->next != temp) { 
        num_items++;
        M += Rover->size;
        if (Rover->size < min) 
            min = Rover->size;
        if (Rover->size > max)
            max = Rover->size;
        Rover = Rover->next;
    }

    min *= sizeof(chunk_t), max *= sizeof(chunk_t);
    avg = (M/num_items)*sizeof(chunk_t), M *= sizeof(chunk_t);
    printf("Number of items in the free list: %d\n", num_items);
    printf("Min (B) = %d    Max (B) = %d    Average Size (B) = %d\n", min, max, avg);
    printf("Total memory stored (B): %d\n", M);
    printf("Total calls: %d    Total Pages: %d\n", C, P);
    if (M == P*PAGESIZE) 
        printf("all memory is in the heap -- no leaks are possible\n");
}

/* print table of memory in free list 
 *
 * The print should include the dummy item in the list 
 */
void Mem_print(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    chunk_t *p = Rover;
    chunk_t *start = p;
    do {
        // example format.  Modify for your design
        printf("p=%p, size=%d, end=%p, next=%p %s\n", 
                p, p->size, p + p->size, p->next, p->size!=0?"":"<-- dummy");
        p = p->next;
    } while (p != start);
    mem_validate();
}

/* This is an experimental function to attempt to validate the free
 * list when coalescing is used.  It is not clear that these tests
 * will be appropriate for all designs.  If your design utilizes a different
 * approach, that is fine.  You do not need to use this function and you
 * are not required to write your own validate function.
 */
void mem_validate(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    assert(Rover->size >= 0);
    int wrapped = FALSE;
    int found_dummy = FALSE;
    int found_rover = FALSE;
    chunk_t *p, *largest, *smallest;

    // for validate begin at Dummy
    p = &Dummy;
    do {
        if (p->size == 0) {
            assert(found_dummy == FALSE);
            found_dummy = TRUE;
        } else {
            assert(p->size > 0);
        }
        if (p == Rover) {
            assert(found_rover == FALSE);
            found_rover = TRUE;
        }
        p = p->next;
    } while (p != &Dummy);
    assert(found_dummy == TRUE);
    assert(found_rover == TRUE);

    if (Coalescing) {
        do {
            if (p >= p->next) {
                // this is not good unless at the one wrap
                if (wrapped == TRUE) {
                    printf("validate: List is out of order, already found wrap\n");
                    printf("first largest %p, smallest %p\n", largest, smallest);
                    printf("second largest %p, smallest %p\n", p, p->next);
                    Mem_print();
                    assert(0);   // stop and use gdb
                } else {
                    wrapped = TRUE;
                    largest = p;
                    smallest = p->next;
                }
            } else {
                assert(p + p->size < p->next);
            }
            p = p->next;
        } while (p != &Dummy);
        assert(wrapped == TRUE);
    }
}

/* vi:set ts=8 sts=4 sw=4 et: */

