#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXBUF 1024

void PANIC(char *msg);

#define PANIC(msg) {perror(msg); abort();}

int main(int Count, char *Strings[])
{
	int sockfd, bytes_read;
	struct sockaddr_in dest;
	char buffer[MAXBUF];

	if(Count!=3)
		PANIC("usage: http-client <IP-addr> <send-msg>\n");
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		PANIC("Socket");

	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(80); /*default HTTP Server port*/
	if(inet_aton(Strings[1], &dest.sin_addr.s_addr)==0)
		PANIC(Strings[1]);

	if(connect(sockfd, (struct sockaddr*)&dest, sizeof(dest))!=0)
		PANIC("Connect");

	sprintf(buffer, "GET %s HTTP/1.0\r\n\r\n", Strings[2]);
	send(sockfd, buffer, strlen(buffer), 0);

	do
	{
		bzero(buffer, sizeof(buffer));
		bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
		if(bytes_read>0)
			printf("%s", buffer);
	}
	while(bytes_read>0);

	close(sockfd);

	return 0;
}
