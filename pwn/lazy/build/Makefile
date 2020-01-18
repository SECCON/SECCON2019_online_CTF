CC = gcc
CFLAGS_BINARY = -Wl,-z,relro,-z,now -fstack-protector

CFLAGS_BOF = -fno-stack-protector
CFLAGS_OTHER = -fstack-protector

OBJS = util.o main.o setup.o login.o private.o public.o

lazy: $(OBJS)
	$(CC) $(CFLAGS_BINARY) -o $@ $(OBJS) -Wall -g

main.o: main.c
	$(CC) -c $< $(CFLAGS_OTHER)

util.o: util.c
	$(CC) -c $< $(CFLAGS_OTHER)

setup.o: setup.c
	$(CC) -c $< $(CFLAGS_OTHER)

login.o: login.c
	$(CC) -c $< $(CFLAGS_BOF)

private.o: private.c
	$(CC) -c $< $(CFLAGS_OTHER)

public.o: public.c
	$(CC) -c $< $(CFLAGS_OTHER)


clean:
	$(RM) lazy $(OBJS)
	
