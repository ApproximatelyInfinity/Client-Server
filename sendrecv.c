#include "header.h"


int create_rec(struct Record *rec, int sockfd)
{	
	int status;
	ssize_t n, len;
	char sendbuf[MAXLINE], temp[MAXLINE];
	char msg[MAXLINE];
	char header[HEADLEN+1]; //stores msg length as string
	
	msg[0] = '\0';
	sprintf(msg, "<ac>%d</ac>", CREATE); //serialize action, store in msg
	serialize_rec(temp, rec); //serialize record
	strcat(msg, temp); //append serialized record to msg
	create_header(header, HEADLEN, msg); //create header for msg
	strcpy(sendbuf, header); //put header in sendbuf
	strcat(sendbuf, msg); //put msg in sendbuf
	
	//start temp code
	/*
	printf("Length of msg: %ld\n", strlen(msg));
	printf("Header string: %s\n", header);
	printf("Message string: %s\n", msg);
	system("pause");
	exit(0);
	*/
	//end temp code
	
	
	if( writen(sockfd, sendbuf, strlen(sendbuf)) == -1 ) { //send data
		printf("Error: write to socket failed.\n");
		return -1;
	}
		//success sending data
	printf("Success sending record to server. Awaiting response..\n");
	
	long wait_seconds = 30;
	if ( (status = wait_for_data(sockfd, wait_seconds)) < 1 ) { //wait for response
		printf("Error: failed receiving server response.\n");
		return -1;
	}
		//received response
			
	if ( (n = read_socket(sockfd, header, HEADLEN)) <= 0 ) { //read header
		if(n == 0) {
			printf("Error: Server terminated prematurely.\n");
		} else {
			printf("Error: read from socket failed.\n");
		}
		return -1;
	}
		//success getting header
		
	sscanf(header, "%ld", &len); //retrieve msg length from header
	
	if ( (n = read_socket(sockfd, msg, len)) <= 0 ) { //read data
		if(n == 0) {
			printf("Error: Server terminated prematurely.\n");
		} else {
			printf("Error: read from socket failed.\n");
		}
		return -1;
	}
		//success getting data

	deserialize_int(&status, msg, "<st>", "</st>"); //decode message
	
	return status; //return server response
	
} //end create_rec


int request_rec(struct Record *rec, int sockfd)
{
	int status;
    ssize_t n, len;
    char sendbuf[MAXLINE], temp[MAXLINE];
    char msg[MAXLINE];
    char header[HEADLEN+1];
    msg[0] = '\0';
    sprintf(msg, "<ac>%d</ac>", REQUEST); //Changed this to DELETE
    serialize_rec(temp, rec);
    strcat(msg, temp);
    create_header(header, HEADLEN, msg);
    strcpy(sendbuf, header);
    strcat(sendbuf, msg);
    if( writen(sockfd, sendbuf, strlen(sendbuf)) == -1 ) {
        printf("Error: write to socket failed.\n");
        return -1;
    }
    printf("Success sending record to server. Awaiting response..\n");
   
    long wait_seconds = 30;
    if ( (status = wait_for_data(sockfd, wait_seconds)) < 1 ) {
        printf("Error: failed receiving server response.\n");
        return -1;
    }
           
    if ( (n = read_socket(sockfd, header, HEADLEN)) <= 0 ) {
        if(n == 0) {
            printf("Error: Server terminated prematurely.\n");
        } else {
            printf("Error: read from socket failed.\n");
        }
        return -1;
    }
       
    sscanf(header, "%ld", &len);
   
    if ( (n = read_socket(sockfd, msg, len)) <= 0 ) {
        if(n == 0) {
            printf("Error: Server terminated prematurely.\n");
        } else {
            printf("Error: read from socket failed.\n");
        }
        return -1;
    }

    deserialize_int(&status, msg, "<st>", "</st>");
   
    return status;
} //rend request_rec


