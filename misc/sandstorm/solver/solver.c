#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <math.h>

#include "pngle.h"

uint8_t *img;
int width, height;

#define UNUSED(x) (void)(x)

void put_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t *p = img + (y * width + x) * 3;

	p[0] = r;
	p[1] = g;
	p[2] = b;
}

void rectangle(int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b)
{
	int ix, iy;

	for (iy = 0; iy < h; iy++) {
		for (ix = 0; ix < w; ix++) {
			put_pixel(x + ix, y + iy, r, g, b);
		}
	}
}

void draw_pixel(pngle_t *pngle, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t rgba[4])
{
	UNUSED(pngle);
	UNUSED(w);
	UNUSED(h);
	uint8_t r = rgba[0];
	uint8_t g = rgba[1];
	uint8_t b = rgba[2];
	// uint8_t a = rgba[3]; // ignore

	// XXX: solver! extract the 1st interlaced frame
	if (w == 8 && h == 8) {
		rectangle(x, y, w, h, r, g, b);
	}
}

void init_screen(pngle_t *pngle, uint32_t w, uint32_t h)
{
	UNUSED(pngle);
	width = w;
	height = h;

	img = calloc(width * height, 3);
}

void flush_screen(pngle_t *pngle) {
	UNUSED(pngle);
	printf("P6\n");
	printf("%u %u\n", width, height);
	printf("255\n");

	fwrite(img, width * height, 3, stdout);
}

int main(int argc, char *argv[])
{
	FILE *fp = stdin;
	char buf[1024];
	size_t remain = 0;
	int len;

	pngle_t *pngle = pngle_new();

	pngle_set_init_callback(pngle, init_screen);
	pngle_set_draw_callback(pngle, draw_pixel);

	while (!feof(fp)) {
		if (remain >= sizeof(buf)) {
			if (argc > 1) fprintf(stderr, "%s: ", argv[1]);
			fprintf(stderr, "Buffer exceeded\n");
			return -1;
		}

		len = fread(buf + remain, 1, sizeof(buf) - remain, fp);
		if (len <= 0) {
			//printf("EOF\n");
			break;
		}

		int fed = pngle_feed(pngle, buf, remain + len);
		if (fed < 0) {
			if (argc > 1) fprintf(stderr, "%s: ", argv[1]);
			fprintf(stderr, "ERROR; %s\n", pngle_error(pngle));
			return -1;
		}

		remain = remain + len - fed;
		if (remain > 0) memmove(buf, buf + fed, remain);
	}

	flush_screen(pngle);

	return 0;
}
