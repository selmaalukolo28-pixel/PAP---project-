#include <stdio.h>
#include <string.h>
#include "employee.h"

int   empID[MAX_EMPLOYEES];
char  empName[MAX_EMPLOYEES][50];
char  empDepartment[MAX_EMPLOYEES][50];
float empBasic[MAX_EMPLOYEES];
float empHousing[MAX_EMPLOYEES];
float empTransport[MAX_EMPLOYEES];
int   empCount = 0;

void  addEmployee(void);
void  displayEmployees(void);
void  searchEmployee(void);
void  calculateSalaryInfo(void);
int   findEmployeeByName(char name[]);
float calculateGrossSalary(float basic, float housing, float transport);

void employeeMenu(void)
{
    int choice;

    do
    {
        printf("\n--- EMPLOYEE MANAGEMENT ---\n");
        printf("1. Add Employee\n");
        printf("2. Display All Employees\n");
        printf("3. Search Employee by Name\n");
        printf("4. Calculate Salary Information\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();   /* clear newline */

        switch (choice)
        {
            case 1: addEmployee();          break;
            case 2: displayEmployees();     break;
            case 3: searchEmployee();       break;
            case 4: calculateSalaryInfo();  break;
            case 5: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
}
void addEmployee(void)
{
    char  name[50];
    char  department[50];
    float basic;
    float housing;
    float transport;

    if (empCount >= MAX_EMPLOYEES)
    {
        printf("Employee list is full.\n");
        return;
    }

    printf("Enter employee name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    if (strlen(name) == 0)
    {
        printf("Name cannot be empty.\n");
        return;
    }

    printf("Enter department: ");
    fgets(department, sizeof(department), stdin);
    department[strcspn(department, "\n")] = '\0';

    if (strlen(department) == 0)
    {
        printf("Department cannot be empty.\n");
        return;
    }

    printf("Enter basic salary: ");
    scanf("%f", &basic);
    getchar();

    if (basic < 0)
    {
        printf("Basic salary cannot be negative.\n");
        return;
    }

    printf("Enter housing allowance: ");
    scanf("%f", &housing);
    getchar();

    if (housing < 0)
    {
        printf("Housing allowance cannot be negative.\n");
        return;
    }

    printf("Enter transport allowance: ");
    scanf("%f", &transport);
    getchar();

    if (transport < 0)
    {
        printf("Transport allowance cannot be negative.\n");
        return;
    }
    empID[empCount] = 100 + empCount + 1;  
    strcpy(empName[empCount], name);
    strcpy(empDepartment[empCount], department);
    empBasic[empCount]     = basic;
    empHousing[empCount]   = housing;
    empTransport[empCount] = transport;
    empCount++;

    printf("Employee added with ID %d.\n", empID[empCount - 1]);
}
void displayEmployees(void)
{
    int   i;
    float gross;

    if (empCount == 0)
    {
        printf("No employees to display.\n");
        return;
    }

    printf("\n%-6s %-20s %-15s %12s %12s %12s %12s\n",
           "ID", "Name", "Department",
           "Basic", "Housing", "Transport", "Gross");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (i = 0; i < empCount; i++)
    {
        gross = calculateGrossSalary(empBasic[i], empHousing[i], empTransport[i]);

        printf("%-6d %-20s %-15s %12.2f %12.2f %12.2f %12.2f\n",
               empID[i], empName[i], empDepartment[i],
               empBasic[i], empHousing[i], empTransport[i], gross);
    }
}
void searchEmployee(void)
{
    char name[50];
    int  index;

    if (empCount == 0)
    {
        printf("No employees yet.\n");
        return;
    }

    printf("Enter employee name to search: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    index = findEmployeeByName(name);

    if (index == -1)
    {
        printf("Employee not found.\n");
    }
    else
    {
        printf("\nEmployee found:\n");
        printf("ID         : %d\n", empID[index]);
        printf("Name       : %s\n", empName[index]);
        printf("Department : %s\n", empDepartment[index]);
        printf("Basic      : %.2f\n", empBasic[index]);
        printf("Housing    : %.2f\n", empHousing[index]);
        printf("Transport  : %.2f\n", empTransport[index]);
        printf("Gross      : %.2f\n",
               calculateGrossSalary(empBasic[index],
                                    empHousing[index],
                                    empTransport[index]));
    }
}
void calculateSalaryInfo(void)
{
    char  name[50];
    int   index;
    float gross;

    if (empCount == 0)
    {
        printf("No employees yet.\n");
        return;
    }

    printf("Enter employee name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    index = findEmployeeByName(name);

    if (index == -1)
    {
        printf("Employee not found.\n");
        return;
    }

    gross = calculateGrossSalary(empBasic[index],
                                 empHousing[index],
                                 empTransport[index]);

    printf("\n--- SALARY INFORMATION ---\n");
    printf("Employee   : %s\n", empName[index]);
    printf("Basic      : %.2f\n", empBasic[index]);
    printf("Housing    : %.2f\n", empHousing[index]);
    printf("Transport  : %.2f\n", empTransport[index]);
    printf("Gross      : %.2f\n", gross);

    if (gross >= 20000)
    {
        printf("Category   : High Income\n");
    }
    else
    {
        printf("Category   : Standard Income\n");
    }
}
int findEmployeeByName(char name[])
{
    int i;

    for (i = 0; i < empCount; i++)
    {
        if (strcmp(empName[i], name) == 0)
        {
            return i;
        }
    }

    return -1;
}
float calculateGrossSalary(float basic, float housing, float transport)
{
    return basic + housing + transport;
}




employee.h


#ifndef EMPLOYEES_H
#define EMPLOYEES_H

#define MAX_EMPLOYEES 50

void employeeMenu(void);

#endif
