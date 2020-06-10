#include "hisho_buddy.h"

#include <assert.h>
#include <sys/mman.h>

// max number of blocks at level l
static inline uint32_t _max_block_count(uint8_t l) {
    return (1 << l);
};

// get size of block at level l
static inline uint32_t _block_size(uint8_t l) {
    return TOTAL_SIZE / (1 << l);
}

// block index given level and index in level
static inline uint32_t _block_index(uint8_t l, uint32_t i) {
    return (1 << l) + i - 1;
};

// block index given level and index in level
static inline uint32_t _block_index_from_ptr(const void *buffer_start_ptr, const void *block_ptr,
                                             uint8_t l) {
    return ((char *)buffer_start_ptr - (char *)block_ptr) / _block_size(l);
}

// buddy index in level given block index in level
static inline uint32_t _buddy_index_in_level(uint32_t i) {
    return i % 2 == 0 ? i + 1 : i - 1;
}

// block level given block size
static inline uint8_t _block_level(uint32_t n_bytes) {
    for (int l = MAX_LEVELS - 1; l >= 0; l--) {
        if (n_bytes <= _block_size(l)) {
            return l;
        }
    }
    return MAX_LEVELS;
}

// number of leaf_size blocks block ptr is offset by from block buffer base
static inline uint32_t _block_ptr_offset(char *block_ptr) {
    return (block_ptr - _base_ptr) / LEAF_SIZE;
}

/**
 * Find and allocate a free block at a certain level.
 * If a block of the required size for the level aren't found, larger blocks from lower levels are
 * split to create new blocks.
 *
 * @param level Level at which to find free block to allocate
 * @return Pointer to the header of new block. NULL if no block was found (out of memory).
 */
static Header *_find_block_to_alloc(uint8_t level) {
    if (level >= MAX_LEVELS) {
        return NULL;
    }
    if (_free_lists[level] == NULL) {
        if (level == 0) {
            return NULL;
        }
        uint8_t parent_level = level - 1;
        Header *parent_header = _find_block_to_alloc(parent_level);
        if (parent_header == NULL) {
            return NULL;
        }
        uint32_t parent_header_size = _block_size(parent_level);
        char *one_header_ptr = (char *)parent_header;
        char *two_header_ptr = one_header_ptr + (parent_header_size / 2);
        _free_lists[level] = (Header *)one_header_ptr;
        _free_lists[level]->next = (Header *)two_header_ptr;
        _free_lists[level]->next->next = NULL;
    }
    printf("hi\n");
    Header *header = _free_lists[level];
    _free_lists[level] = header->next;
    return header;
}

void *hisho_buddy__alloc(uint32_t n_bytes) {
    // System memory allocation if first time allocating
    if (_base_ptr == NULL) {
        assert(LEAF_SIZE > sizeof(Header));
        // Get heap memory from system
        _base_ptr = mmap(0, TOTAL_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (_base_ptr == MAP_FAILED) {
            fprintf(stderr, "MAP failed.\n");
            return NULL;
        }
        // Create initial block at level 0
        Header *init_header = (Header *)_base_ptr;
        init_header->next = NULL;
        _free_lists[0] = init_header;
    }

    uint8_t new_block_level = _block_level(n_bytes + sizeof(Header));
    assert(new_block_level >= 0);
    Header *new_block = _find_block_to_alloc(new_block_level);
    if (new_block == NULL) {
        fprintf(stderr, "Out of memory.\n");
        return NULL;
    }
    alloc_block_size[_block_ptr_offset((char *)new_block)] = _block_size(new_block_level);
    return new_block + 1; // skip the header and return a pointer to block buffer
};

void hisho_buddy__free(void *p) {
    // todo
    printf("%p %u\n", p, alloc_block_size[_block_ptr_offset(p)]);
}

void hisho_buddy__print_free_lists() {
    for (int l = 0; l < MAX_LEVELS; l++) {
        printf("Level %d: ", l);
        Header *curr = _free_lists[l];
        while (curr != NULL) {
            printf("%u ", _block_size(l));
            curr = curr->next;
        }
        printf("\n");
    }
}