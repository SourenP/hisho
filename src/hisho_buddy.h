#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const uint32_t LEAF_SIZE = 64; // smallest block size in bytes
static const uint8_t LEVEL_COUNT = 5; // number of levels, determines total size
static const uint32_t INDEX_COUNT = (1 << LEVEL_COUNT) - 1;
static const uint32_t TOTAL_SIZE = (1 << (LEVEL_COUNT - 1)) * LEAF_SIZE;
static const uint32_t TOTAL_BLOCK_PTR_COUNT = (1 << (LEVEL_COUNT - 1));

typedef struct _header {
    struct _header *next;
    struct _header *prev;
} Header;

/**
 * Pointer to beginning of memory where blocks are stored.
 */
static char *_buff_base = NULL;

/**
 * Array of free lists indexed by level.
 */
static Header *_free_lists[LEVEL_COUNT] = {NULL};

/**
 * Array of block sizes by indexed by block pointer offset from _buff_base.
 */
static uint32_t alloc_block_size[TOTAL_BLOCK_PTR_COUNT];

/**
 * Bool indicating if block at index is free.
 */
static bool _is_block_free[1 << LEVEL_COUNT] = {true};

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
