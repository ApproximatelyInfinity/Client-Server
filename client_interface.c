#include "header.h"

void display_top_menu(); //displays main menu and prompt for selection
void display_update_menu(); //displays update menu and prompt for selection
void display_record(struct Record *rec); //displays record
void display_status(int action, int status); //displays status messages
void fill_record(struct Record *rec); //fills record from keyboard
void fill_record_first_mid_last(struct Record *rec);
	//fills rec firstname, middlename, lastname (for server search)
void fill_record_var(char choice, struct Record *rec);
	//fills variable in rec based on given choice
	
void fill_record_var_submenu(char * str); //helper function for fill_record_var()
//fills str with keyboard entries (appending to or replacing all)

void get_trimmed_input(char * str, int maxlen);
//fills str from keyboard and trim the newline char. maxlen is str maxsize

/*-------------------------------------------------*/
void client_interface(sockfd) {

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

	int status;

	char inputbuf[MAXLINE];
	char choice,	//main menu choice
		 subchoice; //submenu choice

	//stating the main client interface loop
	do {
		display_top_menu();
		fgets(inputbuf, MAXLINE, stdin);
		choice = tolower(inputbuf[0]); //first characted typed in
		switch (choice) {
			case '1': //CREATE RECORD
				printf("\n");
				fill_record(&rec); //get values for rec from keyboard
				status = create_rec(&rec, sockfd); //send the record to server
				display_status(CREATE, status);
				break;
			case '2': //REQUEST RECORD
				//get firstname, middlename, lastname for server search criteria
				printf("\n");
				fill_record_first_mid_last(&rec);
				if ( (status = request_rec(&rec, sockfd)) == 1 ) {
					printf("Success retrieving record.\n");
					display_record(&rec); //success getting rec so display
				}
				else {
					display_status(REQUEST, status); //display status
				}
				break;
			case '3': //UPDATE RECORD
				//get firstname, middlename, lastname for server search criteria
				printf("\n");
				fill_record_first_mid_last(&rec);
				if ( (status = request_rec(&rec, sockfd)) == 1 ) {
					display_record(&rec); //success getting rec so display
					printf("Ready to update record.\n");
					do {
						display_update_menu();
						fgets(inputbuf, MAXLINE, stdin);
						subchoice = tolower(inputbuf[0]);
						if(subchoice != 'q' && subchoice != 's') {
							fill_record_var(subchoice, &rec);
						}
					} while(subchoice != 'q' && subchoice != 's');
					if(subchoice == 's') { //user wants to send updated record
						status = update_rec(&rec, sockfd); //update rec
						display_status(UPDATE, status); //display status
					}
				}
				else {
					display_status(REQUEST, status); //display status
				}
				break;
			case '4': //DELETE RECORD
				//get firstname, middlename, lastname for server search criteria
				printf("\n");
				fill_record_first_mid_last(&rec);
				//confirm
				printf("Are you sure you want to delete %s %s %s's record (Y/N)? ",
				        rec.firstname , rec.middlename, rec.lastname);
				fgets(inputbuf, MAXLINE, stdin);
				if (inputbuf[0] == 'Y' || inputbuf[0] == 'y')
				{
					status = delete_rec(&rec, sockfd); // delete record from server
					display_status(DELETE, status); //display status
				} else {
					printf("Cancelled deleting record.\n");
				}
				break; //this is temporary code
		}

	} while(choice != 'q');
	
	printf("\nBye.\n");
	
}
/*-------------------------------------------------*/

void display_top_menu() {
	printf("\n  MAIN MENU\n");
	printf("   (1) CREATE RECORD\n");
	printf("   (2) REQUEST RECORD\n");
	printf("   (3) UPDATE RECORD\n");
	printf("   (4) DELETE RECORD\n");
	printf("   (q) quit rogram\n\n");
	printf("  Enter selection: ");
}
void display_update_menu()
{
	printf("\n  UPDATE RECORD\n");
	printf("   (0) First name\n");
	printf("   (1) Middle name\n");
	printf("   (2) Last name\n");
	printf("   (3) Address\n");
	printf("   (4) Phone number\n");
    printf("   (5) Height\n");
	printf("   (6) Weight\n");
	printf("   (7) Allergies\n");
	printf("   (8) Smoking status\n");
	printf("   (9) Medical History\n");
	printf("   (q) discard changes and go to main menu\n");
	printf("   (s) send modified record and go to main menu\n\n");
	printf("  Enter selection: ");
}

void display_record(struct Record *rec) {
	printf("\n-----------MEDICAL RECORD------------\n");
	printf("NAME: %s %s %s\n",
			rec->firstname, rec->middlename, rec->lastname);
	printf("PHONE: %s\nADDRESS: %s\nDOB: %s\n",
			rec->phone,  rec->address,  rec->dob);
	printf("HEIGHT: %d   WEIGHT: %d\nETHNICITY: %s   RELIGION: %s\n",
			rec->height, rec->weight, rec->ethnicity, rec->religion);
	printf("ALLERGIES: %s\nSMOKING STATUS: %s\nMEDICAL HISTORY: %s\n",
			rec->allergies, rec->smoker, rec->medical_history);
	printf("-------------------------------------\n");
}

