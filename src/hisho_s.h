/**
 * A rudimentary storage allocator that uses a LIFO queue (stack) to manage
 * memory.
 *
 * Free calls must be made in opposite order to the alloc calls.
 * Memory is fixed in size and stored in a static variable (data segment of
 * virtual address space of program).
 *
 * See README.md for resrouces used to write this.
 */

#pragma once

#include <stdio.h>

#define DEBUG_HISHO_STACK 0

#define ALLOC_BUFFER_SIZE 12

static char alloc_buffer[ALLOC_BUFFER_SIZE];
static char *alloc_next = alloc_buffer;

/**
 * Allocate a certain number of characters.
 * @param n Number of characters to allocate.
 * @return Pointer to n characters in storage.
 */
char *hisho_s__alloc(int n);

/**
 * Free previously allocated storage space pointed to by p.
 * @param p Pointer to start of storage space to free.
 */
void hisho_s__free(char *p);

/**
 * Print characters of the entire buffer and show where next is pointing with a
 * ^ char. Line of characters will be wrapped with square brackets.
 * Example output:
 * [hello  ]
 * [     ^ ]
 * Used for debugging.
 */
void hisho_s__print();