CFLAGS+=-Os -Wall

all: solve

solver: solver.c pngle.c miniz.c miniz.h Makefile
	$(CC) $(CFLAGS) -o solver solver.c pngle.c miniz.c -lm

solve: flag.png
	zbarimg flag.png

flag.png: solver
	./solver < ../files/sandstorm.png | pnmtopng > flag.png

clean:
	rm -f solver flag.png
