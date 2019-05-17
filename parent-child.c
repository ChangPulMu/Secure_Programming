#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	int pchild;

	if((pchild = fork()) == 0)
	{
		int i;
		for(i=0;i<10;i++)
		{
			printf("Child #%d\n",i);
			sleep(1);
		}
		exit(500);
	}
	else if(pchild>0)
	{
		int i, result;
		for(i=0;i<7;i++)
		{
			printf("Parent #%d\n", i);
			sleep(1);
		}
		printf("awaiting child's termination...\n");
		wait(&result);
		printf("Child's result=%d\n", result);
	}
	else
		perror("Fork failure");
	exit(0);
}
