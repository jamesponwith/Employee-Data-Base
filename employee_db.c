/*
 * Employee database
 * Patrick Hall
 * James Ponwith
 * 20 hours
 * 0 Bucks
 * We enjoyed working collaboratively on the project.
 * We truly felt apart of the hackathon at TechCrunch Distrupt.
 * The project posed some tough obstacles.
 *
 * Some include: passing the array of structs through functions,
 * assigning new values to a certain position because of struggles
 * involving scanf, and a silly mistake being sorting by ID right after
 * printing database meaning our sort by last name function "never worked".
 * Also Writing code on vim while connected to the linux machine was,
 * super slow. So we wrote the code on our local machine's vim, and used scp
 * to transfer the edited files back to the linux machine for debugging.
 */

//hey
#include <stdio.h> 		    // the C standard I/O library
#include <stdlib.h> 		  // the C standard library
#include <string.h>     	// the C string library
#include "readfile.h"   	// my file reading routines

#define MAXFILENAME  128
#define MAXNAME      64
#define MAXEMPLOYEES 1024

struct Employee {
	char first_name[MAXNAME];
	char last_name[MAXNAME];
	int salary;
	int id;
};

typedef struct Employee Employee;

// forward declaration of functions
void getFilenameFromCommandLine(char filename[], int argc, char *argv[]);

void readAndPrintFile(char *filename);

struct Employee *createEmployeeArray();

void displayMenu();

int importEmployees(Employee *employees, char *filename);

void printDatabase(Employee *data_base, int length);

void searchById(Employee *employees, int length);

void searchByLastName(Employee *employees, int length);

void addEmployee(Employee *employees, int length);

void sortByLastName(Employee *employees, int length);

void sortById(Employee *employees, int length);

//update functions
void update(Employee *employees, int length);

void updateID(Employee *employees, int length);

void updateFirst(Employee *employees, int length);

void updateLast(Employee *employees, int length);

void updateSalary(Employee *employees, int length);

int main (int argc, char *argv[]) {

	//this comes in handy when passing through fucntions
	int num_employees;

	// this initializes the filename string from the command line arguments
  char filename[MAXFILENAME];
	getFilenameFromCommandLine(filename, argc, argv);
  //Create an array of Employee pointers
	Employee *employees = createEmployeeArray();
  //importEmployees() returns an int, to set the initial number of employees. only gets //called once
	num_employees = importEmployees(employees, filename);

	int selection;

    /* Begins the loop that reprompts menu options until user chooses to quit */
    do {
				displayMenu();
        printf("%s", "Enter your choice: ");
				scanf("%d", &selection);
				switch(selection) {
						case 1:
								printDatabase(employees, num_employees);
								break;
						case 2:
								searchById(employees, num_employees);
								break;
						case 3:
								searchByLastName(employees, num_employees);
								break;
						case 4:
								addEmployee(employees, num_employees);
              	num_employees++; //increment number of employees
              	sortById(employees, num_employees); //sort by ID name after adding
								break;
						case 5:
								selection = 5; //to quit
								break;
            case 6:
                update(employees, num_employees);
                break;
            case 7:
								sortByLastName(employees, num_employees);
								printDatabase(employees, num_employees);
                break;
            case 8:
								sortById(employees, num_employees);
								printDatabase(employees, num_employees);
                break;
						default:
								printf("%s", "Invalid option (");
                printf("%d", selection);
                printf("%s\n", ") Valid values are 1 through 8\n");
        	}
			} while(selection != 5); // end of do/while after case 5 is choosen

	free(employees); // frees memory for employees array
	printf("\ngood bye\n");
	return 0;
} // end main()

/*
 * Reads a file name in order to open it and print its contents
 * @param takes in the desired file to be opened
 * @return
 */
void readAndPrintFile(char *filename) {
	printf("filename '%s'\n", filename);

	int ret = openFile(filename);  // try to open the DB file
	if (ret == -1) {
		printf("bad error: can't open %s\n", filename);
		exit(1);
	}

	int id, salary;
	char fname[MAXNAME], lname[MAXNAME];

	while (ret != -1) {
		ret = readInt(&id);         // read the first line of values from file
		if (ret) { break; }
		ret = readString(fname);
		if (ret) { break; }
		ret = readString(lname);
		if (ret) { break; }
		ret = readInt(&salary);
		if (ret == 0) { // stuff was read in okay
			printf("%d %s %s %d\n", id, fname, lname, salary);
		}
	}
	closeFile();  // when all done close the file
}

/*
 * Retrieves the file name
 * @param filename[] -
 * @param argc -
 * @param *argv -
 * @return
 */
