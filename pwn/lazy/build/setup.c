#include "setup.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define TIMEOUT_SECOND 180

void setup(void){
	struct sigaction act;

	setvbuf(stdin,NULL,_IONBF,0x0);
	setvbuf(stdout,NULL,_IONBF,0x0);
	setvbuf(stderr,NULL,_IONBF,0x0);

	if(chdir(getenv("HOME")) == -1){
		puts("Error! Tell admin");
		exit(-1);
	}

	memset(&act,0x0,sizeof(act));
	act.sa_sigaction = &signal_handler;
	act.sa_flags = SA_SIGINFO;
	if(sigaction(SIGALRM,&act,NULL) < 0){
		exit(EXIT_FAILURE);
	}
	alarm(TIMEOUT_SECOND);
}

void signal_handler(int sig, siginfo_t *siginfo, void *context){
	puts("timeout!");
	exit(EXIT_FAILURE);
}
