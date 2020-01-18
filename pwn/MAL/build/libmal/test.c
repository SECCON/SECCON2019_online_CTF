#include <stdio.h>
#include <assert.h>
#include "mal.h"

int main(void)
{
	void *a, *b, *c, *d;
	a = allocate(10);
	deallocate(a);
	assert(allocate(10) == a);
	deallocate(a);

	assert(allocate(MALLOC_BUF_SIZE * 2) == 0);

	a = allocate(10);
	deallocate(allocate(20));
	b = allocate(10);
	deallocate(a);
	assert(a == allocate(10));
	deallocate(b);
	deallocate(a);

	assert(allocate(40) == a);
    deallocate(a);

    a = allocate(10);
    b = allocate(20);
    c = allocate(30);
    deallocate(a);
    deallocate(b);

    assert(allocate(10) == a);
    assert(allocate(20) == b);
    assert(allocate(10) != a);

	return 0;
}
