#include "hisho_first_fit.h"

#include <stdio.h>
#include <stdlib.h>

void *hisho_first_fit__alloc(unsigned n_bytes) {
    // Calculate how many units of memory is needed for this block (including header)
    div_t div_res = div(n_bytes, sizeof(Header));
    unsigned n_units = div_res.quot + (div_res.rem > 0) + 1; // + 1 for header

    // Initialize free_start into a dummy start header with no buffer space
    if (free_start == NULL) {
        base.s.next_block = NULL;
        base.s.size = 0;
        base.s.is_used = true;

        free_start = &base;
    }

    // Find an unused block's header and it's previous block's header
    HeaderPair unused_header_pair = _hisho_fit_first__find_unused_block();

    // There are no free blocks, we need to allocate more space and use the new block
    if (unused_header_pair.curr == NULL) {
        unused_header_pair.curr = _hisho_fit_first__more_core(n_units);
        if (unused_header_pair.curr == NULL) {
            printf("Failed to allocate more memory from system. Exiting...\n");
            exit(EXIT_FAILURE);
        }
        unused_header_pair.prev->s.next_block = unused_header_pair.curr;
    }

    return NULL; // todo remove
}

void hisho_first_fit__free(void *p) {
}

Header *_hisho_fit_first__more_core(unsigned n_units) {
    char *new_mem;

    if (n_units < REFRESH_BLOCK_COUNT) {
        n_units = REFRESH_BLOCK_COUNT;
    }

    new_mem = sbrk(n_units * sizeof(Header));
    if (new_mem == (void *)-1) {
        return NULL;
    }

    // Construct new block
    Header *new_header = (Header *)new_mem;
    new_header->s.next_block = NULL;
    new_header->s.size = n_units - 1;
    new_header->s.is_used = false;

    return new_header;
}

static HeaderPair _hisho_fit_first__find_unused_block() {
    Header *prev = free_start;
    Header *curr = free_start->s.next_block;
    while (curr != NULL) {
        if (curr->s.is_used == false) {
            break;
        }
        curr = curr->s.next_block;
        prev = curr;
    }
    return (HeaderPair){.prev = prev, .curr = curr};
}

void hisho_fit_first__print() {
    printf("\n");
    printf("Header  \tSize\tUsed\tNext\n");
    Header *h = free_start;
    while (h != NULL) {
        printf("%p\t%u\t%d\t%p\n", h, h->s.size, h->s.is_used, h->s.next_block);
        h = h->s.next_block;
    }
    printf("\n");
}