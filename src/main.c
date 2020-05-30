#include "main.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "hisho_first_fit_test.h"
#include "hisho_stack_test.h"

int main(int argc, char *argv[]) {
    test_hisho_stack();
    test_hisho_first_fit();
    printf("Done.\n");
    return 0;
}