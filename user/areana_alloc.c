#include "kernel/types.h"
#include "user/user.h"

#define ARENA_SIZE 1024

typedef struct {
    uint8 memory[ARENA_SIZE];
    uint64 offset;
} Arena;

// Initialize the arena
void arena_init(Arena* a) {
    a->offset = 0;
}

void* arena_alloc(Arena* a, uint64 size) {
    // 1. Check if there is enough space left in the arena (ARENA_SIZE - current offset).
    // 2. If not, return NULL.
    // 3. If there is, calculate the pointer to the current free memory.
    // 4. Increment the offset by 'size'.
    // 5. Return the pointer.

    if (a->offset + size > ARENA_SIZE) return 0;

    void* ptr = &a->memory[a->offset];

    a->offset += size;

    return ptr; // Replace this
}

int main() {
    Arena my_arena;
    arena_init(&my_arena);

    int* num = (int*)arena_alloc(&my_arena, sizeof(int));
    if (num) {
        *num = 42;
        printf("Allocated number: %d\n", *num);
    } else {
        printf("Allocation failed!\n");
    }

    // Try allocating more than the remaining space to test your error handling
    char* big_string = (char*)arena_alloc(&my_arena, 2048);
    if (!big_string) printf("Successfully caught out-of-memory error!\n");

    return 0;
}