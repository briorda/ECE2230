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

    chunk_t *free_block = (chunk_t *) return_ptr;
    chunk_t *slow = NULL, *prev = NULL;
    free_block--;

    if (Coalescing && Rover->next != Rover) {
            //find location to insert
            slow = Rover;
            prev = slow->next;
            Rover = Rover->next->next;
            while (slow) {
                if ((free_block < prev && free_block > Rover) || 
                    (free_block > prev && Rover == &Dummy))  
                        break;
                slow = prev;
                prev = Rover;
                Rover = Rover->next;  
            }     
            //coalesce with upper neighbor || lower neighbor
            if ((free_block + free_block->size == prev) || (prev + prev->size == free_block)) {
                free_block->size = free_block->size + prev->size;
                free_block->next = Rover;
                slow->next = free_block;
                prev = prev->next;
            }                   
           else {
                free_block->next = Rover->next;
                Rover->next = free_block;
           }
    }
    else {
        free_block->next = Rover->next;
        Rover->next = free_block;
    }
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

    //precondition
    assert(nbytes > 0);
    assert(Rover != NULL && Rover->next != NULL);

    chunk_t *prev = NULL;
    chunk_t *avail_block = NULL;
    chunk_t *split_block = NULL;   
    int nunits = nbytes / sizeof(chunk_t) + 1;

    //not a multiple of sizeof(chunk_t) --> int division
    if (nbytes % sizeof(chunk_t) != 0)
        nunits += 1;

    //find correct block according to fit type
    Find_mem_block(nbytes, SearchPolicy, &prev);

    //if no memory block available -> allocate memory block
    if (Rover->size < nunits) {
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
        
    //if exact -> take block, else split block 
    if (Rover->size >= nunits) {
        int block_size = Rover->size;
        if (Rover->size == nunits) {
            avail_block = Rover;
            Rover = Rover->next;
            prev->next = Rover;
            assert(avail_block->size == block_size);
        }
        else {
            split_block = Rover;
            avail_block = split_block + (block_size - nunits);
            split_block->size = block_size - nunits;
            avail_block->size = nunits;
            assert(avail_block->size + split_block->size == block_size);
            assert(nbytes%16==0 ? avail_block->size == nbytes / sizeof(chunk_t) + 1
                               : avail_block->size == nbytes / sizeof(chunk_t) + 2);
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
    chunk_t *slow = NULL, *fast = NULL;
    int min = 0, max = 0;
    int num_items = 0, avg = 0, M = 0;
     
    if (Rover->size == 0)
        Rover = Rover->next;
    slow = Rover, fast = Rover;
    min = Rover->size, max = Rover->size;
    while (slow && fast && fast->next) { 
        if (Rover->size == 0);
        else {
            num_items++;
            M += Rover->size;
            if (Rover->size < min) 
                min = Rover->size;
            if (Rover->size > max)
                max = Rover->size;
        }
        slow = slow->next;
        fast = fast->next->next;
        Rover = Rover->next;
        if (slow == fast)
            break;
    }
    if (Rover->next == Rover);
    else {
        min *= sizeof(chunk_t), max *= sizeof(chunk_t);
        M *= sizeof(chunk_t), avg = (M/num_items);
    }
    printf("\nFree List Stats:"); 
    printf("\nNumber of items: %d\t\tSize of chunk_t (B): %lu", num_items, sizeof(chunk_t));
    printf("\nMin Size (B): %d\t\tMax Size (B): %d", min, max);
    printf("\nAverage Size (B): %d\t\tTotal memory (B): %d", avg, M);
    printf("\nTotal calls: %d\t\t\tTotal Pages: %d\n\n", C, P);
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
    //mem_validate();
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

void Find_mem_block (const int nbytes, int fit_type, chunk_t **prev) {

    chunk_t *slow = Rover, *fast = Rover, *mem_block = Rover;
    int nunits = nbytes / sizeof(chunk_t) + 1;
    int best_fit = 9999999, worst_fit = 0;

    if (fit_type == FIRST_FIT) {
        while (slow && fast && fast->next) { 
            *prev = Rover;
            Rover = Rover->next;
            slow = slow->next;
            fast = fast->next->next;
            if (Rover->size >= nunits || slow == fast)
                break;
        }
    } 

    if (fit_type == BEST_FIT) {
        while (slow && fast && fast->next) { 
            *prev = Rover;
            Rover = Rover->next;
            slow = slow->next;
            fast = fast->next->next;
            if ((Rover->size-nunits > 0) && (Rover->size-nunits < best_fit)) {
                best_fit = Rover->size - nunits;
                mem_block = Rover;
            }
            if (slow == fast) {
                Rover = mem_block;
                break;
            }
        }
    }

    if (fit_type == WORST_FIT) {
        while (slow && fast && fast->next) { 
            *prev = Rover;
            Rover = Rover->next;
            slow = slow->next;
            fast = fast->next->next;
            if ((Rover->size-nunits > 0) && (Rover->size-nunits > worst_fit)) {
                worst_fit = Rover->size - nunits;
                mem_block = Rover;      
            }          
            if (slow == fast) {
                Rover = mem_block;
                break;
            }
        }
    }
}

/* vi:set ts=8 sts=4 sw=4 et: */
