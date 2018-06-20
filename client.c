#include "header.h"

int
main(int argc, char **argv)
{
	if (argc != 2) {
		printf("%s\n", "usage: client <IPaddress>");
		exit(1);
	}

	int			sockfd;
	struct sockaddr_in	servaddr;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT); //chargen server
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	if ( connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == 0 ) {
	  //connect function returns 0 if success

		printf("\nWelcome, you are now connected.\n");

		client_interface(sockfd);
		
	} else {
		
		printf("\nCan't connect to server. Exiting program.\n");
		
	}

	shutdown(sockfd, SHUT_RD);
	shutdown(sockfd, SHUT_WR);	//send FIN
	close(sockfd);
	
	printf("\nConnection to server closed.\n");

	exit(0);
}