void getFilenameFromCommandLine(char filename[], int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s database_file\n", argv[0]);
		exit(1);
	}
	if (strlen(argv[1]) >= MAXFILENAME) {
		printf("Filename, %s, is too long, cp to shorter name and try again\n",
				filename);
		exit(1);
	}
	strcpy(filename, argv[1]);
}

/* Creates and allocates memory to the array of structs
 * @return Pointer to a struct in the array
 */
struct Employee *createEmployeeArray() {
	Employee *employees = calloc(MAXEMPLOYEES, sizeof(Employee));
	return employees;
}

/* Prints main menu (Options 1-8)
 * @param none
 * @return prints out the menu options for the user
 */
void displayMenu() {
    printf("%s\n", "Employee DB Menu:");
		printf("%s\n", "----------------------------------");
		printf("%s\n", "  (1) Print the Database.");
		printf("%s\n", "  (2) Lookup by ID.");
		printf("%s\n", "  (3) Lookup by Last Name.");
		printf("%s\n", "  (4) Add an Employee.");
		printf("%s\n", "  (5) Quit.");
    printf("%s\n", "  (6) Update an employee's info.");
    printf("%s\n", "  (7) Sort employees by last name.");
    printf("%s\n", "  (8) Sort employees by ID.");
		printf("%s\n", "----------------------------------");
}

/*
 * This function imports the employee information from a text file
 * @param import the array meant to be filled and the file that we are reading from
 * @return an array that is filled with employee information
 */

int importEmployees(Employee *employees, char *filename) {
	int ret = openFile(filename); // try to open db file
	if (ret == -1) {
		printf("bad error: can't open %s\n", filename);
		exit(1);
	}
	int id, salary;
	char first_name[MAXNAME], last_name[MAXNAME];

	int i = 0;
	while (ret != -1) {
		ret = readInt(&id);
		if (ret) { break; }
		ret = readString(first_name);
		if (ret) { break; }
		ret = readString(last_name);
		if (ret) { break; }
		ret = readInt(&salary);
		if (ret == 0) {
			employees[i].id = id;
			strcpy(employees[i].first_name, first_name);
			strcpy(employees[i].last_name, last_name);
			employees[i].salary = salary;
		}
		i++;
	}
	return i;
}

/* Formats and prints out the entire data base
* @param *data_base represents the employee array that is holding
* all of the employees information
* @param length - the length of the array, used for looping
* @return it prints each element of each object in the array
*/
void printDatabase(Employee *data_base, int length) {
	printf("%s\n", "------------------------------------------------------------------------");
	int i;
	for (i = 0; i < length; i++) {
		printf("%-26s", data_base[i].first_name);
		printf("%-26s", data_base[i].last_name);
		printf("%-10d", data_base[i].salary);
		printf("%d\n", data_base[i].id);
	} // loop to print all elements of structs
	printf("%s\n", "------------------------------------------------------------------------");
	printf("%s", "Number of employees: (");
	printf("%d", length);
	printf("%s\n", ")");
}

/*
 * Search by ID looks up employees by ID through user input
 * @param *employees - The array of structs representing the different employees
 * @param length - represents num_employees. Useful for looping through array
 * and matching input to correct employee
 * No return, just looks up by existing ID
 */
void searchById(Employee *employees, int length) {
	int input = 0;
  printf("%s", "Enter a 6-digit employee DD: ");
	scanf("%d", &input);
	for(int i = 0; i < length; i++) {
		if (input == employees[i].id) {
			printf("%-26s", employees[i].first_name);
			printf("%-26s", employees[i].last_name);
			printf("%-10d", employees[i].salary);
			printf("%d\n", employees[i].id);
			break;
		} // Prints the information for employee that matches
			// ID input
		else if (i == length - 1) {
	   		printf("%s","Employee with id ");
            printf("%d", input);
            printf("%s\n", " not found int DB\n");
		} // if the loop reaches the end of the array without matching ID
	}   // prints error
}

/*
 * Search by last name looks up employees by ID through user input
 * @param *employees - The array of structs representing the different employees
 * @param length - represents num_employees. Useful for looping through array
 * and matching input to correct employee
 * No return, just looks up by existing last name
*/
void searchByLastName(Employee *employees, int length) {
	char input[MAXNAME];
    printf("%s", "Enter last name: ");
	scanf("%s", input);
	for(int i = 0; i < length; i++) {
		if(strcmp(input, employees[i].last_name) == 0) {
			printf("%-26s", employees[i].first_name);
			printf("%-26s", employees[i].last_name);
			printf("%-10d", employees[i].salary);
			printf("%d\n", employees[i].id);
			break;
		} // Loop to match ID input
		else if (i == length - 1) {
			printf("%s\n","Error, no ID number matching input.");
		} // error message if ID not matched
	}
}