void display_status(int action, int status) {
	switch (action) {
		case CREATE:
			if (status == -1) {
				printf("Error creating record.\n");
			} else if (status == 0) {
				printf("This record already exist or open by another client.\n");
			} else if (status == 1) {
				printf("Successfully created the record.\n");
			}
			break;
		case REQUEST:
			if(status == 0) {
				printf("This record does not exit or open by another client.\n");
			} else if(status == -1) {
				printf("Error loading the record.\n");
			}
			break;
		case UPDATE:
			if(status == 0) {
				printf("This record does not exist or open by another client.\n");
			} else if(status == -1) {
				printf("Error loading the record.\n");
			} else if(status == 1) {
				printf("Successfully updated the record.\n");
			}
			break;
		case DELETE:
			if(status == 0) {
				printf("This record does not exist or open by another client.\n");
			} else if(status == -1) {
				printf("Error deleting record.\n");
			} else if(status == 1) {
				printf("Successfully deleted the record.\n");
			}
			break;
	}
}

void fill_record(struct Record *rec) {
	char tmp[MAXLEN];
	fill_record_first_mid_last(rec); //get first, middle, last
	printf("Date of Birth: ");
	get_trimmed_input(rec->dob, MAXLEN);
	printf("Address: ");
	get_trimmed_input(rec->address, MAXLEN);
	printf("Phone Number: ");
	get_trimmed_input(rec->phone, MAXLEN);
	printf("Height: ");
	scanf("%d", &rec->height);
	fgets(tmp, MAXLEN, stdin); //get rid of newline char left by scanf to integer
	printf("weight: ");
	scanf("%d" , &rec->weight);
	fgets(tmp, MAXLEN, stdin); //get rid of newline char left by scanf to integer
	printf("Ethnicity: ");
	get_trimmed_input(rec->ethnicity, MAXLEN);
	printf("Religion: ");
	get_trimmed_input(rec->religion, MAXLEN);
	printf("Allergies: ");
	get_trimmed_input(rec->allergies, MAXLEN);
	printf("Smoking status: ");
	get_trimmed_input(rec->smoker, MAXLEN);
	printf("Medical History: ");
	get_trimmed_input(rec->medical_history, MAXLEN);
}

void fill_record_first_mid_last(struct Record *rec) {
	printf("First Name: ");
	get_trimmed_input(rec->firstname, MAXLEN);
	printf("Middle Name: ");
	get_trimmed_input(rec->middlename, MAXLEN);
	printf("Last Name: ");
	get_trimmed_input(rec->lastname, MAXLEN);
}

 //fills str with keyboard entries (appending to or replacing all)
void fill_record_var_submenu(char * str) {
	char temp[MAXLEN];
	do {
		printf(" (1) Add   (2) Replace all   enter choice: ");
		fgets(temp, MAXLEN, stdin);
	} while(temp[0] != '1' && temp[0] != '2' && temp[0] != '\n');
	if(temp[0] == '1') { //user wants to add new
		printf("Enter new entry: ");
		get_trimmed_input(temp, MAXLEN);
		strcat(str, ", "); //enter comma separator
		strcat(str, temp); //enter new allergy
	} else { //user wants to replace all
		printf("Enter all entries: ");
		get_trimmed_input(str, MAXLEN);
	}
}

void fill_record_var(char choice, struct Record *rec) { //uses fill_record_var_submenu()
	char tmp[MAXLEN];
	switch (choice) {
		case '0': //update firstname
			printf("First Name: %s\nEnter new name: ", rec->firstname);
			get_trimmed_input(rec->firstname, MAXLEN);
			break;
		case '1'://update middlename
			printf("Middle Name: %s\nEnter new name: ", rec->middlename);
			get_trimmed_input(rec->middlename, MAXLEN);
			break;
		case '2': //update lastname
			printf("Last Name: %s\nEnter new name: ", rec->lastname);
			get_trimmed_input(rec->lastname, MAXLEN);
			break;
		case '3': //update address
			printf("Address: %s\nEnter new address: ", rec->address);
			get_trimmed_input(rec->address, MAXLEN);
			break;
		case '4': //update phone number
			printf("Phone Number: %s\nEnter new phone number: ", rec->phone);
			get_trimmed_input(rec->phone, MAXLEN);
			break;
		case '5': //update height
			printf("Height: %d\nEnter new height: ", rec->height);
			scanf("%d", &rec->height);
			fgets(tmp, MAXLEN, stdin); //get rid of newline char left by scanf to integer
			break;
		case '6': //update weight
			printf("weight: %d\nEnter new weight: ", rec->weight);
			scanf("%d", &rec->weight);
			fgets(tmp, MAXLEN, stdin); //get rid of newline char left by scanf to integer
			break;
		case '7': //update Allergies
			printf("Allergies: %s\n", rec->allergies);
			fill_record_var_submenu(rec->allergies);
			break;
		case '8': //update Smoking status
			printf("Smoking status: %s\nEnter new status: ", rec->smoker);
			get_trimmed_input(rec->smoker, MAXLEN);
			break;
		case '9': //update medical record
			printf("Medical History: %s\n", rec->medical_history);
			fill_record_var_submenu(rec->medical_history);
			break;
	}
}

void get_trimmed_input(char * str, int maxlen) {
	char * ch;
	fgets(str, maxlen, stdin);
	ch = strchr(str, '\n'); //find newline in str
	*ch = '\0'; //replace newline char with null char
}
