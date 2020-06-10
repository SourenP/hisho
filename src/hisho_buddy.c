#include "hisho_buddy.h"

#include <assert.h>
#include <sys/mman.h>

/**
 * Get block size at a level.
 *
 * @param level Level
 * @return Size of block at level
 */
static uint32_t _block_size(uint8_t level) {
    return TOTAL_SIZE / (1 << level);
}

/**
 * Get level index of a block by providing its pointer and level.
 *
 * The level index of a block is where it falls sequentially if the level was an array of blocks.
 * Visualization:
 *     -----------------
 * 128 |       0       |
 *     -----------------
 * 64  |   0   |   1   |
 *     -----------------
 * 32  | 0 | 1 | 2 | 3 |
 *     -----------------
 *
 * @param block Pointer to block.
 * @param level Level of block.
 * @return Level index of block.
 */
static uint32_t _block_level_index(const void *block, uint8_t level) {
    return ((char *)block - (char *)_buff_base) / _block_size(level);
}

/**
 * Get index of a block by providing its level and level index.
 *
 * The index of a block is where it falls sequentially if all levels together were an array of
 * blocks.
 * Visualization:
 *     -----------------
 * 128 |       0       |
 *     -----------------
 * 64  |   1   |   2   |
 *     -----------------
 * 32  | 3 | 4 | 5 | 6 |
 *     -----------------
 *
 * @param level Level of block.
 * @param level_index Level index of block.
 * @return Index of block.
 */
static uint32_t _block_index(uint8_t level, uint8_t level_index) {
    return (1 << level) + level_index - 1;
};

/**
 * Get the level index of block's buddy by providing its level index.
 *
 * @param block_level_index Level index of block whose buddy level is needed.
 * @return Level index of blocks buddy.
 */
static uint32_t _buddy_level_index(uint32_t block_level_index) {
    return block_level_index % 2 == 0 ? block_level_index + 1 : block_level_index - 1;
}

/**
 * Get the level a block would be on by providing its size in bytes.
 * Note that the size passed in doesn't need to be exactly the size of a block. The size is rounded
 * up to the smallest block that can contain memory of this size.
 *
 * @param n_bytes Size of memory to get corresponding block's level for.
 * @return Level of block that could hold n_bytes.
 */
static uint8_t _block_level(uint32_t n_bytes) {
    for (int l = LEVEL_COUNT - 1; l >= 0; l--) {
        if (n_bytes <= _block_size(l)) {
            return l;
        }
    }
    return LEVEL_COUNT;
}

/**
 * Get number of leaf_size blocks a block is offset by from the buffer base.
 * Visualization of offsets on two levels where leaf size is 32:
 *     -----------------
 * 64  | 0 | 1 | 2 | 3 |
 *     -----------------
 * 32  | 4 | 5 | 6 | 7 |
 *     -----------------
 *
 * @param block_ptr Pointer to block whose offset is needed.
 * @return Offset of block from base.
 */
static uint32_t _block_ptr_offset(char *block_ptr) {
    return (block_ptr - _buff_base) / LEAF_SIZE;
}

/**
 * Initalize heap memory for the allocator by creating a new mapping in the virtual address space.
 * A single block the size of the memory allocated will be created at level 0.
 */