/*
 * Adds employee, then prompts to ask if user wants to keep or discard
 * @param *employees - The array of structs representing the different employees
 * @param length - represents num_employees. Useful for looping through array
 * and adding new employee to array
 * No return, just looks up by existing last name
*/
void addEmployee(Employee *employees, int length) {
	printf("%s", "Please enter ID, first name, last name, and salary.\n");
	int id;
    do {
    	printf("%s", "Enter ID: ");
        scanf("%d", &id);
        if (id > 999999 || id < 100000) {
            printf("%s\n", "Invalid id try again");
        }
    } while(id > 999999 || id < 100000);
		// ID input with validation and error handling to reprompt

    char first[MAXNAME];
    do {
        printf("%s", "Enter first name: ");
    	scanf("%s", first);
        if (first == NULL) {
            printf("%s\n", "invalid name ... enter again");
        }
    } while(first == NULL);
		// first name input with validation and error handling to reprompt

    char last[MAXNAME];
    do {
        printf("%s", "Enter last name: ");
    	scanf("%s", last);
        if (last == NULL) {
            printf("%s\n", "invalid name ... enter again");
        }
    } while(last == NULL);
		// last name input with validation and error handling to reprompt

    int sal;
    do {
        printf("%s", "Enter salary: ");
    	scanf("%d", &sal);
        if (sal < 30000 || sal > 150000) {
            printf("%s\n", "invalid salary ... enter again");
        }
    } while(sal < 30000 || sal > 150000);
		// salary input with validation and error handling to reprompt

    int choice = -1;
    do {
        printf("%s\n", "Are you sure you want to add this employee? \n(1) Yes \n(0) No\n");
        printf("%-14s", first);
        printf("%-14s", last);
        printf("%-10d", sal);
        printf("%d\n", id);

        scanf("%d", &choice);
        if (choice == 1) {
            employees[length].id = id;
            strcpy(employees[length].first_name, first);
            strcpy(employees[length].last_name, last);
            employees[length].salary = sal;
        } // 1 saves employee
        else if (choice != 0) {
            printf("%s\n", "invalid choice, please select yes(1) or no(0)");
            printf("%s", "Enter choice: ");
        } // 0 deletes employee
    } while(choice != 0 && choice != 1);
		// Asking if user want to keep or trash the new employee created
}

/*
 * Sort by last name manipulates data base to be sorted by last name
 * @param *employees - The array of structs representing the different employees
 * @param length - represents num_employees. Useful for looping through array
 * and for sorting
 * No return, just looks up by existing last name
*/
void sortByLastName(Employee *employees, int length) {
    for(int i = 1; i < length; i++) {
        for(int j = 0; j < length - i; j++) {
						if (strcmp(employees[j].last_name, employees[j + 1].last_name) > 0) {
								struct Employee temp = employees[j];
								employees[j] = employees[j + 1];
								employees[j + 1] = temp;
						}
        }
    } // Sorts array using strcmp
}

/*
 * Sort by ID manipulates data base to be sorted by ID
 * @param *employees - The array of structs representing the different employees
 * @param length - represents num_employees. Useful for looping through array
 * and for sorting
 * No return, just looks up by existing last name
*/
void sortById(Employee *employees, int length) {
    for(int i = 1; i < length; i++) {
        for(int j =0; j < length - i; j++) {
            if(employees[j].id > employees[j+1].id) {
                struct Employee temp = employees[j];
                employees[j] = employees[j+1];
                employees[j+1] = temp;
            }
        }
    } // Sorts array using logical operator > to compare ID numbers
}

//Update Functions
/*
 * Update ID updates the ID of the chosen employees
 * @param *employees - The array of structs representing the different employees
 * @param length - represents num_employees. Useful for all for loops manipulating
 * current information
 * No returns, all simply update or manipulate current information
 *** Parameters are the same for all UPDATE FUNCTIONS below ***
 */
void updateID(Employee *employees, int length) {
    int id;
    do {
        printf("%s\n", "Enter ID of person to change: ");
        scanf("%d", &id);
        if (id > 999999 || id < 100000) {
            printf("%s\n", "Invalid id try again");
        } // Print error prompting more input
    } while(id > 999999 || id < 100000);
		// Keeps loop running until correct ID is input

    for(int i = 0; i<length; i++){
        if(id == employees[i].id) {
            int id_input;
            do {
                printf("%s\n", "Enter new ID: ");
                scanf("%d", &id_input);
                if (id > 999999 || id < 100000) {
                    printf("%s\n", "Invalid id try again");
                }
            } while(id_input > 999999 || id_input < 100000);
            employees[i].id = id_input;
						break;
        } // searching for employee who matchs ID input and if found
					// prompts user to change the ID for that employee
        else if (i == length - 1) {
            printf("%s\n", "No person found");
        } // If entered ID is not found
    }
}

