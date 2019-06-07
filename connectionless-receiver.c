#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <ctype.h>

#define BUFSIZE		1024

void alltoupper(char* s)
{
	while(*s != 0)
		*s++ = toupper(*s);
}

int main()
{
	char buffer[BUFSIZE];
	struct sockaddr_in addr;
	int sd, addr_size, bytes_read;

	sd = socket(PF_INET, SOCK_DGRAM, 0);	// SOCK_DGRAM : UDP

	if(sd<0)
	{
		perror("socket");
		abort();
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		abort();
	}

	do
	{
		bzero(buffer, BUFSIZE);
		addr_size = BUFSIZE;

		bytes_read = recvfrom(sd, buffer, BUFSIZE, 0, (struct sockaddr*)&addr, &addr_size);

		if(bytes_read > 0)
		{
			printf("Connect: %s:%d \"%s\"\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), buffer);
			alltoupper(buffer);

			if(sendto(sd, buffer, bytes_read, 0, (struct sockaddr*)&addr, addr_size) < 0)
			perror("reply");
		}
		else
			perror("recvfrom");
	}
	while(bytes_read > 0);

	close(sd);

	return 0;
}
		
