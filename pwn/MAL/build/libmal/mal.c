#include <stdio.h>
#include <sys/mman.h>
#include "mal.h"

//#define DEBUG

#ifdef DEBUG
# define dbg(fmt, ...)  printf(fmt, __VA_ARGS__);
#else
# define dbg(fmt, ...)
#endif


typedef struct Free
{
    void *_pad;
    usize size;
    void *next;
} Free;
void *top = NULL;
Free *free_list = NULL;
usize top_size = 0;

usize size_up(usize size)
{
    if (size < 0x10)
        return 0x10;
    usize rem = size % 0x10;
    return size + 0x10 - rem;
}

void *allocate_inner(usize size)
{
    if (MALLOC_BUF_SIZE < size)
        return NULL;
    size = size_up(size) + 0x10;
    if (free_list != NULL)
    {
        Free *cur = free_list;
        Free **bef = &free_list;
        while (1)
        {
            if (cur->size == size)
            {
                *bef = cur->next;
                return (void *)((usize)cur + 0x10);
            }
            bef = (Free **)&cur->next;
            cur = cur->next;
            if (cur == NULL) break;
        }
    }
    if (top == NULL)
    {
        top = mmap(NULL, MALLOC_BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (top == MAP_FAILED)
            return NULL;
        top_size = MALLOC_BUF_SIZE;
    }
    if (top_size < size)
        return NULL;
    top_size -= size;
    Free *ptr = top;
    top = (void *)((usize)top + size);
    ptr->size = size;
    return (void *)((usize)ptr + 0x10);
}

void *allocate(usize size) {
    dbg("[all]%llx -> ", free_list);
    void *ptr = allocate_inner(size);
    dbg("%llx %llx %llx\n", ptr, size, free_list);
    return ptr;
}

void deallocate(void *ptr)
{
    Free *buf = (Free *)((usize)ptr - 0x10);
    dbg("[dea]%llx %llx -> ", ptr, buf->size);

    if ((usize)buf + buf->size == (usize)top)
    {
        top = buf;
        top_size += buf->size;
        dbg("top %llx\n", top);
        return;
    }

    dbg("old %llx ", free_list);
    buf->next = free_list;
    free_list = buf;
    dbg("list %llx\n", free_list);

}

void *reallocate(void *ptr, usize size)
{
    // _(:3 」∠ )_
    deallocate(ptr);
    return allocate(size);
}
