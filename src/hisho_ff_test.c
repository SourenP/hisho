#include "hisho_ff_test.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_hisho_ff() {
    printf("[%s] Starting tests.\n", __FUNCTION__);

    // alloc
    char *p = (char *)hisho_ff__alloc(6);
    char str[] = "hisho";
    memcpy(p, str, 6);

    // debug print
    if (DEBUG_HISHO_FF_TEST) {
        hisho_ff__print_blocks();
        hisho_ff__print_stats();
    }

    // free
    hisho_ff__free(p);

    // debug print
    if (DEBUG_HISHO_FF_TEST) {
        hisho_ff__print_blocks();
        hisho_ff__print_stats();
    }

    printf("[%s] Ended tests.\n", __FUNCTION__);
}