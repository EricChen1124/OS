#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"

int main()
{
pid_t p1,p2,p3,p4,p5;
while((p1=fork()) == -1);
if(!p1)
{
  while((p2=fork()) == -1);
  if(!p2)
    {
        while((p3=fork())==-1);
        if (!p3)
       {
             while(1){
              printf("P4���̺ţ�%d�������̺ţ�%d\n",getpid(),getppid());}
       }
       else
      {
           while ((p4=fork())==-1);
           if (!p4)
          {
               while(1){
                 printf("P5���̺ţ�%d�������̺ţ�%d\n",getpid(),getppid());}
          }
       }
      while(1){
        printf("P2���̺ţ�%d�������̺ţ�%d\n",getpid(),getppid());}
   }
 else
 {
  while ((p5=fork())==-1);
           if (!p5)
          {
               while(1){
                 printf("P3���̺ţ�%d�������̺ţ�%d\n",getpid(),getppid());}
          }
 }
   while(1){
    printf("P1���̺ţ�%d�������̺ţ�%d\n",getpid(),getppid());}
}

return 0;
}