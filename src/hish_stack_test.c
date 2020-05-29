#include "hisho_stack_test.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_hisho_stack() {
    // First alloc
    if (DEBUG_HISHO_STACK)
        printf("Alloc \"hisho\":\n");
    char *hisho_str = "hisho";
    char *hisho_p = hisho_stack__alloc(5);
    assert(hisho_p != NULL);
    memcpy(hisho_p, hisho_str, 5 * sizeof(char));
    if (DEBUG_HISHO_STACK)
        hisho_stack__print();

    // Second alloc
    if (DEBUG_HISHO_STACK)
        printf("Alloc \"ghutyun\":\n");
    char *ghutyun_str = "ghutyun";
    char *ghutyun_p = hisho_stack__alloc(7);
    assert(ghutyun_p != NULL);
    memcpy(ghutyun_p, ghutyun_str, 7 * sizeof(char));
    if (DEBUG_HISHO_STACK)
        hisho_stack__print();

    // Third alloc (failed)
    if (DEBUG_HISHO_STACK)
        printf("Alloc \"fail\":\n");
    char *fail_str = "fail";
    char *fail_p = hisho_stack__alloc(4);
    assert(fail_p == NULL);
    if (DEBUG_HISHO_STACK)
        hisho_stack__print();

    // Free
    if (DEBUG_HISHO_STACK)
        printf("Free \"ghutyun\":\n");
    hisho_stack__free(ghutyun_p);
    if (DEBUG_HISHO_STACK)
        hisho_stack__print();

    // Fourth alloc
    if (DEBUG_HISHO_STACK)
        printf("Alloc \" memory\":\n");
    char *memory_str = " memory";
    char *memory_p = hisho_stack__alloc(7);
    assert(memory_p != NULL);
    memcpy(memory_p, memory_str, 7 * sizeof(char));
    if (DEBUG_HISHO_STACK)
        hisho_stack__print();
    printf("Test hisho_stack done without errors.\n");
}