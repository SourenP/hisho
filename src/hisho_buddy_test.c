#include "hisho_buddy_test.h"

#include "hisho_buddy.h"

void test_hisho_buddy() {
    printf("[%s] Starting tests.\n", __FUNCTION__);
    hisho_buddy__alloc(66);
    hisho_buddy__print_free_lists();
    printf("[%s] Ended tests.\n", __FUNCTION__);
}