#include "hisho_first_fit_test.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_hisho_first_fit() {
    hisho_first_fit__alloc(2);
    hisho_fit_first__print();
    printf("Test hisho_first_fit done without errors.\n");
}