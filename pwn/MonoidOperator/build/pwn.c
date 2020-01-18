#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <alloca.h>
#define WRITE(x) write(1,x,strlen(x))
int main(int argc, char const* argv[])
{
  char buf[2][1024];
  long nm=0;
  char* name=(char*)&nm;
  alarm(10);
  setlinebuf(stdout);
  setlinebuf(stderr);
  memset(buf,0,2*1024);
  printf("Welcome to Monoid Operator!!\n");
  while(1){
    int f=0;
    printf("What operator do you choose?\n");
    char op='\0';
    int x=scanf("%c",&op);
    if(x==0||x==EOF){
      _exit(1);
    }
    if(op=='q'){
      break;
    }
    if(op!='+'&&op!='*'&&op!='^'){
      fprintf(stderr,"No such operator is supported.Sorry.\n");
      continue;
    }
    printf("How many integers do you input?\n");
    int n=0;
    scanf("%d",&n);
    printf("Input integers.\n");
    unsigned long *a=(unsigned long *)malloc(n * sizeof(long));
    if(a==NULL){
      printf("Internal Error Occurs.\n");
      continue;
    }
    for (int i = 0; i < n; i++) {
      scanf("%lu",&a[i]);
    }
    unsigned long ans=0;
    switch (op) {
      case '+':
        for (int i = 0; i < n; i++) {
          ans+=a[i];
          asm goto ("jc %l0\n"
          :
          :
          : 
          :LOOP_END);
        }
        break;
      case '*':
        ans=1;
        for (int i = 0; i < n; i++) {
          ans*=a[i];
          asm goto ("jc %l0\n"
          :
          :
          : 
          :LOOP_END);
        }
        break;
      case '^':
        for (int i = 0; i < n; i++) {
          ans^=a[i];
        }
        break;
    }
    printf("The answer is %lu.\n",ans);
    fflush(stdout);
    free(a);
    continue;
LOOP_END:
    fprintf(stderr, "Overflow is detected.\n");
    free(a);
  }
  WRITE("Before end, please submit feed back!\n"
        "What is your name?\n");
  int n=read(0,name, 7);
  name[n]='\0';
  printf("Hi, %s.\nPlease write your feed back!\n",name);
  n=read(0,buf[0], 1023);
  buf[0][n]='\0';
  if(strchr(buf[0],(int)'n')){
    _exit(1);
  }
  sprintf(buf[1],buf[0]);
  return 0;
}
