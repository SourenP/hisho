#include "hisho_ff_test.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_hisho_ff() {
    printf("[%s] Starting tests.\n", __FUNCTION__);

    // first alloc
    char str_1[] = "hisho";
    char *p_1 = (char *)hisho_ff__alloc(sizeof(str_1));
    memcpy(p_1, str_1, sizeof(str_1));

    // debug print
    if (DEBUG_HISHO_FF_TEST) {
        hisho_ff__print_blocks(stdout);
        hisho_ff__print_stats(stdout);
    }

    // second alloc
    char str_2[] = "ghutyun";
    char *p_2 = (char *)hisho_ff__alloc(sizeof(str_2));
    memcpy(p_2, str_2, sizeof(str_2));

    // debug print
    if (DEBUG_HISHO_FF_TEST) {
        hisho_ff__print_blocks(stdout);
        hisho_ff__print_stats(stdout);
    }

    // third alloc
    char str_3[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor";
    char *p_3 = (char *)hisho_ff__alloc(sizeof(str_3));
    memcpy(p_3, str_3, sizeof(str_3));

    // debug print
    if (DEBUG_HISHO_FF_TEST) {
        hisho_ff__print_blocks(stdout);
        hisho_ff__print_stats(stdout);
    }

    // free second
    hisho_ff__free(p_2);

    // debug print
    if (DEBUG_HISHO_FF_TEST) {
        hisho_ff__print_blocks(stdout);
        hisho_ff__print_stats(stdout);
    }

    // free first
    hisho_ff__free(p_1);

    // debug print
    if (DEBUG_HISHO_FF_TEST) {
        hisho_ff__print_blocks(stdout);
        hisho_ff__print_stats(stdout);
    }

    // free third
    hisho_ff__free(p_3);

    // debug print
    if (DEBUG_HISHO_FF_TEST) {
        hisho_ff__print_blocks(stdout);
        hisho_ff__print_stats(stdout);
    }

    printf("[%s] Ended tests.\n", __FUNCTION__);
}