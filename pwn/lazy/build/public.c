#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "public.h"

#include "util.h"

#define PUBLIC_PATH "./q/public"

void public(void){
	char filename[20];

	if(chdir(PUBLIC_PATH) == -1){
		puts("Error! Tell admin");
		exit(-1);
	}

	puts("Welcome to public directory");
	puts("You can download contents in this directory");

	listing();

	fgets(filename,sizeof(filename),stdin);

	download(filename);

	if(chdir(getenv("HOME")) == -1){
		puts("Error! Tell admin");
		exit(-1);
	}

}
