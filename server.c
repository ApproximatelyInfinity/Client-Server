#include	"header.h"
#include	<netinet/tcp.h>		/* for TCP_MAXSEG value */

int
main(int argc, char **argv)
{
	//Initalization of the mysql connection
	con = mysql_init(NULL);
	
	//memory for rec strings
	char first[MAXLEN];
	char mid[MAXLEN];
	char last[MAXLEN];
	char addr[MAXLEN];
	char phone[MAXLEN];
	char dob[MAXLEN];
	char ethnic[MAXLEN];
	char relig[MAXLEN];
	char allerg[MAXLEN];
	char smoker[MAXLEN];
	char medhist[MAXLEN];
	
	//create and init rec
	struct Record rec = {
		0, //filenumber (will be assigned by server)
		first,mid,last,addr,phone,dob,
		0,0, //height, weight
		ethnic,relig,allerg,smoker,medhist
	};
	
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	ssize_t				n, len;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	listen(listenfd, LISTENQ);

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	
	for ( ; ; ) {
		rset = allset;		/* structure assignment */
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE) {
				//err_quit("too many clients");
				printf("%s", "too many clients");
				exit(1);
			}

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) { //sockfd is readable
			
				int action, status;
				char msg[MAXLINE], temp[MAXLINE], header[HEADLEN+1];
			
				//read HEADLEN bytes header data
				if ( (n = read_socket(sockfd, header, HEADLEN)) == 0 ) {
					//client closed connection
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
					process_message(sockfd, DISCON, &rec); //dissociate sockfd from database
				} else if (n != -1) { //process data
					//success getting header data
					
					sscanf(header, "%ld", &len); //retrieve msg length from header
					
					//read len bytes data and process
					if ( (n = read_socket(sockfd, msg, len)) > 0 ) {
						//received data
						
						deserialize_int(&action, msg, "<ac>", "</ac>"); //deserialize action
						deserialize_rec(msg, &rec); //deserialize rec
						
						status = process_message(sockfd, action, &rec); //process client request
						
						msg[0] = '\0';
						sprintf(msg, "<st>%d</st>", status); //serialize status
						
						//if action=REQUEST and record is available (status=1) then serialize rec
						if(action == REQUEST && status == 1) {
							serialize_rec(temp, &rec); //serialize rec
							strcat(msg, temp);
						}
						
						create_header(header, HEADLEN, msg); //create header for msg
						strcpy(buf, header); //put header in buf
						strcat(buf, msg); //put msg in buf
						
						writen(sockfd, buf, strlen(buf));
						
					}
					
				} //end process data

				if (--nready <= 0)
					break;				/* no more readable descriptors */
				
			} //end sockfd is readable
		} //end check all clients for data loop
	} //end main loop
	
	mysql_close(con); //close connection to mysql database
	
} //end main
