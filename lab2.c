#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
	int pid = fork();
	if( pid > 0 ){
	while (1);
		}
	else if (pid == 0){
		int ret;
		ret = execlp("vi","",NULL);
		if (ret == -1){
		printf("execl error/n");	
		exit(-1);
		}
	}
	else {
	printf("fork error\n");
	exit(-1);
	}
	return 0;
}