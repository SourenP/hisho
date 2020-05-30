# Hisho

Several memory allocators in C99 written for learning purposes.

Status: _WIP_ (see [#Todo](#todo))

## Run

Reqs:
  - `clang`
  - `asan`

```bash
make
./hisho_test
make clean
```

## Implementations

### First fit memory allocator

- **Code**: [src/hisho_ff.h](src/hisho_ff.h)
- **Test**: [src/hisho_ff_test.c](src/hisho_ff_test.c)
- **Description**:
  - A storage allocator implemented with a linked list that allocates blocks using the 'first fit' strategy.
  - On free a block will merge with the next block if it's unused but not the previous block.
  - All block sizes are a multiple of the block header size which is set to `2*sizeof(long)` for alignment purposes.
  - Written with the help of resources listed in [#Resources](#resources).
- **Pros**:
  - Todo: add more later
- **Cons**:
  - Todo: add more later
- **Usage**:
    ```c
    // alloc
    char *p = (char *)hisho_ff__alloc(6);
    char str[] = "hisho";
    memcpy(p, str, 6);

    // free
    hisho_ff__free(p);
    ```
- **Debug**
    ```c
    // Print a table of all blocks and their properties
    hisho_ff__print_blocks();

    // Print stats about the allocator
    hisho_ff__print_stats();
    ```
    ```
    Blocks
            Header          Units   Size    Used    Next            Chars
            0x1098c32c0     0       0       1       0x10d1ed000
            0x10d1ed000     2       32      1       0x10d1ed030     hisho
            0x10d1ed030     1020    16320   0       0x0
    Stats
            Header  Buffer  Free    Total
            2       2       1020    1024
    ```

### Stack memory allocator with static storage
- **Code**: [src/hisho_s.h](src/hisho_s.h)
- **Test**: [src/hisho_s_test.c](src/hisho_s_test.c)
- **Description**:
  -  A rudimentary storage allocator that uses a LIFO queue (stack) to manage memory.
  - Free calls must be made in opposite order to the alloc calls.
  - Memory is fixed in size and stored in a static variable (data segment of virtual address space of program).
  - Written with the help of resources listed in [#Resources](#resources).
- **Pros**:
  - Very simple
  - Data stored sequentially
- **Cons**:
  - Fixed size
  - Can only free last allocation
  - Wastes unused memory space
  - Todo: add more later
- **Usage**:
    ```c
    // alloc
    char str[] = "hisho";
    char *p = hisho_s__alloc(6);
    memcpy(p, str, 6);

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

## Todo

- [ ] Stack memory allocator with static storage
  - [X] Write core code
  - [ ] Add more pros/cons
  - [ ] Improve usage example
- [ ] First fit memory allocator
  - [X] Write core code
  - [ ] Clean up code
  - [ ] Add more pros/cons
  - [ ] Improve usage example
- [ ] Buddy memory allocator
  - [ ] Write core code

## Namesake

"hishoghutyun" (հիշողություն) means "memory" in Armenian. Hisho is short for that :)
