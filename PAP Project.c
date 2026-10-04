/*
* PAP521S - Programming in Practice
* Project A: Municipal Financial Management System (MFMS)
* 
* Modules : 1. Employee 2. Budget 3. Supplier 4. Asset 5. Reports
* Storage : arrays (all data is kept in memory while the program runs)
*
* Compile : gcc -std=c99 -Wall -Wextra -pedantic "PAP Project.c" -o PAP521S_Project_A
* Run : ./PAP521S_Project_A
* Author : [Pharrell Mwiya 226013006, Selma Alukolo 225090082, Lisema Garoës 225030772, Guillermo Heita 225109190, Joseph Heita 225062453]
* Date : 2024-06-15
*/  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

/* Forward declaration used by input routines and menu modules. */
double readDouble(const char prompt[]);

/* Some C environments do not expose INFINITY unless implementation-specific
    floating-point extensions are enabled. Use a portable fallback so the
    constant is always available. */
#ifndef INFINITY
#define INFINITY DBL_MAX
#endif

/* -----------------------------------------------------------------------------
 LIMITS AND CONSTANTS
 * ----------------------------------------------------------------------------- */
#define MAX_EMPLOYEES 50
#define MAX_DEPARTMENTS 20
#define MAX_SUPPLIERS 50
#define MAX_ASSETS 100

#define TEXT_LEN 50    /* names, departments, job titles, e-mail, town */
#define SHORT_LEN 20   /* phone numbers, asset type, asset condition */

#define EMP_ID_START 101  /* IDs are generated: 101, 102, 103, ... */
#define SUP_ID_START 201  
#define ASSET_ID_START 301

#define HIGH_INCOME_LIMIT 20000.0

#define SEARCH_BY_NAME 0
#define SEARCH_BY_TOWN 1

#define TYPE_COUNT 6
#define CONDITION_COUNT 4

/* -----------------------------------------------------------------------------
DATA
Parallel arrays: index i in every array of a module is the same record.
Money is stored as double (float loses cents on large amounts).
----------------------------------------------------------------------------- */

/* Employee module */
int empID[MAX_EMPLOYEES];  /* unique employee ID */
char empName[MAX_EMPLOYEES][TEXT_LEN];  /* employee name */
char empDepartment[MAX_EMPLOYEES][TEXT_LEN];  /* department name */
char empPosition[MAX_EMPLOYEES][TEXT_LEN];  /* job title */
double empBasic[MAX_EMPLOYEES];  /* basic salary */
double empHousing[MAX_EMPLOYEES];  /* housing allowance */
double empTransport[MAX_EMPLOYEES];  /* transport allowance */
int empCount = 0;  /* number of employees */

/* Department module */
char deptName[MAX_DEPARTMENTS][TEXT_LEN];  /* department name */
double deptAllocated[MAX_DEPARTMENTS];  /* allocated budget */
int deptCount = 0;  /* number of departments */
double deptSpent [MAX_DEPARTMENTS];

/* Supplier module */
int supID[MAX_SUPPLIERS];  /* unique supplier ID */
char supName[MAX_SUPPLIERS][TEXT_LEN];  /* supplier name */
char supEmail[MAX_SUPPLIERS][TEXT_LEN];  /* supplier e-mail */
char supPhone[MAX_SUPPLIERS][SHORT_LEN];  /* supplier phone number */
char supTown[MAX_SUPPLIERS][TEXT_LEN];  /* supplier town */
int supCount = 0;  /* number of suppliers */

/* Asset module */
int assetID[MAX_ASSETS];  /* unique asset ID */
char assetName[MAX_ASSETS][TEXT_LEN];  /* asset name */
char assetType[MAX_ASSETS][SHORT_LEN];  /* asset type */
double assetValue[MAX_ASSETS];  /* asset value */
char assetDepartment[MAX_ASSETS][TEXT_LEN];  /* department that owns the asset */
char assetCondition[MAX_ASSETS][SHORT_LEN];  /* asset condition */
int assetCount = 0;  /* number of assets */

/* Lists the user can choose from when registering an asset */
const char assetTypeNames[TYPE_COUNT][SHORT_LEN] = {"Vehicle", "Equipment", "Furniture", "Computers", "Building", "Other"};
const char conditionNames[CONDITION_COUNT][SHORT_LEN] = {"New", "Good", "Fair", "Poor"};

/* Backward-compatible aliases for both the new and legacy identifiers. */
#define assetTypes assetTypeNames
#define assetTypeNameList assetTypeNames

/* ---------------------------------------------------
FUNCTION DECLARATIONS
---------------------------------------------------  */

/* Section 1 - input and string helpers */
void readline(char buffer[], int size);
void trimSpaces(char text[]);
int isWholeNumber(const char text[]);
int isDecimalNumber(const char text[]);
int readInt(const char prompt[], int minValue, int maxValue);
double readDouble(const char prompt[]);
double readAmount(const char prompt[], const char label[]);
void readRequiredText(const char prompt[], char buffer[], int size);
void toLowerCopy(const char source[], char destination[]);
int equalsIgnoreCase(const char first[], const char second[]);
int containsIgnoreCase(const char text[], const char part[]);
void printLine(int length);

