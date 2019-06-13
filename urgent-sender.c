#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>

int main()
{
	int sockfd;
	struct sockaddr_in addr;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);

	inet_aton("127.0.0.1", &addr.sin_addr);

	if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
		perror("connect");

	printf("bytes written = %zu (normal)\n", send(sockfd, "ABCDE", 6, 0));
	sleep(5);
	printf("bytes written = %zu (OOB)\n", send(sockfd, "A", 1, MSG_OOB));
	sleep(1);
	printf("bytes written = %zu (normal)\n", send(sockfd, "12345678", 9, 0));
	sleep(5);
	printf("bytes written = %zu (OOB)\n", send(sockfd, "B", 1, 0));
	sleep(1);

	close(sockfd);

	return 0;
}
