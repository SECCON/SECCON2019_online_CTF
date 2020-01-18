#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "private.h"

#include "util.h"

#define PRIVATE_PATH "./q/private"

void private(void){
	char filename[20] = {0};

	if(chdir(PRIVATE_PATH) == -1){
                puts("Error! Tell admin");
                exit(-1);
        }

	puts("Welcome to private directory");
	puts("You can download contents in this directory, libc, and binary");

	listing();

	puts("Input file name");

	fgets(filename,sizeof(filename),stdin);

	printf("Filename : ");
	printf(filename);

	puts("OK! Downloading...");
	download(filename);

	if(chdir(getenv("HOME")) == -1){
		puts("Error! Tell admin");
		exit(-1);
	}

}