static void _init_memory() {
    // Get heap memory from system
    _buff_base = mmap(0, TOTAL_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (_buff_base == MAP_FAILED) {
        fprintf(stderr, "MAP failed.\n");
        exit(1);
    }
    // Create initial block at level 0
    Header *init_header = (Header *)_buff_base;
    init_header->next = NULL;
    _free_lists[0] = init_header;
}

/**
 * Push a block onto the end of the free list corresponding to its level.
 * Block is marked as free as a side effect.
 *
 * @param level Level of block to be pushed. Determines which free list to push onto.
 * @param block Block to be pushed
 */
static void _free_list_push_back(uint8_t level, Header *block) {
    if (_free_lists[level] == NULL) {
        _free_lists[level] = block;
        block->prev = NULL;
    } else {
        Header *curr = _free_lists[level];
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = block;
        block->prev = curr;
    }
    block->next = NULL;

    uint32_t level_index = _block_level_index(block, level);
    _is_block_free[_block_index(level, level_index)] = true;
}

/**
 * Pop the front most block off the free list of a level.
 * Block is marked as unfree as a side effect.
 *
 * @param level Level of block to be popped. Determines which free list to pop from.
 * @return Header pointer to block that was popped.
 */
static Header *_free_list_pop_front(uint8_t level) {
    Header *block = _free_lists[level];
    _free_lists[level] = block->next;
    if (block->next != NULL) {
        block->next->prev = NULL;
    }

    uint32_t level_index = _block_level_index(block, level);
    _is_block_free[_block_index(level, level_index)] = false;

    return block;
}

/**
 * Remove a block off the free list of a level.
 * Block is marked as unfree as a side effect.
 *
 * @param level Level of block to be removed. Determines which free list to remove from.
 * @return Header pointer to block that was removed.
 */
static void _free_list_remove(uint8_t level, Header *block) {
    if (block->prev == NULL) {
        _free_list_pop_front(level);
        return;
    }

    block->prev->next = block->next;
    uint32_t level_index = _block_level_index(block, level);
    _is_block_free[_block_index(level, level_index)] = false;
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
    // Attempting to allocate block with too large of a size.
    if (level >= LEVEL_COUNT) {
        return NULL;
    }

    // If there aren't any blocks at this level, attempt to split a block at the parent level
    if (_free_lists[level] == NULL) {
        if (level == 0) {
            return NULL;
        }

        // Get parent block
        uint8_t parent_level = level - 1;
        Header *parent_block = _find_block_to_alloc(parent_level);
        if (parent_block == NULL) {
            return NULL;
        }

        // Split parent block and add children to free list
        Header *child_block_1 = parent_block;
        Header *child_block_2 = (Header *)((char *)child_block_1 + _block_size(level));

        // push blocks to free list
        _free_list_push_back(level, child_block_1);
        _free_list_push_back(level, child_block_2);
    }

    // Block found in free list, pop and return
    Header *block = _free_list_pop_front(level);
    alloc_block_size[_block_ptr_offset((char *)block)] = _block_size(level);
    return block;
}

void *hisho_buddy__alloc(uint32_t n_bytes) {
    // System memory allocation if first time allocating
    if (_buff_base == NULL) {
        assert(LEAF_SIZE > sizeof(Header));
        _init_memory();
    }

    // Attempt to allocate new block
    uint8_t new_block_level = _block_level(n_bytes + sizeof(Header));
    assert(new_block_level >= 0);
    Header *new_block = _find_block_to_alloc(new_block_level);
    if (new_block == NULL) {
        fprintf(stderr, "Out of memory.\n");
        return NULL;
    }

    return new_block + 1; // skip the header and return a pointer to block buffer
};

void hisho_buddy__free(void *block_ptr) {
    Header *block = (Header *)block_ptr;
    uint32_t block_size = alloc_block_size[_block_ptr_offset(block_ptr)];
    uint8_t block_level = _block_level(block_size);

    // If buddy is free then merge and free the merged block, otherwise just free the block
    uint32_t block_level_index = _block_level_index(block_ptr, block_level);
    uint32_t buddy_level_index = _buddy_level_index(block_level_index);
    if (_is_block_free[_block_index(block_level, buddy_level_index)]) {
        // Buddy is free
        // Remove buddy from free list
        bool buddy_first = buddy_level_index < block_level_index;
        Header *buddy = buddy_first ? block_ptr - block_size : block_ptr + block_size;
        _free_list_remove(block_level, buddy);

        // Add merged block to free list
        Header *merge_block = buddy_first ? buddy : block;
        alloc_block_size[_block_ptr_offset((char *)merge_block)] = _block_size(block_level - 1);
        // Free merged block recursively until no buddies are left to merge with
        hisho_buddy__free((void *)merge_block);
    } else {
        // Buddy is not free,
        // Just free block
        _free_list_push_back(block_level, block);
    }
}

void hisho_buddy__print_free_lists() {
    for (int l = 0; l < LEVEL_COUNT; l++) {
        printf("Level %d: ", l);
        Header *curr = _free_lists[l];
        while (curr != NULL) {
            printf("%u ", _block_size(l));
            curr = curr->next;
        }
        printf("\n");
    }
}