/* Section 2 - Employee module */
void employeeMenu(void);
void addEmployee(void);
void displayEmployees(void);
void searchEmployees(void);
void calculateSalaryInfo(void);
void showEmployeeDetails(int index);
int findEmployeeByName(const char name[]);
int findEmployeeByID(int id);
double calculateGrossSalary(double basic, double housing, double transport);

/* Section 3 - Budget module */
void budgetMenu(void);
void addDepartmentBudget(void);
void enterExpenditure(void);
void displayBudgets(void);
void showOverBudgetDepartments(void);
int findDepartment(const char name[]) {
int i;
for (i = 0; i < deptCount; i++) {
if (equalsIgnoreCase(deptName[i], name)) {
return i;
}
}
return -1;
}
double calculateRemaining(double allocated, double expenditure);

/* Section 4 - Supplier module */
void supplierMenu(void);
void addSupplier(void);
void displaySuppliers(void);
void searchSuppliers(int searchBy);
void compareSuppliers(void);
void showSupplierDetails(int index);
int findSupplierByName(const char name[]);
int findSupplierByID(int id);
int isValidEmail(const char email[]);
int isValidPhone(const char phone[]);
void readEmail(char email[]);
void readPhone(char phone[]);

/* Section 5 - Asset module */
void assetMenu(void);
void addAsset(void);
void displayAssets(void);
void searchAssets(void);
void printAssetHeader(void);
void printAssetRow(int index);
void chooseFromList(const char title[], const char options[][SHORT_LEN], int count, char result[]);

/* Section 6 - Reports module */
void reportsMenu(void);
void employeeReport(void);
void budgetReport(void);
void supplierReport(void);
void assetReport(void);
void displayReports(void);

/* Section 7 - Main menu */
void displayMainMenu(void);
void displayMenu(void);

/* ============================================================================================================================
SECTION 1: Input and String Helpers
Every module reads its input through these functions, so numbers, empty text and invalid menu choices are checked in one place.
===============================================================================================================================  */

/* Removes spaces and tabs at the start and at the end of text. */
void trimSpaces(char text[]) {
    int start = 0;
    int end = (int)strlen(text) - 1;
    int i;

    while (end >= 0 && isspace((unsigned char)text[end])) {
        end--;
    }

    while (text[start] != '\0' && isspace((unsigned char)text[start])) {
        start++;
    }

    if (start > 0) {
        for (i = 0; text[start + i] != '\0'; i++) {
            text[i] = text[start + i];
        }
        text[i] = '\0';  
    }
}

/* Reads one line from the keyboard into buffer (at most size-1 characters).
  - removes the newline and the spaces around the text
  - throws away the rest of the line if it is longer than the buffer
  - stops the program if the input has ended (Ctrl+D / Ctrl+Z), so the program can never get stuck in an endless input loop */
   void readline(char buffer[], int size) {
    int len;
    int c;

    if (fgets(buffer, size, stdin) == NULL) {
        printf("\nNo more input. Exiting program.\n");
        exit(0);
    }
    len = (int)strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';  
    } else {
        while ((c = getchar()) != '\n' && c != EOF) {
            ;  
        }
    }

    trimSpaces(buffer);
   }

   /*Returns 1 if text is a whole number such a 7 or -12 (1 to 9 digits). */
   int isWholeNumber(const char text[]) {
    int i = 0;
    int digits = 0;

    if (text[0] == '-' || text[0] == '+') {
        i = 1;
    }

    for (; text[i] != '\0'; i++) {
        if (!isdigit((unsigned char)text[i])) {
            return 0;  
        }
        digits++;
    }

    return digits >= 1 && digits <= 9;
}

/* Returns 1 if test is a decimal number such as 250cor 1500.75.
Only digits and at most one decimal point are accepted.*/
int isDecimalNumber(const char text[]) {
    int i = 0;
    int digits = 0;
    int dots = 0;

    if (text[0] == '-' || text[0] == '+') {
        i = 1;
    }

    for (; text[i] != '\0'; i++) {
        if (isdigit((unsigned char)text[i])) {
            digits++;
        } else if (text[i] == '.') {
            dots++;
            if (dots > 1) {
                return 0;  
            }
        } else {
            return 0;  
        }
    }

    return (digits >= 1 && digits <= 9) && (dots <= 1);
}

/* Asks until the user enters a whole number from minValue to maxValue (inclusive) */
int readInt(const char prompt[], int minValue, int maxValue) {
    char line[TEXT_LEN];
    int value;

    while(1) {
        printf("%s", prompt);
        readline(line, TEXT_LEN);

        if (!isWholeNumber(line)) {
            printf("Invalid input. Please enter a whole number.\n");
        } else {
            value = atoi(line);
            if (value >= minValue && value <= maxValue) {
                return value;
            }

            printf("Please enter a number between %d and %d.\n", minValue, maxValue);
        }
    }
}