int update_rec(struct Record *rec, int sockfd)
{
	int status;
    ssize_t n, len;
    char sendbuf[MAXLINE], temp[MAXLINE];
    char msg[MAXLINE];
    char header[HEADLEN+1];
    msg[0] = '\0';
    sprintf(msg, "<ac>%d</ac>", UPDATE); //Changed this to UPDATE
    serialize_rec(temp, rec);
    strcat(msg, temp);
    create_header(header, HEADLEN, msg);
    strcpy(sendbuf, header);
    strcat(sendbuf, msg);
    if( writen(sockfd, sendbuf, strlen(sendbuf)) == -1 ) {
        printf("Error: write to socket failed.\n");
        return -1;
    }
    printf("Success sending record to server. Awaiting response..\n");
   
    long wait_seconds = 30;
    if ( (status = wait_for_data(sockfd, wait_seconds)) < 1 ) {
        printf("Error: failed receiving server response.\n");
        return -1;
    }
           
    if ( (n = read_socket(sockfd, header, HEADLEN)) <= 0 ) {
        if(n == 0) {
            printf("Error: Server terminated prematurely.\n");
        } else {
            printf("Error: read from socket failed.\n");
        }
        return -1;
    }
       
    sscanf(header, "%ld", &len);
   
    if ( (n = read_socket(sockfd, msg, len)) <= 0 ) {
        if(n == 0) {
            printf("Error: Server terminated prematurely.\n");
        } else {
            printf("Error: read from socket failed.\n");
        }
        return -1;
    }

    deserialize_int(&status, msg, "<st>", "</st>");
   
    return status;
} //end update_rec


int delete_rec(struct Record *rec, int sockfd)
{
	int status;
    ssize_t n, len;
    char sendbuf[MAXLINE], temp[MAXLINE];
    char msg[MAXLINE];
    char header[HEADLEN+1];
    msg[0] = '\0';
    sprintf(msg, "<ac>%d</ac>", DELETE); //Changed this to DELETE
    serialize_rec(temp, rec);
    strcat(msg, temp);
    create_header(header, HEADLEN, msg);
    strcpy(sendbuf, header);
    strcat(sendbuf, msg);
    if( writen(sockfd, sendbuf, strlen(sendbuf)) == -1 ) {
        printf("Error: write to socket failed.\n");
        return -1;
    }
    printf("Success sending record to server. Awaiting response..\n");
   
    long wait_seconds = 30;
    if ( (status = wait_for_data(sockfd, wait_seconds)) < 1 ) {
        printf("Error: failed receiving server response.\n");
        return -1;
    }
           
    if ( (n = read_socket(sockfd, header, HEADLEN)) <= 0 ) {
        if(n == 0) {
            printf("Error: Server terminated prematurely.\n");
        } else {
            printf("Error: read from socket failed.\n");
        }
        return -1;
    }
       
    sscanf(header, "%ld", &len);
   
    if ( (n = read_socket(sockfd, msg, len)) <= 0 ) {
        if(n == 0) {
            printf("Error: Server terminated prematurely.\n");
        } else {
            printf("Error: read from socket failed.\n");
        }
        return -1;
    }

    deserialize_int(&status, msg, "<st>", "</st>");
   
    return status;
} //end delete_rec


/*------------------------------------------------------------*/

void create_header(char * header, int headlen, const char * msg)
{
	//header maxsize should be greater than headlen
	ssize_t n, hlen;
	hlen = headlen;
	n = strlen(msg); //get length of msg
	sprintf(header, "%ld", n); //put length of msg in header
    n = strlen(header); //get header's new length
    while(n < hlen) { //pad with x until length = hlen
        header[n] = 'x';
        n++;
    }
    header[n] = '\0'; //null terminate header
}

