#include "hisho_ff.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

void *hisho_ff__alloc(unsigned n_bytes) {
    // Calculate how many units of memory is needed for this block (including header)
    div_t div_res = div(n_bytes, sizeof(Header));
    unsigned n_units = div_res.quot + (div_res.rem > 0) + 1; // + 1 for header

    // Initialize free_start into a dummy start header with no buffer space
    if (free_start == NULL) {
        base.s.next_block = NULL;
        base.s.u_size = 0;
        base.s.is_used = true;

        free_start = &base;
    }

    // Find an unused block's header (curr) and its previous block's header (prev)
    HeaderPair unused_header_pair = _hisho_ff__find_unused_block(n_units);
    Header *prev_header = unused_header_pair.prev;
    Header *curr_header = unused_header_pair.curr;

    // If there are no free blocks we need to allocate more mem and create a block from that mem
    if (curr_header == NULL) {
        curr_header = _hisho_ff__more_core(n_units);
        if (curr_header == NULL) {
            printf("Failed to allocate more memory from system. Exiting.\n");
            exit(EXIT_FAILURE);
        }

        prev_header->s.next_block = curr_header;
    }

    // Start by creating unused block with the remaining memory that will be left over
    size_t rem_units = curr_header->s.u_size - n_units; // remaining units of memory
    if (rem_units > 1) {
        Header *rem_header = curr_header + 1 + n_units;
        rem_header->s.next_block = NULL;
        rem_header->s.is_used = false;
        rem_header->s.u_size = rem_units - 1;
        curr_header->s.next_block = rem_header;
    } else {
        curr_header->s.next_block = NULL;
    }

    // Create current block
    curr_header->s.u_size = n_units;
    curr_header->s.is_used = true;

    return curr_header + 1; // return memory address of buffer after header
}

void hisho_ff__free(void *p) {
    Header *curr = (Header *)p - 1; // Move back pointer to header
    curr->s.is_used = false;

    // If next block is unused, merge with current block
    Header *next = curr->s.next_block;
    if (next->s.is_used == false) {
        _hisho_ff__merge(curr, next);
    }
}

Header *_hisho_ff__more_core(unsigned n_units) {
    char *new_mem;

    if (n_units < REFRESH_BLOCK_COUNT) {
        n_units = REFRESH_BLOCK_COUNT;
    }

    size_t size = n_units * sizeof(Header);
    new_mem = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_mem == MAP_FAILED) {
        return NULL;
    }

    // Construct new block
    Header *new_header = (Header *)new_mem;
    new_header->s.next_block = NULL;
    new_header->s.u_size = n_units - 1;
    new_header->s.is_used = false;

    return new_header;
}

HeaderPair _hisho_ff__find_unused_block(unsigned n_units) {
    Header *prev = free_start;
    Header *curr = free_start->s.next_block;
    while (curr != NULL) {
        if (curr->s.is_used == false && curr->s.u_size >= n_units) {
            break;
        }
        prev = curr;
        curr = curr->s.next_block;
    }
    return (HeaderPair){.prev = prev, .curr = curr};
}

void _hisho_ff__merge(Header *a, Header *b) {
    a->s.next_block = b->s.next_block;
    a->s.u_size = a->s.u_size + b->s.u_size + 1; // +1 for b's header
    a->s.is_used = false;
}

void hisho_ff__print_blocks() {
    printf("BLOCKS\n");
    printf("%-8s\t%s\t%s\t%s\t%-8s\t%s\n", "Header", "Units", "Size", "Used", "Next", "Chars");
    Header *h = free_start;
    while (h != NULL) {
        printf("%p\t%u\t%lu\t%d\t%-8p\t", h, h->s.u_size, h->s.u_size * sizeof(Header),
               h->s.is_used, h->s.next_block);
        char *c = (char *)(h + 1);
        for (int i = 0; i < (h->s.u_size * sizeof(Header)); i++) {
            printf("%c", c[i]);
        }
        h = h->s.next_block;
        printf("\n");
    }
}

void hisho_ff__print_stats() {
    unsigned header_units = 0;
    unsigned buffer_units = 0;
    unsigned free_units = 0;
    unsigned total_units = 0;

    Header *h = (Header *)free_start;
    if (h == NULL) {
        return;
    }
    h = h->s.next_block; // Skip dummy block
    while (h != NULL) {
        header_units += 1;
        if (h->s.is_used) {
            buffer_units += h->s.u_size;
        } else {
            free_units += h->s.u_size;
        }
        h = h->s.next_block;
    }
    total_units = header_units + buffer_units + free_units;
    printf("UNIT COUNTS\n");
    printf("%s\t%s\t%s\t%s\n", "Header", "Buffer", "Free", "Total");
    printf("%u\t%u\t%u\t%u\n", header_units, buffer_units, free_units, total_units);
}