/**
 * A storage allocator implemented with a linked list that allocates blocks using the 'first fit'
 * strategy.
 *
 * Each block has an overhead of 16 bytes for its header.
 * On free a block will coalesce with the next block if it's unused, but not the previous block.
 * All block sizes are a multiple of the block header size for alignment purposes.
 *
 *
 * Written with the help of resoruces listed in README.md
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_CHAR_PRINT 16        // Number of characters of memory to print before stopping in debug
#define REFRESH_BLOCK_COUNT 1024 // Number of block sizes to allocate on sys mem call

/* Header of a block */
typedef union header {
    struct {
        union header *next_block; // Pointer to next block.
        unsigned u_size; // Size of block buffer by units where unit is the size of a header.
        bool is_used;    // True if block is currently being used.
    } s;
    size_t x[2]; // Field not used. Exists for alignment purposes.
} Header;

typedef struct header_pair {
    union header *prev;
    union header *curr;
} HeaderPair;

static Header *free_start = NULL; // Free list starting point
static Header base;               // Dummy header that marks the start of the free list

/**
 * Allocate memory on heap and return a pointer to it.
 * New memory is not guaranteed to be cleared to 0s.
 *
 * @param n_bytes Number of bytes to allocate.
 * @return Pointer to the memory address.
 */
void *hisho_ff__alloc(unsigned n_bytes);

/**
 * Free memory.
 * If the freed block's next block is unused, they will be coalesced into one block.
 *
 * @param p Pointer to memory block to free.
 */
void hisho_ff__free(void *p);

/**
 * Ask system for more memory.
 *
 * @param n_units Number of units of memory blocks to retrive. A unit is the
 * size of a header.
 * @return Return pointer to beginning of memory. Return NULL if failed to retrieve memory.
 */
static Header *_hisho_ff__more_core(unsigned n_units);

/**
 * Find a block in the free list that's unused and fits n_units.
 *
 * @param n_units Units of memory block should be able to fit.
 * @return Pointer to header of unused block. If none are found NULL is returned.
 */
static HeaderPair _hisho_ff__find_unused_block(unsigned n_units);

/**
 * Merge two unused blocks into one.
 * New block will use a's header, will point to b's next and will be marked unused.
 *
 * @param a First block to be coalesced.
 * @param b Second block to be coalesced.
 */
static void _hisho_ff__coalesce(Header *a, Header *b);

/**
 * Print list of blocks and their properties.
 *
 * @param stream Stream to print to.
 */
void hisho_ff__print_blocks(FILE *stream);

/**
 * Print stats about allocator memory.
 *
 * @param stream Stream to print to.
 */
void hisho_ff__print_stats(FILE *stream);