#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MY_PORT		9999
#define MAXBUF		1024

int main(int Count, char* Strings[])
{
	int sockfd;
	struct sockaddr_in self;
	char buffer[MAXBUF];

	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("Socket");
		exit(errno);
	}

	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr*)&self, sizeof(self))!=0)
	{
		perror("socket--bind");
		exit(errno);
	}

	if(listen(sockfd, 20)!=0)
	{
		perror("socket--listen");
		exit(errno);
	}

	while(1)
	{
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen = sizeof(client_addr);

		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		send(clientfd, buffer, recv(clientfd, buffer, MAXBUF, 0), 0);

		close(clientfd);
	}

	close(sockfd);

	return 0;
}