/*
 * Update first updates the first name of the chosen employees
 * @param array of employees and length of the array
 * @return void funciton, modifies the chosen employees first name
 */
void updateFirst(Employee *employees, int length) {
    char first_input[MAXNAME];
    int id;
    do {
        printf("%s\n", "Enter ID of person to change: ");
        scanf("%d", &id);
        if (id > 999999 || id < 100000) {
            printf("%s\n", "Invalid num try again");
        } // Print error prompting more input
    } while(id > 999999 || id < 100000);
		// Keeps loop running until correct ID is input

    for(int i = 0; i<length; i++){
				if(id == employees[i].id) {
						printf("%s\n", "Enter new first name: ");
          	scanf("%s", first_input);
          	strcpy(employees[i].first_name, first_input);
						break;
        } // searching for employee who matchs ID input and if found
					// prompts user to change the first name for that employee
        else if (i == length - 1) {
            printf("%s\n", "No person found");
        } // Error message if employee not found
    }
}

/*
 * Update last updates the last name of the chosen employees
 * @param array of employees and the the length of the array
 * @return void function, it modifies the chosen employees last name.
 */
void updateLast(Employee *employees, int length) {
    char last_input[MAXNAME];
    int id;
    do {
        printf("%s\n", "Enter ID of person to change: ");
        scanf("%d", &id);
        if (id > 999999 || id < 100000) {
            printf("%s\n", "Invalid id try again");
        }
    } while(id > 999999 || id < 100000);
		// Keeps loop running until correct ID is input

    for(int i = 0; i<length; i++){
        if(id == employees[i].id) {
            printf("%s\n", "Enter new last name: ");
            scanf("%s", last_input);
          	strcpy(employees[i].last_name, last_input);
						break;
        } // searching for employee who matchs ID input and if found
					// prompts user to change the last name for that employee
        else if (i == length - 1) {
            printf("%s\n", "No person found");
        } // Error message if matching ID is not found
    }
}

/*
 * Update salary updates the salary of the chosen employees
 * @param array of employees and the length of the array
 * @return an updated array of employees
 */
void updateSalary(Employee *employees, int length) {
    int id;
    do {
        printf("%s\n", "Enter ID of person to change: ");
        scanf("%d", &id);
        if (id > 999999 || id < 100000) {
            printf("%s\n", "Invalid id try again");
        }
    } while(id > 999999 || id < 100000);
		// Keeps loop running until correct ID is input

    for(int i = 0; i<length; i++){
        if(id == employees[i].id) {
            int sal_input;
            do {
                printf("%s\n", "Enter new salary: ");
                scanf("%d", &sal_input);
                if (sal_input > 150000 || sal_input < 30000) {
                    printf("%s\n", "Invalid id try again");
                }
            } while(sal_input > 150000 || sal_input < 30000);
            employees[i].salary = sal_input;
						break;
        } // searching for employee who matchs ID input and if found
					// prompts user to change the salary for that employee
        else if (i == length - 1) {
            printf("%s\n", "No person found");
        } // Error message if matching ID is not found
    }
}

/*
 * Update, updates all of the information of the chosen employees
 * @param the employee array and length of the array
 * @return returns an update array after altering the desired information
 */
void update(Employee *employees, int length) {
    int input;
    do {
				printf("%s\n", "1. Change the ID: ");
        printf("%s\n", "2. Change the first name: ");
        printf("%s\n", "3. Change the last name: ");
        printf("%s\n", "4. Change the salary: ");
        printf("%s\n", "5. Change all information: ");
        scanf("%d\n", &input);
        if (input < 1 || input > 5) {
            printf("%s\n", "invalid, try again");
        }
    } while(input < 1 || input > 5); // input validation

			// Menu asking which of the employees information the user wants to
			// alter
    switch(input) {
        case 1:
            updateID(employees, length);
            break;
        case 2:
            updateFirst(employees, length);
            break;
        case 3:
            updateLast(employees, length);
            break;
        case 4:
            updateSalary(employees, length);
            break;
        case 5:
            updateID(employees, length);
            updateFirst(employees, length);
            updateLast(employees, length);
            updateSalary(employees, length);
            break;
    }
}
