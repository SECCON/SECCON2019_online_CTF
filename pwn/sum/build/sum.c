#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE 5
#define ptr_index BUFSIZE
#define result_index (BUFSIZE + 1)


int sum(long long *l, long long *ret) {
    *ret = 0;
    int i = 0;
    while(l[i] != 0) {
        *ret += l[i++];
    }
    return i;
}

void read_ints(long long *l, long long size) {
    for (long long i = 0; i <= size; i++) {
        if (scanf("%lld", l + i) != 1) {
            exit(-1);
        }
        if (l[i] == 0) return;
    }
}

static void setup(void) __attribute__((constructor));

void setup() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    alarm(30);
}

/*
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21
*/
int main(void) {
    long long l[BUFSIZE + 2] = {0};
    l[ptr_index] = (long long)&l[result_index];

    printf("[sum system]\nInput numbers except for 0.\n0 is interpreted as the end of sequence.\n\n");
    printf("[Example]\n3 4 0\n");
    read_ints(l, BUFSIZE);
    if (sum(l, (long long *)l[ptr_index]) > BUFSIZE) {
        exit(-1);
    }
    printf("%llu\n", l[result_index]);
    return 0;
}
