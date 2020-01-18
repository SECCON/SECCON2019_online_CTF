#define MALLOC_BUF_SIZE 0x200000ULL
typedef unsigned long long usize;

void *allocate(usize size);
void deallocate(void *ptr);
void *reallocate(void *ptr, usize size);