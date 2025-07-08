#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

static int call_count = 0;

void *malloc(size_t size) {
    static void* (*real_malloc)(size_t) = NULL;

    if (!real_malloc) {
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        if (!real_malloc) {
            fprintf(stderr, "Error in dlsym\n");
            return NULL;
        }
    }

    call_count++;
    if (call_count == 1) {
        return real_malloc(size);
    }
    return NULL;
}
// gcc -shared -fPIC malloc.c -ldl 

// LD_PRELOAD=./a.out ./your program