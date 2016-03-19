/*
 *    tools/build.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 *
 * Usage: build  boot.o system.bin a.img 
 */

#include <stdio.h>
#include <stdlib.h>

#define size (80*18*2*512)

int main(int argc, char **argv)
{
	FILE *boot, *sys, *img;
	char buf[512];

	if (argc != 4) {
		fprintf(stderr, "Usage: build boot system image\n");
		exit(0);
	}

	boot = fopen(argv[1], "rb");
	if (boot == NULL) {
		fprintf(stderr, "File Open Error:%s", argv[1]);
		exit(0);
	}

	sys = fopen(argv[2], "rb");
	if (sys == NULL) {
		fprintf(stderr, "File Open Error:%s", argv[2]);
		exit(0);
	}

	img = fopen(argv[3], "wb");
	if (img == NULL) {
		fprintf(stderr, "File Open Error:%s", argv[3]);
		exit(0);
	}

	fread(buf, 512, 1, boot);
	fwrite(buf, 512, 1, img);
	while (fread(&buf, 1, 512, sys)) {
		fwrite(&buf, 1, 512, img);
	}
	fseek(img, size - 1, 0);
	fputc(0, img);

	fclose(img);
}
