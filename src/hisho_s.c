#include "hisho_s.h"

#include <stdlib.h>

char *hisho_s__alloc(int n) {
    if (alloc_next + n > alloc_buffer + ALLOC_BUFFER_SIZE) {
        if (DEBUG_HISHO_STACK)
            printf("Failed to allocate. Out of memory.\n");
        return NULL;
    }
    alloc_next += n;
    return alloc_next - n;
}

void hisho_s__free(char *p) {
    if (p >= alloc_buffer && p < alloc_buffer + ALLOC_BUFFER_SIZE) {
        alloc_next = p;
    }
}

void hisho_s__print() {
    char *a = alloc_buffer;
    printf("[");
    while (a < alloc_buffer + ALLOC_BUFFER_SIZE) {
        printf("%c", (*a == '\0') ? ' ' : *a);
        a++;
    }
    a = alloc_buffer;
    printf("]\n[");
    while (a++ < alloc_buffer + ALLOC_BUFFER_SIZE) {
        printf("%c", a == alloc_next ? '^' : ' ');
    }
    printf("]\n");
    printf("\n");
}