#include "hisho_ff_test.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_hisho_ff() {
    printf("[%s] Starting tests.\n", __FUNCTION__);
    hisho_ff__alloc(2);
    hisho_ff__print();
    printf("[%s] Ended tests.\n", __FUNCTION__);
}