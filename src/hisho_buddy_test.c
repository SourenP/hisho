#include "hisho_buddy_test.h"

#include "hisho_buddy.h"

void test_hisho_buddy() {
    printf("[%s] Starting tests.\n", __FUNCTION__);
    void *p1 = hisho_buddy__alloc(66);
    void *p2 = hisho_buddy__alloc(66);
    void *p3 = hisho_buddy__alloc(1);
    void *p4 = hisho_buddy__alloc(25);
    hisho_buddy__print_free_lists();
    hisho_buddy__free(p1);
    hisho_buddy__free(p2);
    hisho_buddy__free(p3);
    hisho_buddy__free(p4);
    printf("[%s] Ended tests.\n", __FUNCTION__);
}