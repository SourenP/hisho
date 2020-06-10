# Hisho

Several memory allocators in C99 written for learning purposes.

## Run

```bash
make
./hisho_test
make clean
```

## Implementations

### [WIP] Buddy Allocator

- **WIP Code**: [src/hisho_buddy.h](src/hisho_buddy.h)

### Dynamic memory allocator with 'first fit' strategy

- **Code**: [src/hisho_ff.h](src/hisho_ff.h)
- **Description**:
  - A storage allocator implemented with a linked list that allocates blocks using the 'first fit' strategy.
  - Each block has an overhead of 16 bytes for its header.
  - On free a block will coalesce with the next block if it's unused, but not the previous block.
  - All block sizes are a multiple of the block header size for alignment purposes.
  - Written with the help of resources listed in [#Resources](#resources).
- **Pros**:
  - Simple
  - Avoids external fragmentation to a certain degree
  - Memory blocks are aligned
- **Cons**:
  - 16 byte overhead due to header size (todo)
  - Internal fragmentation due to:
    - block sizes being a miltiple of the header size
  - External fragmentation due to:
    - simplicity of first fit strategy
    - coalescing only occuring with next block (todo)
  - Fixed memory expansion size set at compile time (todo)
- **Usage**:
    ```c
    // alloc
    char str[] = "hisho";
    char *p = (char *)hisho_ff__alloc(sizeof(str));
    memcpy(p, str, sizeof(str));

    // free
    hisho_ff__free(p);
    ```
- **Debug**:
    ```c
    // Print a table of all blocks and their properties
    hisho_ff__print_blocks(stdout);

    // Print stats about the allocator
    hisho_ff__print_stats(stdout);
    ```
    ```
    Blocks
            Header          Units           Size            Used            Next            Chars
            0x1059bc420     0               0               1               0x1092e6000
            0x1092e6000     1               16              1               0x1092e6030     hisho
            0x1092e6030     1020            16320           0               0x0

    Stats
            Header U        Buffer U        Free U          Total U         Total S
            2               1               1020            1023            16368
    ```
- **Todo**
  - Add functionality to coalese with prev block
  - Reduce size of header to `sizeof(size_t)` instead of `2*sizeof(size_t)`
    - Encode `is_used` into `u_size`'s first bit
  - Allow option to initalize allocator in order to set parameters like expansion size.

### Static memory allocator with stack
- **Code**: [src/hisho_s.h](src/hisho_s.h)
- **Description**:
  -  A rudimentary storage allocator that uses a LIFO queue (stack) to manage memory.
  - Free calls must be made in opposite order to the alloc calls.
  - Memory is fixed in size and stored in a static variable (data segment of virtual address space of program).
  - Written with the help of resources listed in [#Resources](#resources).
- **Pros**:
  - Very simple
  - No fragmentation since data stored sequentially
- **Cons**:
  - Fixed size that must be set at compile time
  - Can only free last allocation
  - Wastes unused memory space
- **Usage**:
    ```c
    // alloc
    char str[] = "hisho";
    char *p = hisho_s__alloc(sizeof(str));
    memcpy(p, str, sizeof(str));

    // free
    hisho_s__free(p);
    ```
- **Debug**:
    ```c
    // Print memory and head pointer
    hisho_s__print();
    ```
    ```
    [hisho       ]
    [     ^      ]
    ```

## Resources

- Books
  - [K&R: The C Programming Language. 2nd Edition](https://g.co/kgs/qax39B)
  - [Game Programming Patterns: Optimization Patterns: Data Locality](http://gameprogrammingpatterns.com/data-locality.html)
- Articles
  - [Wikipedia: Memory Management](https://en.wikipedia.org/wiki/Memory_management)
  - [Wikipedia: Buddy Memory Allocation](https://en.wikipedia.org/wiki/Buddy_memory_allocation)
  - [Wikipedia: CPU cache](https://en.wikipedia.org/wiki/CPU_cache)
  - [Data alignment: Straighten up and fly right](https://developer.ibm.com/technologies/systems/articles/pa-dalign/)
  - [Writing a memory manager](https://wiki.osdev.org/Writing_a_memory_manager)
- Blog Posts
  - [Allocation Adventures 1: The DataComponent](http://bitsquid.blogspot.com/2015/06/allocation-adventures-1-datacomponent.html)
  - [Allocation Adventures 2: Arrays of Arrays](http://bitsquid.blogspot.com/2015/06/allocation-adventures-2-arrays-of-arrays.html)
  - [Allocation Adventures 3: The Buddy Allocator](http://bitsquid.blogspot.com/2015/08/allocation-adventures-3-buddy-allocator.html)
  - [angrave/SystemProgramming: Memory, Part 1: Heap Memory Introduction](https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction)
  - [angrave/SystemProgramming: Memory, Part 2: Implementing a Memory Allocator](https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator)
  - [What a C programmer should know about memory](https://marek.vavrusa.com/memory/)
- Stack Overflow
  - [Explain this implementation of malloc from the K&R book](https://stackoverflow.com/a/36512105/2019764)
- Videos
  - [Code for Game Developers - Anatomy of a Memory Allocation](https://www.youtube.com/watch?v=c0g3S_2QxWM)
  - [Ryan Zezeski on Memory by the Slab: The Tale of Bonwick's Slab Allocator](https://paperswelove.org/2015/video/ryan-zezeski-memory-by-the-slab/)
  - [Handmade Hero Day 157 - Introduction to General Purpose Allocation](https://www.youtube.com/watch?v=MvDUe2evkHg)

## Todo

- [X] Stack memory allocator with static storage
- [X] First fit memory allocator
- [ ] Buddy memory allocator

## Namesake

"hishoghutyun" (հիշողություն) means "memory" in Armenian. Hisho is short for that :)
