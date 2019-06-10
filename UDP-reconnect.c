#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>

int main()
{
	int sd;
	struct sockaddr_in addr;
	int addr_len = sizeof(addr);
	char buffer[1024];

	sd = socket(PF_INET, SOCK_DGRAM, 0);
	
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	inet_aton("127.0.0.1", &addr.sin_addr);

	if(connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
		perror("connect 1");

	send(sd, "sean\n", 5, 0);

	bzero(buffer, sizeof(buffer));

	recv(sd, buffer, sizeof(buffer), 0);

	printf("%s\n", buffer);

	addr.sin_port = htons(10000);

	if(connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
		perror("connect 2");

	send(sd, "GET / HTTP/1.0\n\n", 16, 0);

	bzero(buffer, sizeof(buffer));

	recv(sd, buffer, sizeof(buffer), 0);

	printf("%s\n", buffer);

	close(sd);
}
