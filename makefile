CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

ClientAppName = clientapp
ServerAppName = serverapp
ArchiveName = project3.tar

PROGS = $(ClientAppName) $(ServerAppName)

all: $(PROGS)

$(ClientAppName): client.o client_interface.o sendrecv.o
	$(CC) client.o client_interface.o sendrecv.o -o $(ClientAppName)

client.o: client.c header.h
	$(CC) $(CFLAGS) client.c

client_interface.o: client_interface.c header.h
	$(CC) $(CFLAGS) client_interface.c

sendrecv.o: sendrecv.c header.h
	$(CC) $(CFLAGS) sendrecv.c

$(ServerAppName): server.c sendrecv.c process_message.c header.h
	$(CC) -o $(ServerAppName) $$(mysql_config --cflags) server.c sendrecv.c process_message.c $$(mysql_config --libs)

clean:
	rm -f *.o *~
	rm $(ClientAppName) $(ServerAppName)

tar:
	tar cfv $(ArchiveName) header.h client.c server.c client_interface.c \
	process_message.c sendrecv.c makefile readme.txt
	
fresh:
	touch *
	make clean
	make

test:
	touch *
	make clean
	make
	$(ServerAppName) &
	$(ClientAppName) 127.0.0.1
	ps | grep $(ServerAppName) | awk '{print $$1}' | xargs kill -9

killserv:
	ps aux | grep $$('whoami') | grep $(ServerAppName) | grep -v grep | awk '{print $$2}' | xargs kill -9

