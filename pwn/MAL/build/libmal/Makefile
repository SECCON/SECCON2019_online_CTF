build:
	gcc -c mal.c -o mal.o
	ar rcs ../../files/libmal.a mal.o

.PHONY: build

test: test.c mal.c
	gcc mal.c test.c -o test
	./test
