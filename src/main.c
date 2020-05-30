#include "main.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "hisho_ff_test.h"
#include "hisho_s_test.h"

int main(int argc, char *argv[]) {
    test_hisho_s();
    printf("\n");
    test_hisho_ff();
    printf("Done.\n");
    return 0;
}