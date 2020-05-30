#include "hisho_ff_test.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_hisho_ff() {
    printf("[%s] Starting tests.\n", __FUNCTION__);

    char *p = (char *)hisho_ff__alloc(5);
    char *p_str = "hello";
    memcpy(p, p_str, 5);
    hisho_ff__print_blocks();

    char *x = hisho_ff__alloc(10);
    char *x_str = "there bear";
    memcpy(x, x_str, 10);
    printf("\n");
    hisho_ff__print_blocks();

    hisho_ff__free(x);
    printf("\n");
    hisho_ff__print_blocks();

    hisho_ff__free(p);
    printf("\n");
    hisho_ff__print_blocks();

    printf("\n");
    hisho_ff__print_stats();

    printf("[%s] Ended tests.\n", __FUNCTION__);
}