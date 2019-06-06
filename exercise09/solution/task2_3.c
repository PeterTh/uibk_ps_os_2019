#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>
#include <assert.h>

#define MEMORY_POOL_SIZE (2ull << 28)

typedef struct _block_list_node {
    bool is_free;
    size_t block_size;
    struct _block_list_node* next;
} block_list_node;

#ifdef PER_THREAD_ALLOCATOR
static __thread block_list_node* block_list_head = NULL;
#else
static block_list_node* block_list_head = NULL;
static pthread_mutex_t my_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

static void teardown() {
    if(block_list_head == NULL) return;
    if(block_list_head->next != NULL) {
        fprintf(stderr, "Warning: Some memory was not free'd\n");
    }
    if(block_list_head->block_size != MEMORY_POOL_SIZE - sizeof(block_list_node)) {
        fprintf(stderr, "Warning: Head block size is %lu, expected %llu\n",
            block_list_head->block_size, MEMORY_POOL_SIZE - sizeof(block_list_node));
    }
    assert(block_list_head->next != NULL ||
        block_list_head->block_size == MEMORY_POOL_SIZE - sizeof(block_list_node));
    // Free the block list head, and with it the entire memory pool
    free(block_list_head);
}

static void init() {
    // We allocate a memory pool and immediately use
    // the first few bytes as our block list head.
    block_list_head = malloc(MEMORY_POOL_SIZE);
    block_list_head->is_free = true;
    block_list_head->block_size = MEMORY_POOL_SIZE - sizeof(block_list_node);
    block_list_head->next = NULL;

    // Since we initialize the allocator automatically upon first use (see below),
    // we also have to make sure it is automatically destroyed.
    // For this we use two different mechanisms, depending on whether the
    // allocator is multi-threaded or single-threaded.
#ifdef PER_THREAD_ALLOCATOR
    pthread_key_t key;
    pthread_key_create(&key, teardown);
    pthread_setspecific(key, block_list_head);
#else
    atexit(teardown);
#endif
}

static void insert_block_node(block_list_node* after, size_t block_size) {
    block_list_node* new_node =
        (void*)after + sizeof(block_list_node) + after->block_size;
    new_node->is_free = true;
    new_node->block_size = block_size;
    new_node->next = after->next;
    after->next = new_node;
}

static void allocate(block_list_node* node, size_t size) {
    assert(node->block_size >= size);
    assert(node->is_free);

    node->is_free = false;
    size_t overhead = node->block_size - size;
    if(overhead > 0) {
        node->block_size = size - sizeof(block_list_node);
        insert_block_node(node, overhead);
    }
}

static void deallocate(block_list_node* node, block_list_node* predecessor) {
    node->is_free = true;
    // First, check if node can be merged with successor
    block_list_node* successor = node->next;
    if(successor != NULL && successor->is_free) {
        node->next = successor->next;
        node->block_size += successor->block_size + sizeof(block_list_node);
    }
    // Now try and merge node with predecessor
    if(predecessor != NULL && predecessor->is_free) {
        predecessor->next = node->next;
        predecessor->block_size += node->block_size + sizeof(block_list_node);
    }
}

void* my_malloc(size_t size) {
#ifndef PER_THREAD_ALLOCATOR
    pthread_mutex_lock(&my_malloc_mutex);
#endif
    // To avoid having the user manually initialize the
    // allocator, we simply do it on first use.
    if(block_list_head == NULL) {
        init();
    }

    // Do a best fit allocation
    block_list_node* current = block_list_head;
    block_list_node* best_fit = NULL;
    size_t best_fit_overhead = INT_MAX;
    while(current != NULL) {
        if(!current->is_free) {
            current = current->next;
            continue;
        }
        if(current->block_size >= size) {
            size_t overhead = current->block_size - size;
            if(overhead < best_fit_overhead) {
                best_fit = current;
                best_fit_overhead = overhead;
            }
            // Short circuit
            if(overhead == 0) break;
        }
        current = current->next;
    }
    if(best_fit == NULL) {
        return NULL;
    }

    allocate(best_fit, size);
#ifndef PER_THREAD_ALLOCATOR
    pthread_mutex_unlock(&my_malloc_mutex);
#endif
    return (void*)best_fit + sizeof(block_list_node);
}

void my_free(void* ptr) {
#ifndef PER_THREAD_ALLOCATOR
    pthread_mutex_lock(&my_malloc_mutex);
#endif
    // Find block with that ptr in list
    // Crucially, also determine its predecessor
    block_list_node* previous = NULL;
    block_list_node* current = block_list_head;
    while(current != NULL) {
        if(current == ptr - sizeof(block_list_node)) {
            deallocate(current, previous);
            break;
        }
        previous = current;
        current = current->next;
    }
    if(current != ptr - sizeof(block_list_node)) {
        fprintf(stderr, "Invalid my_free for address %p\n", ptr);
    }
#ifndef PER_THREAD_ALLOCATOR
    pthread_mutex_unlock(&my_malloc_mutex);
#endif
}

