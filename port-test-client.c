#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXBUF 1024

int main(int Count, char *Strings[])
{
	int sockfd, bytes_read;
	struct sockaddr_in dest;
	char buffer[MAXBUF];

	if(Count<3||Count>4)
	{
		fprintf(stderr, "usage : %s <IP-addr> <port> [<send-msg>]\n", Strings[0]);
		exit(0);
	}

	if((sockfd=socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("Socket");
		exit(errno);
	}

	bzero(&dest, sizeof(dest));

	dest.sin_family = AF_INET;

	if(inet_addr(Strings[1], &dest.sin_addr.s_addr)==0)
	{
		perror(Strings[1]);
		exit(errno);
	}

	dest.sin_port = htons(atoi(Strings[2]));

	if(connect(sockfd, (struct sockaddr *)&dest, sizeof(dest))!=0)
	{
		perror("Connect");
		exit(errno);
	}

	if(Count==4)
	{
		sprintf(buffer, "%s\n", Strings[3]);
		send(sockfd, buffer, strlen(buffer), 0);
	}

	do
	{
		bzero(buffer, MAXBUF);
		bytes_read = recv(sockfd, buffer, MAXBUF, 0);
		if(bytes_read>0)
			printf("%s", buffer);
	}
	while (bytes_read>0);

	close(sockfd);

	return 0;
}