/* Asks until the user enters a valid non-negative monetary amount. */
double readAmount(const char prompt[], const char label[]) {
    char line[TEXT_LEN];
    double value;

    while (1) {
        printf("%s", prompt);
        readline(line, TEXT_LEN);

        if (isDecimalNumber(line)) {
            value = strtod(line, NULL);
            if (value >= 0.0) {
                return value;
            }
        }

        printf("Invalid %s. Please enter a non-negative number.\n", label);
    }
}

/* Reads a decimal number from the user. Keep this wrapper available to all
   modules through the declaration above. */
double readDouble(const char prompt[]) {
    return readAmount(prompt, "number");
}

/* Asks until the user enters text that is not empty.
buffer must have room for size characters. */
void readRequiredText(const char prompt[], char buffer[], int size) {
    while (1) {
        printf("%s", prompt);
        readline(buffer, size);

        if (strlen(buffer) > 0) {
            return;  
        }

        printf("Input cannot be empty. Please try again.\n");
    }
}

/* Copies source into destination, converted to lower case. */
void toLowerCopy(const char source[], char destination[]) {
    int i;
    for (i = 0; source[i] != '\0'; i++) {
        destination[i] = (char)tolower((unsigned char)source[i]);
    }
    
destination[i] = '\0';
}

/* Returns 1 if both texts are the same, ignoring upper/lower case.*/
int equalsIgnoreCase(const char first[], const char second[]) {
    char a[TEXT_LEN];
    char b[TEXT_LEN];

    toLowerCopy(first, a);
    toLowerCopy(second, b);

    return strcmp(a, b) == 0;
}

/* Returns 1 if text contains part, ignoring upper/lower case.*/
int containsIgnoreCase(const char text[], const char part[]) {
    char t[TEXT_LEN];
    char p[TEXT_LEN];

    toLowerCopy(text, t);
    toLowerCopy(part, p);

    return strstr(t, p) != NULL;
}

/* Prints a line of '-' characters, used under table headings. */
void printLine(int length) {
    int i;
    for (i = 0; i < length; i++) {
        printf("-");
    }
    printf("\n");
}

/* =====================================================
SECTION 2: EMPLOYEE MANAGEMENT
=====================================================*/

