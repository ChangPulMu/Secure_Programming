#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MY_PORT		9999

int main()
{
	int sd;
	struct sockaddr_in self;

	if((sd=socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("Socket");
		exit(errno);
	}

	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_addr.s_addr = INADDR_ANY;
	self.sin_port = htons(MY_PORT);

	if(bind(sd, (struct sockaddr *)&self, sizeof(self)) != 0)
	{
		perror("Bind");
		abort();
	}

	if(listen(sd, 20) != 0)
	{
		perror("Listen");
		abort();
	}

	while(1)
	{
		static int client;
		int sd_copy, size;
		struct sockaddr_in client_info;

		size = sizeof(client_info);
		client = accept(sd, (struct sockaddr *)&client_info, &size);
		printf("Connected : %s : %d\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));

		if(fork()!=0)
		{
			if(close(client)<0)
				perror("Close");
		}
		else
		{
			if(close(sd)<0)
				perror("Client--close listener");

			if(dup2(client, 0)<0)
				perror("Dup stdin");
			if(dup2(client, 1)<0)
				perror("Dup stdout");
			if(dup2(client, 2)<0)
				perror("Dup stderr");

			execl("/bin/ls", "/bin/ls", "-al", "/sbin", (char *)0);
		}
	}

	close(sd);
	return 0;
}
