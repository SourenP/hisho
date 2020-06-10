#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static const uint8_t MAX_LEVELS = 5;
static const uint32_t LEAF_SIZE = 16;
static const uint32_t INDEX_COUNT = (1 << MAX_LEVELS) - 1;
static const uint32_t TOTAL_SIZE = (1 << MAX_LEVELS) * LEAF_SIZE;

typedef struct _header {
    struct _header *next;
} Header;

/**
 * Base pointer for memory allocated.
 */
static char *_base_ptr = NULL;

/**
 * Array of free lists for each level.
 */
static Header *_free_lists[MAX_LEVELS] = {NULL};

/**
 * Bool indicating if block at index is free.
 * todo(sourenp): reduce this to half the size using the XOR trick
 * todo(sourenp): figure out what this is for lol
 */
// static bool _is_block_free[1 << MAX_LEVELS] = {false};

/**
 * Allocate memory on heap and return a pointer to it.
 * New memory is not guaranteed to be cleared to 0s.
 *
 * @param n_bytes Number of bytes to allocate.
 * @return Pointer to the allocated memory.
 */
void *hisho_buddy__alloc(uint32_t n_bytes);

/**
 * Print free lists and their sizes at each level.
 */
void hisho_buddy__print_free_lists();