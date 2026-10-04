#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void budgetMenu(void);
void addDepartmentBudget(void);
int main(void) {

}
/* =====================================================
   SECTION 3: BUDGET MANAGEMENT
   ===================================================== */

/* ======================== LIMITS ======================== */
#define MAX_EMPLOYEES   50
#define MAX_DEPARTMENTS 20
#define MAX_SUPPLIERS   50
#define MAX_ASSETS      100

#define TEXT_LEN  50
#define SHORT_LEN 20

#define EMP_ID_START   101
#define SUP_ID_START   201
#define ASSET_ID_START 301

#define HIGH_INCOME_LIMIT 20000.0

#define SEARCH_BY_NAME 0
#define SEARCH_BY_TOWN 1

#define TYPE_COUNT      6
#define CONDITION_COUNT 4

/* ======================== HELPERS ======================== */

void trimSpaces(char text[]) {
    int start = 0;
    int end = (int)strlen(text) - 1;
    int i;

    while (end >= 0 && isspace((unsigned char)text[end])) end--;
    while (text[start] != '\0' && isspace((unsigned char)text[start])) start++;

    if (start > 0) {
        for (i = 0; text[start + i] != '\0'; i++) {
            text[i] = text[start + i];
        }
        text[i] = '\0';
    } else if (end >= 0) {
        text[end + 1] = '\0';
    } else {
        text[0] = '\0';
    }
}

void readline(char buffer[], int size) {
    int len, c;

    if (fgets(buffer, size, stdin) == NULL) {
        printf("\nNo more input. Exiting.\n");
        exit(0);
    }

    len = (int)strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF);
    }
    trimSpaces(buffer);
}

int isWholeNumber(const char text[]) {
    int i = 0, digits = 0;

    if (text[0] == '-' || text[0] == '+') i = 1;

    for (; text[i] != '\0'; i++) {
        if (!isdigit((unsigned char)text[i])) return 0;
        digits++;
    }
    return digits >= 1 && digits <= 9;
}

int isDecimalNumber(const char text[]) {
    int i = 0, digits = 0, dots = 0;

    if (text[0] == '-' || text[0] == '+') i = 1;

    for (; text[i] != '\0'; i++) {
        if (isdigit((unsigned char)text[i])) {
            digits++;
        } else if (text[i] == '.') {
            dots++;
            if (dots > 1) return 0;
        } else {
            return 0;
        }
    }
    return (digits >= 1 && digits <= 12) && (dots <= 1);
}

int readInt(const char prompt[], int minValue, int maxValue) {
    char line[TEXT_LEN];
    int value;

    while (1) {
        printf("%s", prompt);
        readline(line, TEXT_LEN);

        if (!isWholeNumber(line)) {
            printf("Invalid input. Please enter a whole number.\n");
            continue;
        }
        value = atoi(line);
        if (value >= minValue && value <= maxValue) return value;
        printf("Please enter a number between %d and %d.\n", minValue, maxValue);
    }
}

double readAmount(const char prompt[], const char label[]) {
    char line[TEXT_LEN];
    double value;

    while (1) {
        printf("%s", prompt);
        readline(line, TEXT_LEN);

        if (isDecimalNumber(line)) {
            value = strtod(line, NULL);
            if (value >= 0.0) return value;
        }
        printf("Invalid %s. Please enter a non-negative number.\n", label);
    }
}

void readRequiredText(const char prompt[], char buffer[], int size) {
    while (1) {
        printf("%s", prompt);
        readline(buffer, size);
        if (strlen(buffer) > 0) return;
        printf("Input cannot be empty. Please try again.\n");
    }
}

void toLowerCopy(const char source[], char destination[]) {
    int i;
    for (i = 0; source[i] != '\0'; i++) {
        destination[i] = (char)tolower((unsigned char)source[i]);
    }
    destination[i] = '\0';
}

int equalsIgnoreCase(const char first[], const char second[]) {
    char a[TEXT_LEN], b[TEXT_LEN];
    toLowerCopy(first, a);
    toLowerCopy(second, b);
    return strcmp(a, b) == 0;
}

int containsIgnoreCase(const char text[], const char part[]) {
    char t[TEXT_LEN], p[TEXT_LEN];
    toLowerCopy(text, t);
    toLowerCopy(part, p);
    return strstr(t, p) != NULL;
}

void printLine(int length) {
    int i;
    for (i = 0; i < length; i++) putchar('-');
    putchar('\n');
}

/* Parallel arrays for Budget module */
char   deptName[MAX_DEPARTMENTS][TEXT_LEN];   /* Department name */
double deptAllocated[MAX_DEPARTMENTS];        /* Allocated budget */
double deptSpent[MAX_DEPARTMENTS];            /* Total expenditure so far */
int    deptCount = 0;                         /* Number of departments */

/* Function prototypes */
void budgetMenu(void);
void addDepartmentBudget(void);
void enterExpenditure(void);
void displayBudgets(void);
void showOverBudgetDepartments(void);
int  findDepartment(const char name[]);
double calculateRemaining(double allocated, double spent);

