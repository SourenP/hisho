#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const uint8_t MAX_LEVELS = 5;
static const uint32_t LEAF_SIZE = 64;
static const uint32_t INDEX_COUNT = (1 << MAX_LEVELS) - 1;
static const uint32_t TOTAL_SIZE = (1 << (MAX_LEVELS - 1)) * LEAF_SIZE;
static const uint32_t TOTAL_BLOCK_PTR_COUNT = (1 << (MAX_LEVELS - 1));

typedef struct _header {
    struct _header *next;
    struct _header *prev;
} Header;

/**
 * Base pointer for memory allocated.
 */
static char *_buff_base = NULL;

/**
 * Array of free lists indexed by level.
 */
static Header *_free_lists[MAX_LEVELS] = {NULL};

/**
 * Array of block sizes by indexed by block pointer offset from _buff_base.
 * todo(sourenp): optimize this by storing whether block was split or not and use algo to find size
 *
 * Init to 0 for debugging purposes. This value should always at least be LEAF_SIZE when accessed.
 * todo(sourenp): remove {0}.
 */
static uint32_t alloc_block_size[TOTAL_BLOCK_PTR_COUNT] = {0};

/**
 * Bool indicating if block at index is free.
 * todo(sourenp): reduce this to half the size using the XOR trick
 */
static bool _is_block_free[1 << MAX_LEVELS] = {true};

/**
 * Allocate memory on heap and return a pointer to it.
 * New memory is not guaranteed to be cleared to 0s.
 *
 * @param n_bytes Number of bytes to allocate.
 * @return Pointer to the allocated memory.
 */
void *hisho_buddy__alloc(uint32_t n_bytes);

/**
 * Free memory at pointer.
 * Note that the size of the memory is not needed. The allocator will figure this out.
 *
 * @param p Pointer to memory block to free.
 */
void hisho_buddy__free(void *p);

/**
 * Prints blocks, by their size, in all free lists.
 */
void hisho_buddy__print_free_lists();
