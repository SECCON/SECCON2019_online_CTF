#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"
#include "setup.h"
#include "public.h"

#include "login.h"
#include "private.h"


void main_loop(void);

int main(void){
	setup();

	main_loop();

	return 0;
}

void main_loop(void){
	unsigned int login_ok = 0;
	unsigned int sel;
	char input_buf[10];
	
	while(1){

		puts("1: Public contents");
		puts("2: Login");
		puts("3: Exit");
		if(login_ok){
			puts("4: Manage");
		}

		input(input_buf);
		sel = atoi(input_buf);
		switch(sel){
			case 1:
				public();
				break;
			case 2:
				login_ok = login();
				if(login_ok == 1){
					puts("Logged in!");
				}else{
					puts("Exit!");
					exit(-1);
				}
				break;
			case 3:
				puts("Thank you! Bye!");
				exit(0);
				break;
			case 4:
				if(!login_ok){
					puts("You are not logged in!");
					exit(-1);
				}

				private();
				break;
			default:
				puts("Invalid choice");
				exit(1);
				break;
		}
	}
}	
