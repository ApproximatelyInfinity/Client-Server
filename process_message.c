//gcc -o process_message $(mysql_config --cflags) process_message.c $(mysql_config --libs)
//./process_message
//pass struct and get sockfd var into database to see if its in use
/*#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>*/
#include "header.h"

/*void rec_to_file(struct Record *rec)
{
	FILE * pFile;
	char filename[MAXLEN];
	
	strcpy(filename, rec->firstname);
	strcat(filename, rec->middlename);
	strcat(filename, rec->lastname);
	
	pFile = fopen (filename, "w");
	
	fprintf (
		pFile,
		"%d\n%s\n%s\n%s\n%s\n%s\n%s\n%d\n%d\n%s\n%s\n%s\n%s\n%s\n",
		rec->file_number, rec->firstname, rec->middlename, rec->lastname,
		rec->address, rec->phone, rec->dob, rec->height, rec->weight,
		rec->ethnicity, rec->religion, rec->allergies, rec->smoker,
		rec->medical_history
	);
	
	fclose (pFile);
	
}*/



int finish_with_error(MYSQL *con) {
  fprintf(stderr, "%s\n", mysql_error(con));
  //mysql_close(con);
  exit(1); 
} 

int process_message(int sockfd, int action, struct Record *rec) {
	
	//start test code
	
	
	//rec_to_file(rec);
	//return 1;
	
	//end test code
		
	
	//MYSQL *con;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	int i;
	//int action;	
	char query[255];

	
//--------------------------------------------	
	/*int height = 180; 
	int weight = 150;
	
	char fname[] = "Don";
	char mname[] = "Eric";
	char lname[] = "Brogdon";
	char address[] = "Comanche";
	char phonenum[] = "5804757973";
	char dob[] = "June 8th 1991";
	char ethnicity[] = "Caucasian";
	char religion[] = "Christian";
	char allergies[] = "Cats";
	char smoker[] = "No";
	char medicalhistory[] = "None";*/
//---------------------------------------------	
	
	
	//Connect to the Database
	char *server = "localhost";
	char *user = "cs08";
	char *password = "CU2FchEL";
	char *database = "cs08";

	
	//Connection function to the database
	if(!mysql_real_connect(con, server, user, password, database, 0, NULL, 0)) { 
		finish_with_error(con);
	}
	
	switch(action) {
		case 1: //Create a field in the Record table
			sprintf(query, "INSERT INTO Record (FirstName, MiddleName, LastName, Address, PhoneNumber, DoB, Height, Weight, Ethnicity, Religion, Allergies, Smoker, MedicalHistory) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%d', '%d', '%s', '%s', '%s', '%s', '%s')", rec->firstname, rec->middlename, rec->lastname, rec->address, rec->phone, rec->dob, rec->height, rec->weight, rec->ethnicity, rec->religion, rec->allergies, rec->smoker, rec->medical_history);
			mysql_query(con, query); //function to execute query variable
			return 1;
			break;
				
		case 2: //Displays the fields in the Record table
			if(mysql_query(con, "SELECT * FROM Record")) {
			finish_with_error(con);
			}
			
			res = mysql_store_result(con);

			if(res == NULL) {
				finish_with_error(con);
			}	
	
			int num_fields = mysql_num_fields(res); 
			while ((row = mysql_fetch_row(res))) { //Prints the results
				for(i = 0; i < num_fields; i++) { 
					printf("%s \n", row[i] ? row[i] : "NULL"); 
				}
			printf("*************************");
			printf("\n"); 
			}
			break;
			
		case 3: //Updates a field in the Record table
			/*printf("Which field do you want to update: ");
			
			printf("Enter the new height: ");
			scanf("%d", &height);
			sprintf(query, "UPDATE Record SET height = %d WHERE Height = 180", height);
			mysql_query(con, query);*/
			break;		
		case 4: //Deletes a field in the Record table
			//printf("Enter the last name of the subject to delete: ");
			//scanf("%s", lname);
			sprintf(query, "DELETE FROM Record WHERE FirstName = ('%s') AND MiddleName = ('%s') AND LastName = ('%s')", rec->firstname, rec->middlename, rec->lastname);
			mysql_query(con, query);
			return 1;
			break;
	}

}
