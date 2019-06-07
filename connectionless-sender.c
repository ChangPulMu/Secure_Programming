#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>

#define BUFSIZE		1024

int main(int Count, char* Strings[])
{
	char buffer[BUFSIZE];
	struct sockaddr_in addr;
	int sd, addr_size;

	if(Count != 2)
	{
		printf("usage: %s <msg>\n", Strings[0]);
		exit(0);
	}

	if((sd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Socket");
		abort();
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);

	if(inet_aton("127.0.0.1", &addr.sin_addr) == 0)
	{
		perror("127.0.0.1");
		abort();
	}

	sendto(sd, Strings[1], strlen(Strings[1])+1, 0, (struct sockaddr*)&addr, sizeof(addr));
	bzero(buffer, BUFSIZE);
	addr_size = sizeof(addr);

	if(recvfrom(sd, buffer, BUFSIZE, 0, (struct sockaddr*)&addr, &addr_size) < 0)
		perror("server reply");
	else
		printf("Reply: %s:%d \"%s\"\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buffer);

	close(sd);
	
	return 0;
}
