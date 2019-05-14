#include <stdio.h>
#include <ctype.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int args, char **argv)
{
	struct hostent *host;
	int err, i, sock, start, end;
	char hostname[100];
	struct sockaddr_in sa;

	printf("Enter IP : ");
	gets(hostname);

	printf("Enter start port number : ");
	scanf("%d", &start);

	printf("Enter end port number : ");
	scanf("%d", &end);

	printf("\nDoing inet_addr...\n");
	strncpy((char*)&sa, "", sizeof sa);
	sa.sin_family = AF_INET;

	if(sa.sin_addr.s_addr = inet_addr(hostname)){
		printf("Done\n\n");
	}
	else{
		herror(hostname);
		exit(2);
	}

	printf("Starting the portscan loop : \n");

	for(i=start; i<=end; i++){
		sa.sin_port = htons(i);
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if(sock<0){
			perror("\nSocket");
			exit(1);
		}
		err=connect(sock, (struct sockaddr*)&sa, sizeof sa);

		if(err<0){
			fflush(stdout);
		}
		else{
			printf("%-5d open\n", i);
		}
		close(sock);
	}

	printf("\r\n");
	fflush(stdout);

	return 0;
}