/* -----------------------------------------------------
   Budget Menu
   ----------------------------------------------------- */
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
            case 1: addDepartmentBudget();          break;
            case 2: enterExpenditure();             break;
            case 3: displayBudgets();               break;
            case 4: showOverBudgetDepartments();    break;
            case 5: printf("Returning to Main Menu...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

/* -----------------------------------------------------
   1. Add a new departmental budget
   ----------------------------------------------------- */
void addDepartmentBudget(void) {
    char name[TEXT_LEN];
    double amount;

    if (deptCount >= MAX_DEPARTMENTS) {
        printf("Budget list is full. Cannot add more departments.\n");
        return;
    }

    readRequiredText("Enter department name: ", name, TEXT_LEN);

    if (findDepartment(name) != -1) {
        printf("Department already exists.\n");
        return;
    }

    amount = readAmount("Enter allocated budget (N$): ", "Budget");

    strcpy(deptName[deptCount], name);
    deptAllocated[deptCount] = amount;
    deptSpent[deptCount] = 0.0;          /* Start with zero expenditure */
    deptCount++;

    printf("Department budget added successfully.\n");
}

/* -----------------------------------------------------
   2. Enter / add expenditure for a department
   ----------------------------------------------------- */
void enterExpenditure(void) {
    char name[TEXT_LEN];
    double amount;
    double remaining;
    int index;

    if (deptCount == 0) {
        printf("No departments registered yet.\n");
        return;
    }

    readRequiredText("Enter department name: ", name, TEXT_LEN);
    index = findDepartment(name);

    if (index == -1) {
        printf("Department not found.\n");
        return;
    }

    amount = readAmount("Enter expenditure amount (N$): ", "Expenditure");

    deptSpent[index] += amount;          /* Add to existing expenditure */
    remaining = calculateRemaining(deptAllocated[index], deptSpent[index]);

    printf("Expenditure recorded successfully.\n");

    if (remaining < 0.0) {
        printf("WARNING: Department is OVER BUDGET by N$%.2f\n", -remaining);
    } else {
        printf("Remaining budget for %s: N$%.2f\n", deptName[index], remaining);
    }
}

/* -----------------------------------------------------
   3. Display all departments with budget status
   ----------------------------------------------------- */
void displayBudgets(void) {
    int i;
    double remaining;

    if (deptCount == 0) {
        printf("No budgets to display.\n");
        return;
    }

    printf("\n%-22s %12s %12s %12s  %s\n",
           "Department", "Allocated", "Spent", "Remaining", "Status");
    printLine(75);

    for (i = 0; i < deptCount; i++) {
        remaining = calculateRemaining(deptAllocated[i], deptSpent[i]);

        if (remaining >= 0.0) {
            printf("%-22.22s %12.2f %12.2f %12.2f  WITHIN BUDGET\n",
                   deptName[i], deptAllocated[i], deptSpent[i], remaining);
        } else {
            printf("%-22.22s %12.2f %12.2f %12.2f  OVER BUDGET\n",
                   deptName[i], deptAllocated[i], deptSpent[i], remaining);
        }
    }
}

/* -----------------------------------------------------
   4. Show only departments that exceeded their budget
   ----------------------------------------------------- */
void showOverBudgetDepartments(void) {
    int i;
    int found = 0;
    double remaining;

    if (deptCount == 0) {
        printf("No departments registered yet.\n");
        return;
    }

    printf("\n--- Over-Budget Departments ---\n");

    for (i = 0; i < deptCount; i++) {
        remaining = calculateRemaining(deptAllocated[i], deptSpent[i]);
        if (remaining < 0.0) {
            printf(" - %s  (Over by N$%.2f)\n", deptName[i], -remaining);
            found = 1;
        }
    }

    if (!found) {
        printf("No departments are currently over budget.\n");
    }
}

/* -----------------------------------------------------
   Find department by name (case-insensitive)
   Returns index or -1 if not found
   ----------------------------------------------------- */
int findDepartment(const char name[]) {
    int i;
    for (i = 0; i < deptCount; i++) {
        if (equalsIgnoreCase(deptName[i], name)) {
            return i;
        }
    }
    return -1;
}

/* -----------------------------------------------------
   Calculate remaining budget
   ----------------------------------------------------- */
double calculateRemaining(double allocated, double spent) {
    double remaining = allocated - spent;

    /* Avoid showing "over by 0.00" due to floating-point precision */
    if (remaining > -0.005 && remaining < 0.005) {
        remaining = 0.0;
    }
    return remaining;
   
 /*=====================================================
    SECTION 5: TEST MAIN (remove when integrating)
   ===================================================== */
#include <stdio.h>
int test_main(); {
    int choice = 0;
    
    while (choice != 2) {
        printf("\n1. Open Budget Management Menu\n");
        printf("2. Exit Test Driver\n");
        choice = readInt("Enter your choice: ", 1, 2);

        if (choice == 1) {
            budgetMenu();
        } else {
            printf("Goodbye.\n");
        }
    }

    return 0;
}
}