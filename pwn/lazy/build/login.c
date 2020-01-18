#include <stdio.h>
#include <string.h>
#include "login.h"
#include "util.h"

#define BUFFER_LENGTH 32
#define PASSWORD "3XPL01717"
#define USERNAME "_H4CK3R_"

int login(void){
	char username[BUFFER_LENGTH];
	char password[BUFFER_LENGTH];
	char input_username[BUFFER_LENGTH];
	char input_password[BUFFER_LENGTH];

	memset(username,0x0,BUFFER_LENGTH);
	memset(password,0x0,BUFFER_LENGTH);
	memset(input_username,0x0,BUFFER_LENGTH);
	memset(input_password,0x0,BUFFER_LENGTH);

	strcpy(username,USERNAME);
	strcpy(password,PASSWORD);

	printf("username : ");
	input(input_username);
	printf("Welcome, %s\n",input_username);

	printf("password : ");
	input(input_password);


	if(strncmp(username,input_username,strlen(USERNAME)) != 0){
		puts("Invalid username");
		return 0;
	}

	if(strncmp(password,input_password,strlen(PASSWORD)) != 0){
		puts("Invalid password");
		return 0;
	}

	return 1;
}

