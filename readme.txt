
compiling commands

  make                  compiles all source files

  make all              compiles all source files
  
  make clientapp        compiles client source files
  
  make serverapp        compiles server source files

  make clean            removes object/backup files, *.o and *~
						also removes clientapp and serverapp

  make tar              archives source files to project3.tar


running commands

  clientapp <serverIP>  runs clientapp
 
  serverapp &           runs serverapp


other commands	

  make test             compiles all source files,
                        runs serverapp in background,
                        runs clientapp with loopback address,
                        kills serverapp after clientapp program terminates.

  make killserv         kills any running serverapp processes on system,
                        if no processes running then it gives usage error (just ignore error).

[db194309@ada: Final2]$ make
gcc -Wall -c client.c
gcc -Wall -c client_interface.c
make: Warning: File `sendrecv.c' has modification time 3.2e+03 s in the future
gcc -Wall -c sendrecv.c
gcc client.o client_interface.o sendrecv.o -o clientapp
gcc -o serverapp $(mysql_config --cflags) server.c sendrecv.c process_message.c $(mysql_config --libs)
make: warning:  Clock skew detected.  Your build may be incomplete.
[db194309@ada: Final2]$ ./serverapp&
[1] 19509
[db194309@ada: Final2]$ ./clientapp 127.0.0.1

Welcome, you are now connected.

  MAIN MENU
   (1) CREATE RECORD
   (2) REQUEST RECORD
   (3) UPDATE RECORD
   (4) DELETE RECORD
   (q) quit rogram

  Enter selection: 1

First Name: Don
Middle Name: Eric
Last Name: Brogdon
Date of Birth: Red
Address: Orange
Phone Number: 1
Height: 2
weight: 3
Ethnicity: Yellow
Religion: Green
Allergies: Blue
Smoking status: Purple
Medical History: Pink
Success sending record to server. Awaiting response..
Successfully created the record.

  MAIN MENU
   (1) CREATE RECORD
   (2) REQUEST RECORD
   (3) UPDATE RECORD
   (4) DELETE RECORD
   (q) quit rogram

  Enter selection: 2

First Name: Don
Middle Name: Eric
Last Name: Brogdon
Success sending record to server. Awaiting response..
Don
Eric
Brogdon
Orange
1
Red
2
3
Yellow
Green
Blue
Purple
Pink
*************************
This record does not exit or open by another client.

  MAIN MENU
   (1) CREATE RECORD
   (2) REQUEST RECORD
   (3) UPDATE RECORD
   (4) DELETE RECORD
   (q) quit rogram

  Enter selection: 4

First Name: Don
Middle Name: Eric
Last Name: Brogdon
Are you sure you want to delete Don Eric Brogdon's record (Y/N)? Y
Success sending record to server. Awaiting response..
Successfully deleted the record.

  MAIN MENU
   (1) CREATE RECORD
   (2) REQUEST RECORD
   (3) UPDATE RECORD
   (4) DELETE RECORD
   (q) quit rogram

  Enter selection: 3

First Name: Don
Middle Name: Eric
Last Name: Brogdon
Success sending record to server. Awaiting response..
This record does not exit or open by another client.

  MAIN MENU
   (1) CREATE RECORD
   (2) REQUEST RECORD
   (3) UPDATE RECORD
   (4) DELETE RECORD
   (q) quit rogram

  Enter selection: q

Bye.

Connection to server closed.
[db194309@ada: Final2]$
*********************************************************************************
mysql> SELECT * FROM Record;
Empty set (0.00 sec)

mysql> SELECT * FROM Record;
+-----------+------------+----------+---------+-------------+------+--------+--------+-----------+----------+-----------+--------+----------------+
| FirstName | MiddleName | LastName | Address | PhoneNumber | DoB  | Height | Weight | Ethnicity | Religion | Allergies | Smoker | MedicalHistory |
+-----------+------------+----------+---------+-------------+------+--------+--------+-----------+----------+-----------+--------+----------------+
| Don       | Eric       | Brogdon  | Orange  | 1           | Red  |      2 |      3 | Yellow    | Green    | Blue      | Purple | Pink           |
+-----------+------------+----------+---------+-------------+------+--------+--------+-----------+----------+-----------+--------+----------------+
1 row in set (0.00 sec)

mysql> SELECT * FROM Record;
Empty set (0.00 sec)