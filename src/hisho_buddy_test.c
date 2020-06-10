#include "hisho_buddy_test.h"

#include "hisho_buddy.h"

void test_hisho_buddy() {
    // Recreate https://en.wikipedia.org/wiki/Buddy_memory_allocation#Example
    printf("[%s] Starting tests.\n", __FUNCTION__);

    printf("2: Program A requests memory 34 K\n");
    void *a = hisho_buddy__alloc(34);
    hisho_buddy__print_free_lists(); // 4, 3, 2, 1

    printf("3: Program B requests memory 66 K\n");
    void *b = hisho_buddy__alloc(66);
    hisho_buddy__print_free_lists(); // 4, 2, 1

    printf("4: Program C requests memory 35 K\n");
    void *c = hisho_buddy__alloc(35);
    hisho_buddy__print_free_lists(); // 2, 1

    printf("5: Program D requests memory 67 K\n");
    void *d = hisho_buddy__alloc(67);
    hisho_buddy__print_free_lists(); // 3, 1

    printf("6: Program B releases memory\n");
    hisho_buddy__free(b);
    hisho_buddy__print_free_lists(); // 3, 3, 1

    printf("7: Program D releases memory\n");
    hisho_buddy__free(d);
    hisho_buddy__print_free_lists(); // 3, 2, 1

    printf("8: Program A releases memory\n");
    hisho_buddy__free(a);
    hisho_buddy__print_free_lists(); // 4, 3, 2, 1

    printf("9: Program C releases memory\n");
    hisho_buddy__free(c);
    hisho_buddy__print_free_lists(); // 0

    printf("[%s] Ended tests.\n", __FUNCTION__);
}