ssize_t read_socket(int sockfd, char * dest, ssize_t len)
{
	ssize_t n, bytesread;
	char recvbuf[MAXLINE];
	
	dest[0] = '\0'; //empty out dest
	bytesread = 0; //init bytesread
	
	while (bytesread < len) {
		if ( (n = readn(sockfd, recvbuf, len-bytesread)) > 0 ) {
			bytesread = bytesread + n;
			recvbuf[n] = '\0'; //null terminate recvbuf
			strcat(dest, recvbuf); //store what's read
		} else {
			return n; // return 0 or -1  (0 = eof, -1 = read error)
		}
	}
	return bytesread;
}

int wait_for_data(int sockfd, long wait_sec)
{
	fd_set rfds;
    struct timeval tv;
    int retval;

	FD_ZERO(&rfds);
	FD_SET(sockfd, &rfds); //monitor sockfd

	tv.tv_sec = wait_sec;
	tv.tv_usec = 0;

	if ( (retval=select(sockfd+1, &rfds, NULL, NULL, &tv)) == -1 ) {
		return -1; //select error
	} else if (retval) {
		return 1; //data avail
	}
	
	return 0; //no data within specified wait time
}

void deserialize_str(char * dest, const char * source, const char * opentag, const char * closetag)
{
    int i = 0;
    char * beg = strstr(source, opentag) + strlen(opentag);
    char * end = strstr(source, closetag);
    while (beg != end) {
        dest[i++] = *beg++;
    }
    dest[i] = '\0';
}

void deserialize_int(int * dest, const char * source, const char * opentag, const char * closetag)
{
	char temp[MAXLINE];
	deserialize_str(temp, source, opentag, closetag);
	*dest = atoi(temp);
}

void serialize_rec(char * dest, struct Record * rec)
{
	char format[MAXLEN];
	strcpy(format, "<no>%d</no><fn>%s</fn><mn>%s</mn><ln>%s</ln><ad>%s</ad>");
	strcat(format, "<ph>%s</ph><db>%s</db><ht>%d</ht><wt>%d</wt><et>%s</et>");
	strcat(format, "<re>%s</re><al>%s</al><sm>%s</sm><mh>%s</mh>");
	sprintf(dest, format, rec->file_number, rec->firstname, rec->middlename,
	rec->lastname, rec->address, rec->phone, rec->dob, rec->height, rec->weight,
	rec->ethnicity, rec->religion, rec->allergies, rec->smoker, rec->medical_history);
}

void deserialize_rec(char * source, struct Record * rec)
{
	deserialize_int(&(rec->file_number), source, "<no>", "</no>");
	deserialize_str(rec->firstname, source, "<fn>", "</fn>");
	deserialize_str(rec->middlename, source, "<mn>", "</mn>");
	deserialize_str(rec->lastname, source, "<ln>", "</ln>");
	deserialize_str(rec->address, source, "<ad>", "</ad>");
	deserialize_str(rec->phone, source, "<ph>", "</ph>");
	deserialize_str(rec->dob, source, "<db>", "</db>");
	deserialize_int(&(rec->height), source, "<ht>", "</ht>");
	deserialize_int(&(rec->weight), source, "<wt>", "</wt>");
	deserialize_str(rec->ethnicity, source, "<et>", "</et>");
	deserialize_str(rec->religion, source, "<re>", "</re>");
	deserialize_str(rec->allergies, source, "<al>", "</al>");
	deserialize_str(rec->smoker, source, "<sm>", "</sm>");
	deserialize_str(rec->medical_history, source, "<mh>", "</mh>");
}

ssize_t writen(int fd, const void *vptr, size_t n) //function from unp.h
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;   /* and call write() again */
			else
				return (-1);    /* error */
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n);
}

ssize_t readn(int fd, void *vptr, size_t n) //function from unp.h
{
	size_t  nleft;
	ssize_t nread;
	char   *ptr;

	ptr = vptr;
	nleft = n;	
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;      /* and call read() again */
			else
				return (-1);
		} else if (nread == 0)
			break;              /* EOF */

		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);         /* return >= 0 */
}
