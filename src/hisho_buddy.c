#include "hisho_buddy.h"

#include <assert.h>
#include <sys/mman.h>

// max number of blocks at level l
static inline uint32_t _max_block_count(uint8_t l) {
    return (1 << l);
};

// get size of block at level l
static inline uint32_t _sizeof_block(uint8_t l) {
    return TOTAL_SIZE / (1 << l);
}

// block index given level and index in level
static inline uint32_t _block_index(uint8_t l, uint32_t i) {
    return (1 << l) + i - 1;
};

// block index given level and index in level
static inline uint32_t _block_index_from_ptr(const void *buffer_start_ptr, const void *block_ptr,
                                             uint8_t l) {
    return ((char *)buffer_start_ptr - (char *)block_ptr) / _sizeof_block(l);
}

// buddy index in level given block index in level
static inline uint32_t _buddy_index_in_level(uint32_t i) {
    return i % 2 == 0 ? i + 1 : i - 1;
}

// block level given block size
static inline uint8_t _block_level(uint32_t n_bytes) {
    for (int l = MAX_LEVELS - 1; l >= 0; l--) {
        if (n_bytes < _sizeof_block(l)) {
            return l;
        }
    }
    return -1;
}

static void _create_block(void *block_ptr, uint8_t level) {
    Header *header = (Header *)block_ptr;
    header->next = NULL;
}

static Header *_find_block_to_alloc(uint8_t level) {
    if (_free_lists[level] == NULL) {
        uint8_t parent_level = level - 1;
        if (parent_level < 0) {
            return NULL;
        }
        Header *parent_header = _find_block_to_alloc(parent_level);
        uint32_t parent_header_size = _sizeof_block(parent_level);
        char *one_header_ptr = (char *)parent_header;
        char *two_header_ptr = one_header_ptr + (parent_header_size / 2);
        _free_lists[level] = (Header *)one_header_ptr;
        _free_lists[level]->next = (Header *)two_header_ptr;
        _free_lists[level]->next->next = NULL;
    }
    Header *header = _free_lists[level];
    _free_lists[level] = header->next;
    return header;
}

void *hisho_buddy__alloc(uint32_t n_bytes) {
    // Check if there is space to allocate
    if (n_bytes > TOTAL_SIZE) {
        fprintf(stderr, "Out of memory.");
        return NULL;
    }

    // System memory allocation if first time allocating
    if (_base_ptr == NULL) {
        // Get heap memory from system
        _base_ptr = mmap(0, TOTAL_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (_base_ptr == MAP_FAILED) {
            fprintf(stderr, "MAP failed.");
            return NULL;
        }
        // Create initial block at level 0
        Header *init_header = (Header *)_base_ptr;
        init_header->next = NULL;
        _free_lists[0] = init_header;
    }

    uint8_t new_block_level = _block_level(n_bytes + sizeof(Header));
    assert(new_block_level >= 0 && new_block_level < MAX_LEVELS);
    Header *new_block = _find_block_to_alloc(new_block_level);
    if (new_block == NULL) {
        fprintf(stderr, "Out of memory.");
        return NULL;
    }

    return NULL;
};

void hisho_buddy__print() {
    for (int l = 0; l < MAX_LEVELS; l++) {
        printf("Level %d: ", l);
        Header *curr = _free_lists[l];
        while (curr != NULL) {
            printf("%u ", _sizeof_block(l));
            curr = curr->next;
        }
        printf("\n");
    }
}