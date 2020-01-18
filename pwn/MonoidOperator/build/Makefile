.PHONY: all

all: monoid_operator

monoid_operator: pwn.c
	gcc -o monoid_operator pwn.c -Wl,-z,relro,-z,now -fpie -fstack-protector-strong
	strip monoid_operator
	cp monoid_operator ../files/monoid_operator
