/**
 * A storage allocator implemented with a linked list that allocates blocks
 * using the 'first fit' strategy.
 *
 * All block sizes are a multiple of the block header size which is set to long
 * for alignment (optimization).
 *
 * Written with the help of:
 * - "K&R: The C Programming Language. 2nd Edition: Chapter 8.7"
 * - "TODO"
 */

#include <stdbool.h>
#include <unistd.h>

#define REFRESH_BLOCK_COUNT 1024 // Number of block sizes to allocate on call to sbrk

typedef union header {
    struct {
        union header *next_block;
        unsigned size;
        bool is_used;
    } s;
    long x[2]; // not used
} Header;

typedef struct header_pair {
    union header *prev;
    union header *curr;
} HeaderPair;

static Header *free_start = NULL; // Free list starting point
static Header base;               // Dummy header that marks the start of the free list

void *hisho_first_fit__alloc(unsigned nbytes);

void hisho_first_fit__free(void *p);

/**
 * Ask system for more memory.
 *
 * @param n_units Number of units of memory blocks to retrive. A unit is the
 * size of a header.
 * @return Return pointer to beginning of memory. Return NULL if failed to retrieve memory.
 */
static Header *_hisho_fit_first__more_core(unsigned n_units);

/**
 * Find an unused block in the free list.
 *
 * @return Pointer to header of unused block. If none are found NULL is returned.
 */
static HeaderPair _hisho_fit_first__find_unused_block();

void hisho_fit_first__print();