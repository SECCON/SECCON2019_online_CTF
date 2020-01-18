// gcc one.c -Wl,-z,relro,-z,now -fPIE -pie -o one
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE	128
#define MEMO_SIZE	64

static int menu(void);
static void add(void);
static void show(void);
static void delete(void);

static int getnline(char *buf, int len);
static int getint(void);

char *memo;

__attribute__((constructor))
int init(){
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	return 0;
}

int main(void){
	printf("just one");
	for(;;){
		switch(menu()){
			case 1:
				add();
				break;
			case 2:
				show();
				break;
			case 3:
				delete();
				break;
			case 0:
				goto end;
			default:
				puts("Wrong input.");
				continue;
		}
		puts("Done.");
	}

end:
	puts("Bye!");

	return 0;
}

static int menu(void){
	printf(	"\nMENU\n"
			"================\n"
			"1. Add\n"
			"2. Show\n"
			"3. Delete\n"
			"0. Exit\n"
			"================\n"
			"> ");

	return getint();
}

static void add(void){
	if(!(memo = (char*)malloc(MEMO_SIZE))){
		puts("malloc failed...");
		return;
	}


	printf("Input memo > ");
	getnline(memo, MEMO_SIZE);
}

static void show(void){
	if(!memo){
		puts("Entry does not exist...");
		return;
	}

	puts(memo);
}

static void delete(void){
	if(!memo){
		puts("Entry does not exist...");
		return;
	}

	free(memo);
}

static int getnline(char *buf, int size){
	char *lf;

	if(size < 0)
		return 0;

	fgets(buf, size, stdin);
	if((lf=strchr(buf,'\n')))
		*lf='\0';

	return 1;
}

static int getint(void){
	char buf[BUF_SIZE];

	getnline(buf, sizeof(buf));
	return atoi(buf);
}
