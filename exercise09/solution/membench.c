#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#include "allocator.h"

size_t N = 0;
size_t S = 0;

void* thread_fn() {
    for(size_t i = 0; i < N; ++i) {
        void* ptr = my_malloc(S);
        if(ptr == NULL) {
            fprintf(stderr, "my_alloc returned NULL - out of memory?!\n");
            abort();
        }
        my_free(ptr);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if(argc != 4) {
        printf("Usage: %s <nthreads> <nallocs> <allocsize>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t num_threads = atol(argv[1]);
    assert(num_threads <= 1000);
    N = atol(argv[2]);
    assert(N <= 1e6);
    S = atol(argv[3]);
    assert(S <= (2 << 20));

    printf("Running %lu threads, each doing %lu allocations of size %lu\n", num_threads, N, S);

    pthread_t* thread_handles = calloc(num_threads, sizeof(pthread_t));
    for(size_t i = 0; i < num_threads; ++i) {
        pthread_create(&thread_handles[i], NULL, thread_fn, NULL);
    }
    for(size_t i = 0; i < num_threads; ++i) {
        pthread_join(thread_handles[i], NULL);
    }
    free(thread_handles);

    return EXIT_SUCCESS;
}

