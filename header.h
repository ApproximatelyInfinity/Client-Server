#ifndef PROJECT3_HEADER_H
#define PROJECT3_HEADER_H

#include <stdio.h>		//fgets, fputs
#include <stdlib.h>		//exit
#include <unistd.h>		//write, read
#include <string.h>		//strlen
#include <errno.h>		//for error codes
#include <time.h>
#include <mysql/mysql.h> //mysql library
#include <ctype.h> //data types
#include <sys/socket.h>	// basic socket definitions
#include <netinet/in.h>	// sockaddr_in{} and other Internet defns
#include <arpa/inet.h>	// inet functions

#define MAXLINE 4096
#define SA struct sockaddr
#define SERV_PORT 5555
#define LISTENQ 1024

#define max(a, b) (((a) > (b)) ? (a) : (b))

MYSQL *con; //mysql connection

//medical record structure
struct Record {
	int file_number;
	char * firstname;
	char * middlename;
	char * lastname;
	char * address;
	char * phone;
	char * dob;
	int height;
	int weight;
	char * ethnicity;
	char * religion;
	char * allergies; //entries separated by commas
	char * smoker;
	char * medical_history; //entries separated by commas
};

//max size for strings in Record
#define MAXLEN 255

enum ACTION { CREATE=1, REQUEST=2, UPDATE=3, DELETE=4, DISCON=5};


//client side functions

void client_interface(int sockfd);
//Main interface for client.
//Connection with server has already been established

int create_rec(struct Record *rec, int sockfd);
//sends rec though sockfd with CREATE signal
//returns -1 if error, 0 if rec exists or already open, 1 if success

int request_rec(struct Record *rec, int sockfd);
//receives rec though sockfd
//  Sends REQUEST signal along with rec (firstname, lastname, middlename filled)
//  If success, rec will contain the requested record
//returns -1 if error, 0 if rec doesn't exist or already open, 1 if success

int update_rec(struct Record *rec, int sockfd);
//sends rec through sockfd with UPDATE signal
//returns -1 if error, 0 if rec doesn't exist or already open, 1 if success

int delete_rec(struct Record *rec, int sockfd);
//sends rec though sockfd with DELETE signal
//      (rec.firstname, rec.lastname, rec.middlename filled)
//returns -1 if error, 0 if rec doesn't exist or already open, 1 if success


//server side functions

int process_message(int sockfd, int action, struct Record *rec);
//creates, retrieves, updates, and deletes rec
//returns -1 if error, 1 if success
//returns 0
//		if action=CREATE and record already exists or open by another
//		if action=REQUEST and record doesn't exist or open by another
//		if action=UPDATE and record doesn't exist or open by another
//		if action=DELETE and record doesn't exist or open by another
//action=DISCON removes client's sockfd connection to any rec in database


//sendrecv functions

ssize_t writen(int fd, const void *vptr, size_t n);
//Write "n" bytes to a descriptor. (function from unp.h)
//if errno = EINTR then it tries again to write
//returns -1 on error, else returns number of bytes written to fd

ssize_t readn(int fd, void *vptr, size_t n);
//Read "n" bytes from a descriptor. (function from unp.h)
//if errno = EINTR then it tries again to read
//returns -1 on error, else returns number of bytes read from fd


void deserialize_str(char * dest, const char * source, const char * opentag, const char * closetag);
void deserialize_int(int * dest, const char * source, const char * opentag, const char * closetag);
void serialize_rec(char * dest, struct Record * rec);
void deserialize_rec(char * source, struct Record * rec);
int wait_for_data(int sockfd, long wait_sec);

#define HEADLEN 10 //length of msg header

void create_header(char * header, int headlen, const char * msg);
ssize_t read_socket(int sockfd, char * dest, ssize_t len);

#endif //PROJECT3_HEADER_H

