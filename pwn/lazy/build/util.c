#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "util.h"


void input(char *buf){
	int recv;
	int i = 0;
	while(1){
		recv = (int)read(STDIN_FILENO,&buf[i],1);
		if(recv == -1){
			puts("ERROR!");
			exit(-1);
		}
		if(buf[i] == '\n'){
			return;
		}
		i++;
	}
}


void listing(void){
	DIR *dir;
	struct dirent *dp;

	dir = opendir(".");

	if(dir == NULL){
		puts("Error! Tell admin");
		exit(-1);
	}

	for(dp = readdir(dir); dp != NULL; dp=readdir(dir)){
		if(strcmp(".",dp->d_name) == 0){
			continue;
		}
		if(strcmp("..",dp->d_name) == 0){
			continue;
		}

		printf("%s\n",dp->d_name);
	}

	closedir(dir);
}

void download(char *filename){
	//FILE *fp;
	char filename_buf[30];
	long filesize;
	char *send_buffer;
	long i;
	struct stat st;

	if(strlen(filename) > sizeof(filename_buf) - (strlen("./") + 1)){
		puts("Too long!");
		exit(-1);
	}

	if(strstr(filename,"/") != NULL){
		puts("No directory traversal!");
		exit(-1);
	}

	memset(filename_buf,0x0,30);
	strcpy(filename_buf,"./");
	strncat(filename_buf,filename,strlen(filename) - 1);
	int fd = open(filename_buf, O_RDONLY);

	puts(filename_buf);

	if(fd == -1){
		puts("No such file!");
		return;
	}


	if(fstat(fd,&st) == -1){
		puts("Error! Exiting...");
		exit(-1);
	}

	filesize = st.st_size;

	send_buffer = (char *)malloc(filesize);
	memset(send_buffer,0x0,sizeof(char) * filesize);

	read(fd,send_buffer,filesize);

	printf("Sending %ld bytes",filesize);

	for(i = 0; i < filesize; i++){
		write(1,&send_buffer[i],1);
	}
	
	exit(0);
}