void employeeMenu(void) {
    int choice;

    do {
        printf("\n--- Employee Management Menu ---\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee (by name or ID)\n");
        printf("4. Calculate Salary Info\n");
        printf("5. Return to Main Menu\n");
        choice = readInt("Enter your choice (1-5): ", 1, 5);

        switch (choice) {
            case 1:
                addEmployee();
                break;
            case 2:
                displayEmployees();
                break;
            case 3:
                searchEmployees();
                break;
            case 4:
                calculateSalaryInfo();
                break;
            case 5:
                printf("Returning to Main Menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

void addEmployee(void) {
    char name[TEXT_LEN];
    char department[TEXT_LEN];
    char position[TEXT_LEN];
    double basic, housing, transport;

    if (empCount >= MAX_EMPLOYEES) {
        printf("Cannot add more employees. Maximum limit reached.\n");
        return;
    }

    readRequiredText("Enter employee name: ", name, TEXT_LEN);
    readRequiredText("Enter department: ", department, TEXT_LEN);
    readRequiredText("Enter position: ", position, TEXT_LEN);
    basic = readAmount("Enter basic salary: ", "Basic salary");
    housing = readAmount("Enter housing allowance: ", "Housing allowance");
    transport = readAmount("Enter transport allowance: ", "Transport allowance");

    /*very text was read with room for TEXT_LEN characters, so strcpy is safe */
    empID[empCount] = EMP_ID_START + empCount;
    strcpy(empName[empCount], name);
    strcpy(empDepartment[empCount], department);
    strcpy(empPosition[empCount], position);
    empBasic[empCount] = basic;
    empHousing[empCount] = housing;
    empTransport[empCount] = transport;
    empCount++;

    printf("Employee added successfully with ID: %d\n", empID[empCount - 1]);
}

void displayEmployees(void) {
    int i;
    double gross;

    if (empCount == 0) {
        printf("No employees to display.\n");
        return;
    }

    /* %-18.18s prints at most 18 characters, so long text cannot break the table */
    printf("\n%-4s %-18s %-13s %-14s %-10s %-10s %-10s %-10s\n",
           "ID", "Name", "Department", "Position", "Basic", "Housing", "Transport", "Gross");
    printLine(97);

    for (i = 0; i < empCount; i++) {
        gross = calculateGrossSalary(empBasic[i], empHousing[i], empTransport[i]);
        printf("%-4d %-18.18s %-13.13s %-14.14s %-10.2f %-10.2f %-10.2f %-10.2f\n",
               empID[i], empName[i], empDepartment[i], empPosition[i],
               empBasic[i], empHousing[i], empTransport[i], gross);
    }
}

/* Prints all details of the employee stored at position index. */
void showEmployeeDetails(int index) {
    double gross;

    gross = calculateGrossSalary(empBasic[index], empHousing[index], empTransport[index]);

    printf("ID: %d\n", empID[index]);
    printf("Name: %s\n", empName[index]);
    printf("Department: %s\n", empDepartment[index]);
    printf("Position: %s\n", empPosition[index]);
    printf("Basic Salary: %.2f\n", empBasic[index]);
    printf("Housing Allowance: %.2f\n", empHousing[index]);
    printf("Transport Allowance: %.2f\n", empTransport[index]);
    printf("Gross Salary: %.2f\n", gross);
}

/* Search by ID (exact) or by name (any part of the name, upper/lower case ignored). */
void searchEmployees(void) {
    char text[TEXT_LEN];
    int i;
    int index;
    int found = 0;

    if (empCount == 0) {
        printf("No employees yet.\n");
        return;
    }

    readRequiredText("Enter employee name (or part of it) or ID to search: ", text, TEXT_LEN);

    if (isWholeNumber(text)) {
        index = findEmployeeByID(atoi(text));
        if (index != -1) {
            printf("\n");
            showEmployeeDetails(index);
            found = 1;
        }
    } else {
        for (i = 0; i < empCount; i++) {
            if (containsIgnoreCase(empName[i], text)) {
                printf("\n");
                showEmployeeDetails(i);
                found++;
            }
        }
    }

    if (found == 0) {
        printf("Employee not found.\n");
    } else {
        printf("\n%d employee(s) found.\n", found);
    }
}

void calculateSalaryInfo(void) {
    char text[TEXT_LEN];
    int index;
    double gross;

    if (empCount == 0) {
        printf("No employees yet.\n");
        return;
    }

    readRequiredText("Enter employee full name or ID: ", text, TEXT_LEN);

    if (isWholeNumber(text)) {
        index = findEmployeeByID(atoi(text));
    } else {
        index = findEmployeeByName(text);
    }

    if (index == -1) {
        printf("Employee not found.\n");
        return;
    }

    gross = calculateGrossSalary(empBasic[index], empHousing[index], empTransport[index]);
    printf("\n--- Salary Information ---\n");
    printf("Employee : %s\n", empName[index]);
    printf("Basic Salary: %.2f\n", empBasic[index]);
    printf("Housing Allowance: %.2f\n", empHousing[index]);
    printf("Transport Allowance: %.2f\n", empTransport[index]);
    printf("Gross Salary: %.2f\n", gross);

    if (gross >= HIGH_INCOME_LIMIT) {
        printf("Category: High Income\n");
    } else {
        printf("Category: Standard\n");
    }
}

/*Returns the position of the employee with this exact name (upper/lower case ignored), or -1 if there is none. */
int findEmployeeByName(const char name[]) {
    int i;
    for (i = 0; i < empCount; i++) {
        if (equalsIgnoreCase(empName[i], name)) {
            return i;
        }
    }
    return -1;
}

/* Returns the position of the employee with this ID, or -1 if there is none. */
int findEmployeeByID(int id) {
    int i;
    for (i = 0; i < empCount; i++) {
        if (empID[i] == id) {
            return i;
        }
    }
    return -1;
}

double calculateGrossSalary(double basic, double housing, double transport) {
    return basic + housing + transport;
}

/* =====================================================
SECTION 3: BUDGET MANAGEMENT
=====================================================*/

    void budgetMenu(void) {
        int choice;

        do {
            printf("\n--- Budget Management Menu ---\n");
            printf("1. Add Department Budget\n");
            printf("2. Enter Expenditure\n");
            printf("3. Display All Budgets\n");
            printf("4. Show Over-Budget Departments\n");
            printf("5. Return to Main Menu\n");
            choice = readInt("Enter your choice (1-5): ", 1, 5);

            switch (choice) {
                case 1:
                    addDepartmentBudget();
                    break;
                case 2:
                    enterExpenditure();
                    break;
                case 3:
                    displayBudgets();
                    break;
                case 4:
                    showOverBudgetDepartments();
                    break;
                case 5:
                    printf("Returning to Main Menu...\n");
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 5);
    }

void addDepartmentBudget(void) {
    char name[TEXT_LEN];
    double amount;

    if (deptCount >= MAX_DEPARTMENTS) {
        printf("Budget list is full.\n");
        return;
    }

    readRequiredText("Enter department name: ", name, TEXT_LEN);

    if (findDepartment(name) != -1) {
        printf("Department already exists.\n");
        return;
    }

    amount = readAmount("Enter allocated budget: ", "Budget");

    strcpy(deptName[deptCount], name);
    deptAllocated[deptCount] = amount;
    deptSpent[deptCount] = 0.0;  /* Initialize expenditure to 0 */
    deptCount++;

    printf("Department budget added successfully.\n");
}

void enterExpenditure(void) {
    char name[TEXT_LEN];
    double amount;
    double remaining;
    int index;

    if (deptCount == 0) {
        printf("No departments yet.\n");
        return;
    }

    readRequiredText("Enter department name: ", name, TEXT_LEN);
    index = findDepartment(name);

    if (index == -1) {
        printf("Department not found.\n");
        return;
    }

    amount = readAmount("Enter expenditure amount: ", "Expenditure");

    deptSpent[index] = deptSpent[index] + amount;
    remaining = calculateRemaining(deptAllocated[index], deptSpent[index]);

    printf("Expenditure recorded.\n");

    if (remaining < 0) {
        printf("Warning: Department is over budget by %.2f\n", -remaining);
    } else {
        printf("Remaining budget for %s: %.2f\n", deptName[index], remaining);
    }
}

void displayBudgets(void) {
    int i;
    double remaining;

    if (deptCount == 0) {
        printf("No budgets to display.\n");
        return;
    }

    printf("\n%-20s %-12s %12s %12s %s\n", "Department", "Allocated", "Spent", "Remaining", "Status");
    printLine(74);

    for (i = 0; i < deptCount; i++) {
        remaining = calculateRemaining(deptAllocated[i], deptSpent[i]);
        if (remaining >= 0) {
            printf("%-20.20s %12.2f %12.2f %12.2f WITHIN BUDGET\n", deptName[i], deptAllocated[i], deptSpent[i], remaining);
        }
        else {
            printf("%-20.20s %12.2f %12.2f %12.2f OVER BUDGET\n", deptName[i], deptAllocated[i], deptSpent[i], remaining);
        }
    }
}

void showOverBudgetDepartments(void) {
    int i;
    int found = 0;
    double remaining;

    if (deptCount == 0) {
        printf("No departments yet.\n");
        return;
    }

    printf("\nOver-Budget Departments:\n");

    for (i = 0; i < deptCount; i++) {
        remaining = calculateRemaining(deptAllocated[i], deptSpent[i]);
        if (remaining < 0) {
            printf(" - %s (Over by %.2f)\n", deptName[i], -remaining);
            found = 1;
        }
    }

    if (!found) {
        printf("No departments are over budget.\n");
    }
}

/* Remaining budget = allocated - spent. Differences smaller than half a cent are treated as 0, so rounding errors never show as "over budget by o.00. */
double calculateRemaining(double allocated, double spent) {
    double remaining = allocated - spent;
    if (remaining > -0.005 && remaining < 0.005) {
        remaining = 0.0;
    }
    return remaining;
}

/* ======================================================
SECTION 4: SUPPLIER MANAGEMENT
====================================================== */

void supplierMenu(void) {
    int choice;

    do {
        printf("\n--- Supplier Management Menu ---\n");
        printf("1. Add Supplier\n");
        printf("2. Display All Suppliers\n");
        printf("3. Search Supplier by Name\n");
        printf("4. Search Supplier by Town/or Location\n");
        printf("5. Compare Two Suppliers\n");
        printf("6. Return to Main Menu\n");
        choice = readInt("Enter your choice: ", 1, 6);

        switch (choice) {
            case 1:
                addSupplier();
                break;
            case 2:
                displaySuppliers();
                break;
            case 3:
                searchSuppliers(SEARCH_BY_NAME);
                break;
            case 4:
                searchSuppliers(SEARCH_BY_TOWN);
                break;
            case 5:
                compareSuppliers();
                break;
            case 6:
                printf("Returning to Main Menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

void addSupplier(void) {
    char name[TEXT_LEN];
    char email[TEXT_LEN];
    char phone[SHORT_LEN];
    char town[TEXT_LEN];

    if (supCount >= MAX_SUPPLIERS) {
        printf("Cannot add more suppliers. Maximum limit reached.\n");
        return;
    }

    readRequiredText("Enter supplier name: ", name, TEXT_LEN);
    
    if (findSupplierByName(name) != -1) {
        printf("Supplier already exists.\n");
        return;
    }

    readEmail(email);
    readPhone(phone);
    readRequiredText("Enter town/location: ", town, TEXT_LEN);

    /* email and town were read with room for TEXT_LEN characters and the phone number was checked to fit in SHORT_LEN, so strcpy is safe */
    supID[supCount] = SUP_ID_START + supCount;
    strcpy(supName[supCount], name);
    strcpy(supEmail[supCount], email);
    strcpy(supPhone[supCount], phone);
    strcpy(supTown[supCount], town);
    supCount++;

    printf("Supplier added with ID %d.\n", supID[supCount - 1]);
}

void displaySuppliers(void) {
    int i;

    if (supCount ==0) {
        printf("No suppliers to display.\n");
        return;
    }

    printf("\n%-4s %-26s %-19s %-16s %s\n", "ID", "Supplier Name", "Telephone", "Town/Location", "Email");
    printLine(94);

    for (i = 0; i < supCount; i++) {
        printf("%-4d %-26.26s %-19s %-16.16s %s\n", supID[i], supName[i], supPhone[i], supTown[i], supEmail[i]);
    }
}

/* Prints all details of the supplier stored at position index. */
void showSupplierDetails(int index) {
    printf("ID : %d\n", supID[index]);
    printf("Name : %s\n", supName[index]);
    printf("Email : %s\n", supEmail[index]);
    printf("Telephone : %s\n", supPhone[index]);
    printf("Town : %s\n", supTown[index]);
}

/* Lists every supplier whose name (or town) contains the text the user types.
searchBy is SEARCH_BY_NAME or SEARCH_BY_TOWN. */
void searchSuppliers(int searchBy) {
    char text[TEXT_LEN];
    int i;
    int found = 0;
    int match;

    if (supCount == 0) {
        printf("No suppliers yet.\n");
        return;
    }

    if (searchBy == SEARCH_BY_TOWN) {
        readRequiredText("Enter town/location (or part of it): ", text, TEXT_LEN);
    }
    else {
        readRequiredText("Enter supplier name (or part of it): ", text, TEXT_LEN);
    }

    for (i = 0; i < supCount; i++) {
        if (searchBy == SEARCH_BY_TOWN) {
            match = containsIgnoreCase(supTown[i], text);
        }
        else {
            match = containsIgnoreCase(supName[i], text);
        }

        if (match) {
            printf("\n");
            showSupplierDetails(i);
            found++;
        }
    }

    if (found == 0) {
        printf("No supplier found.\n");
    }
    else {
        printf("\n%d supplier(s) found.\n", found);
    }
}

/* Shows two suppliers one after the other and compares their towns. */
void compareSuppliers(void) {
    int id;
    int first;
    int second;

    if (supCount < 2) {
        printf("At least two suppliers are needed to compare.\n");
        return;
    }

    id = readInt("Enter the ID of the first supplier: ", 1, 999999999);
    first = findSupplierByID(id);

    if (first == -1) {
        printf("Supplier not found.\n");
        return;
    }

    id = readInt("Enter the ID of the second supplier: ", 1, 999999999);
    second = findSupplierByID(id);

    if (second == -1 || second == first) {
        printf("Please choose two different suppliers.\n");
        return;
    }

    printf("\n--- SUPPLIER 1 ---\n");
    showSupplierDetails(first);
    printf("\n--- SUPPLIER 2 ---\n");
    showSupplierDetails(second);

    printf("\n--- COMPARISON ---\n");

    if (equalsIgnoreCase(supTown[first], supTown[second])) {
        printf("Both suppliers are in the same town: %s\n", supTown[first]);
    }
    else {
        printf("Different towns: %s and %s\n", supTown[first], supTown[second]);
    }
}

/* Returns the position of the supplier with this name (upper/lower case ignored), or -1 if there is none. */
int findSupplierByName(const char name[]) {
    int i;

    for (i = 0; i < supCount; i++) {
        if (equalsIgnoreCase(supName[i], name)) {
            return i;
        }
    }

    return -1;
}

/* Returns the position of the supplier with this ID, or -1 if there is none. */
int findSupplierByID(int id) {
    int i;

    for (i = 0; i < supCount; i++) {
        if (supID[i] == id) {
            return i;
        }
    }

    return -1;
}

/* An e-mail address needs: no spaces, exactly one '@' with text before it, and a '.' after the '@' with the text on both sides (e.g. sales@company.com). */
int isValidEmail(const char email[]) {
    int i;
    int length = (int)strlen(email);
    int atCount = 0;
    int atPosition = -1;
    int lastDot = -1;

    for (i = 0; i < length; i++) {
        if (isspace((unsigned char)email[i])) {
            return 0;
        }

        if (email[i] == '@') {
            atCount++;
            atPosition = i;
        }
    }

    if (atCount != 1 || atPosition == 0) {
        return 0;
    }

    for (i = atPosition + 1; i < length; i++) {
        if (email[i] == '.') {
            lastDot = i;
        }
    }

    if (lastDot == -1 || lastDot == atPosition + 1 || lastDot == length -1) {
        return 0;
    }

    return 1;
}

/* Asks until a valid e-mail address is entered. email needs room for TEXT_LEN. */
void readEmail(char email[]) {
    char line[TEXT_LEN];

    while (1) {
        readRequiredText("Enter e-mail address: ", line, TEXT_LEN);

        if (isValidEmail(line)) {
            strcpy(email, line);
            return;
        }

        printf("Invalid e-mail address. Use format name@company.com\n");
    }
}

/* A telephone number may start with '+'and contain digits, spaces and '-'.
It needs 7 to 15 digits and must fit in SHORT_LEN characters. */
int isValidPhone(const char phone[]) {
    int i;
    int start = 0;
    int digits = 0;
    int length = (int)strlen(phone);

    if (length > SHORT_LEN - 1) {
        return 0;
    }

    if (phone[0] == '+') {
        start = 1;
    }

    for (i = start; i < length; i++) {
        if (isdigit((unsigned char)phone[i])) {
            digits++;
        }
        else if (phone[i] !=' ' && phone[i] != '-') {
            return 0;
        }
    }

    return (digits >= 7 && digits <= 15);
}

/* Asks until a valid telephone number is entered. phone needs room for SHORT_LEN. */
void readPhone(char phone[]) {
    char line[TEXT_LEN];

    while (1) {
        readRequiredText("Enter telephone number: ", line, TEXT_LEN);

        if (isValidPhone(line)) {
            strcpy(phone, line);
            return;
        }

        printf("Invalid telephone number. Use 7 to 15 digits, for example +264 61 123 4567\n");
    }
}

/*============================================================
SECTION 5: ASSET MANAGEMENT
==============================================================*/

void assetMenu(void) {
    int choice;

    do {
        printf("\n--- ASSET MANAGEMENT ---\n");
        printf("1. Add Asset\n");
        printf("2. Display All Assets\n");
        printf("3. Search Assets\n");
        printf("4. Back to Main Menu\n");
        choice = readInt("Enter your choice: ", 1, 4);

        switch (choice) {
            case 1: addAsset(); break;
            case 2: displayAssets(); break;
            case 3: searchAssets(); break;
            case 4: printf("Returning to main menu...\n");
            break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);
}

void addAsset(void) {
    char name[TEXT_LEN];
    char type[SHORT_LEN];
    char department[TEXT_LEN];
    char condition[SHORT_LEN];
    double value;

    if (assetCount >= MAX_ASSETS) {
        printf("Asset register is full.\n");
        return;
    }

    readRequiredText("Enter asset name: ", name, TEXT_LEN);
    chooseFromList("Asset type", assetTypes, TYPE_COUNT, type);
    value = readAmount("Enter purchase value: ", "Purchase value");
    readRequiredText("Enter department: ", department, TEXT_LEN);
    chooseFromList("Asset condition", conditionNames, CONDITION_COUNT, condition);

    assetID[assetCount] = ASSET_ID_START + assetCount;
    strcpy(assetName[assetCount], name);
    strcpy(assetType[assetCount], type);
    assetValue[assetCount] = value;
    strcpy(assetDepartment[assetCount], department);
    strcpy(assetCondition[assetCount], condition);
    assetCount++;

    printf("Asset added with ID %d.\n", assetID[assetCount - 1]);
}

/* Shows a numbered list, lets the user pick one item and copies the chosen text into result (result needs room for SHORT_LEN characters). */
void chooseFromList(const char title[], const char options[][SHORT_LEN], int count, char result[]) {
    int i;
    int choice;

    printf("%s:\n", title);

    for (i = 0; i < count; i++) {
        printf(" %d. %s\n", i + 1, options[i]);
    }

    choice = readInt("Enter your choice: ", 1, count);
    strcpy(result, options[choice - 1]);
}

void printAssetHeader(void) {
    printf("\n%-4s %-22s %-10s %14s %-16s %s\n", "ID", "Asset Name", "Type", "Value (N$)", "Department", "Condition");
    printLine(80);
}

void printAssetRow(int index) {
    printf("%-4d %-22.22s %-10s %14.2f %-16.16s %s\n", assetID[index], assetName[index], assetType[index], assetValue[index], assetDepartment[index], assetCondition[index]);
}

void displayAssets(void) {
    int i;
    if (assetCount == 0) {
        printf("No assets to display.\n");
        return;
    }

    printAssetHeader();

    for (i = 0; i < assetCount; i++) {
        printAssetRow(i);
    }
}

/* Lists every asset whose name, type or department contains the search text. */
void searchAssets(void) {
    char text[TEXT_LEN];
    int i;
    int found = 0;

    if (assetCount == 0) {
        printf("No assets yet.\n");
        return;
    }

    readRequiredText("Enter asset name, type or department to search: ", text, TEXT_LEN);

    for (i = 0; i < assetCount; i++) {
        if (containsIgnoreCase(assetName[i], text) || containsIgnoreCase(assetType[i], text) || containsIgnoreCase(assetDepartment[i], text)) {
            if (found == 0) {
                printAssetHeader();
            }

            printAssetRow(i);
            found++;
        }
    }

    if (found == 0) {
        printf("No matching assets found.\n");
    }
    else {
        printf("\n%d asset(s) found.\n", found);
    }
}

/*================================================================================
SECTION 6: REPORTS
================================================================================== */

void reportsMenu(void) {
    int choice;

    do {
        printf("\n-- REPORTS ---\n");
        printf("1. Employee Report\n");
        printf("2. Budget Report\n");
        printf("3. Supplier Report\n");
        printf("4. Asset Report\n");
        printf("5. Full Report (all of the above)\n");
        printf("6. Back to Main Menu\n");
        choice = readInt("Enter your choice: ", 1, 6);

        switch (choice) {
            case 1: employeeReport();
            break;
            case 2: budgetReport();
            break;
            case 3: supplierReport();
            break;
            case 4: assetReport(); 
            break;
            case 5: displayReports();
            break;
            case 6: printf("Returning to main menu...\n");
            break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);
}

/* Total, average, highest and lowest gross salary. */
void employeeReport(void) {
    int i;
    int highIndex = 0;
    int lowIndex = 0;
    double gross;
    double highest;
    double lowest;
    double total = 0;

    printf("\n--- EMPLOYEE REPORT ---\n");

    if (empCount == 0) {
        printf("No employees registered.\n");
        return;
    }

    highest = calculateGrossSalary(empBasic[0], empHousing[0], empTransport[0]);
    lowest = highest;

    for (i = 0; i < empCount; i++) {
        gross = calculateGrossSalary(empBasic[i], empHousing[i], empTransport[i]);
        total += gross;

        if (gross > highest) {
            highest = gross;
            highIndex = i;
        }

        if (gross < lowest) {
            lowest = gross;
            lowIndex = i;
        }
    }

    printf("Total Employees      : %d\n", empCount);
    printf("Average Gross Salary : N$%.2f\n", total / empCount);
    printf("Highest Gross Salary : N$%.2f (%s)\n", highest, empName[highIndex]);
    printf("Lowest Gross Salary  : N$%.2f (%s)\n", lowest, empName[lowIndex]);
    printf("Total Gross Salaries : N$%.2f\n", total);
}

/* Total allocated, total spent, total remaining and the departments over budget. */
void budgetReport(void) {
    int i;
    int overCount = 0;
    double totalAllocated = 0;
    double totalSpent = 0;
    double totalRemaining;
    char overList[MAX_DEPARTMENTS * (TEXT_LEN + 2) + 1]; /* names separated by ", " */

    printf("\n--- BUDGET REPORT ---\n");

    if (deptCount == 0) {
        printf("No budgets registered.\n");
        return;
    }

    overList[0] = '\0';

    for (i = 0; i < deptCount; i++) {
        totalAllocated = totalAllocated + deptAllocated[i];
        totalSpent = totalSpent + deptSpent[i];

        if (calculateRemaining(deptAllocated[i], deptSpent[i]) < 0) {
            if (overCount > 0) {
                strcat(overList, ", ");
            }

            strcat(overList, deptName[i]);
            overCount++;
        }
    }

    totalRemaining = calculateRemaining(totalAllocated, totalSpent);

    printf("Departments Registered : %d\n", deptCount);
    printf("Total Allocated Budget : N$%.2f\n", totalAllocated);
    printf("Total Expenditure      : N$%.2f\n", totalSpent);

    if (totalRemaining < 0) {
        printf("Total Remaining Budget  : -N$%.2f (overspent)\n", -totalRemaining);
    }
    else {
        printf("Total Remaining Budget  : N$%.2f\n", totalRemaining);
    }

    if (overCount == 0) {
        printf("Departments Over Budget : None\n");
    }
    else {
        printf("Departments Over Budget : %d (%s)\n", overCount, overList);
    }
}

void supplierReport(void) {
    printf("\n--- SUPPLIER REPORT ---\n");
    displaySuppliers();

    if (supCount > 0) {
        printf("\nTotal suppliers registered: %d\n", supCount);
    }
}

/* Register of assets, total value and a summary per asset type. */
void assetReport(void) {
    int i;
    int t;
    int count;
    double sum;
    double total = 0;

    printf("\n--- ASSET REPORT ---\n");
    displayAssets();

    if (assetCount == 0) {
        return;
    }

    for (i = 0; i < assetCount; i++) {
        total = total + assetValue[i];
    }

    printf("\nTotal assets registered : %d\n", assetCount);
    printf("Total asset value   : N$%.2f\n", total);

    printf("\nAssets by type:\n");

    for (t = 0; t < TYPE_COUNT; t++) {
        count = 0;
        sum = 0;

        for (i = 0; i < assetCount; i++) {
            if (strcmp(assetType[i], assetTypes[t]) == 0) {
                count++;
                sum = sum + assetValue[i];
            }
        }

        if (count > 0) {
            printf(" %-10s : %d asset(s), N$%.2f\n", assetTypes[t], count, sum);
        }
    }
}

void displayReports(void) {
    employeeReport();
    budgetReport();
    supplierReport();
    assetReport();
}

/*=============================================================================
SECTION 7: MAIN MENU
=============================================================================== */

void displayMenu(void) {
    printf("\n");
    printf("----------------------------------------------\n");
    printf("  MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");
    printf("----------------------------------------------\n");
    printf("1. Employee Management\n");
    printf("2. Budget Management\n");
    printf("3. Supplier Management\n");
    printf("4. Asset Management\n");
    printf("5. Reports\n");
    printf("6. Exit\n");
    printf("\n");
}

int main(void) {
    int choice =0;

    while (choice != 6) {
        displayMenu();
        choice = readInt("Enter your choice: ", 1, 6);

        switch (choice) {
            case 1: employeeMenu();
            break;
            case 2: budgetMenu();
            break;
            case 3: supplierMenu();
            break;
            case 4: assetMenu();
            break;
            case 5: reportsMenu();
            break;
            case 6: printf("Exiting the system...\n");
            break;
            default: printf("Invalid choice. Please enter a number from 1 to 6.\n");
        }
    }

    return 0;
}
