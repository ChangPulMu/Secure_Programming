#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define MAXTIMES		3

struct
{
	char data[10];
	int top;
} Stack = {{0,0,0,0,0,0,0,0,0,0}, 0};

pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;

void* Child(void* arg)
{
	int i;
	char c;
	int timeout = MAXTIMES;

	do
	{
		pthread_mutex_lock(&mutx);
		if(Stack.top > 0)
			c = Stack.data[--Stack.top];
		else
			c=0;
		pthread_mutex_unlock(&mutx);

		if(c != 0)
		{
			fputc(c, stderr);
			timeout = MAXTIMES;
		}
		else
		{
			sleep(5);
			timeout--;
		}
	}
	while(timeout>0);
}

int main(void)
{
	char c;
	pthread_t pchild;

	if(pthread_create(&pchild, 0, Child, 0) != 0)
	{
		perror("Thread");
		exit(-1);
	}
	c = 'A';

	while(c <= 'Z')
	{
		pthread_mutex_lock(&mutx);

		if(Stack.top < sizeof(Stack.data))
			Stack.data[Stack.top++] = c++;
		pthread_mutex_unlock(&mutx);
		sleep(random()%2);
	}
	pthread_join(pchild, 0);

	return 0;
}
