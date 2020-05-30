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
  - Written with the help of resources listed in [Resources](#resources).
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

    // debug print
    hisho_ff__print_blocks();
    hisho_ff__print_stats();

    // free
    hisho_ff__free(p);

    // debug print
    hisho_ff__print_blocks();
    hisho_ff__print_stats();
    ```
    *stdout*
    ```
    Blocks
            Header          Units   Size    Used    Next            Chars
            0x1098c32c0     0       0       1       0x10d1ed000
            0x10d1ed000     2       32      1       0x10d1ed030     hisho
            0x10d1ed030     1020    16320   0       0x0

    Stats
            Header  Buffer  Free    Total
            2       2       1020    1024

    Blocks
            Header          Units   Size    Used    Next            Chars
            0x1098c32c0     0       0       1       0x10d1ed000
            0x10d1ed000     1023    16368   0       0x0             hisho�

    Stats
            Header  Buffer  Free    Total
            1       0       1023    1024
    ```

### Stack memory allocator with static storage
- **Code**: [src/hisho_s.h](src/hisho_s.h)
- **Test**: [src/hisho_s_test.c](src/hisho_s_test.c)
- **Description**:
  -  A rudimentary storage allocator that uses a LIFO queue (stack) to manage memory.
  - Free calls must be made in opposite order to the alloc calls.
  - Memory is fixed in size and stored in a static variable (data segment of virtual address space of program).
  - See README.md for resrouces used to write this.
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

    // debug print
    hisho_s__print();

    // free
    hisho_s__free(p);

    // debug print
    hisho_s__print();
    ```
    *stdout*
    ```
    [hisho       ]
    [     ^      ]

    [hisho       ]
    [            ]
    ```

## Resources

- K&R
  - [The C Programming Language. 2nd Edition](https://g.co/kgs/qax39B)
- [bitsquid.blogspot.com](http://bitsquid.blogspot.com/)
  - [Allocation Adventures 1: The DataComponent](http://bitsquid.blogspot.com/2015/06/allocation-adventures-1-datacomponent.html)
  - [Allocation Adventures 2: Arrays of Arrays](http://bitsquid.blogspot.com/2015/06/allocation-adventures-2-arrays-of-arrays.html)
  - [Allocation Adventures 3: The Buddy Allocator](http://bitsquid.blogspot.com/2015/08/allocation-adventures-3-buddy-allocator.html)
- [github.com](https://github.com)
  - [angrave/SystemProgramming: Memory, Part 1: Heap Memory Introduction](https://github.com/angrave/SystemProgramming/wiki/Memory,-Part-1:-Heap-Memory-Introduction)
  - [angrave/SystemProgramming: Memory, Part 2: Implementing a Memory Allocator](https://github.com/angrave/SystemProgramming/wiki/Memory%2C-Part-2%3A-Implementing-a-Memory-Allocator)
- [wikipedia.org](https://en.wikipedia.org/wiki/Main_Page)
  - [Memory Management](https://en.wikipedia.org/wiki/Memory_management)
  - [Buddy Memory Allocation](https://en.wikipedia.org/wiki/Buddy_memory_allocation)
  - [CPU cache](https://en.wikipedia.org/wiki/CPU_cache)
- [stackoverflow.com](https://stackoverflow.com)
  - [drpritch: Explain this implementation of malloc from the K&R book](https://stackoverflow.com/a/36512105/2019764)
- [osdev.org](https://wiki.osdev.org/Main_Page)
  - [Writing a memory manager](https://wiki.osdev.org/Writing_a_memory_manager)
- [paperswelove.org](https://paperswelove.org/)
  - [Ryan Zezeski on Memory by the Slab: The Tale of Bonwick's Slab Allocator](https://paperswelove.org/2015/video/ryan-zezeski-memory-by-the-slab/)
- [arjunsreedharan.org](https://arjunsreedharan.org/)
  - [Memory Allocators 101 - Write a simple memory allocator](https://arjunsreedharan.org/post/148675821737/memory-allocators-101-write-a-simple-memory)
- [marek.vavrusa.com](https://marek.vavrusa.com/)
  - [What a C programmer should know about memory](https://marek.vavrusa.com/memory/)
- [developer.ibm.com](https://developer.ibm.com/)
  - [Data alignment: Straighten up and fly right](https://developer.ibm.com/technologies/systems/articles/pa-dalign/)

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
