#include <stdio.h>
#include <string.h>
#include "reports.h"

/* Total monthly pay of one employee: basic + housing + transport. */
static double totalSalary(const Employee *e)
{
    return e->basicSalary + e->housingAllowance + e->transportAllowance;
}

void employeeReport(const Employee emp[], int count)
{
    int i;
    double total = 0.0, highest, lowest, pay;

    printf("\n========== EMPLOYEE REPORT ==========\n");
    if (count == 0) {
        printf("No employees registered.\n");
        return;
    }

    highest = lowest = totalSalary(&emp[0]);
    for (i = 0; i < count; i++) {
        pay = totalSalary(&emp[i]);
        total += pay;
        if (pay > highest) highest = pay;
        if (pay < lowest)  lowest  = pay;
    }

    printf("Total Employees: %d\n", count);
    printf("Average Salary:  N$%.2f\n", total / count);
    printf("Highest Salary:  N$%.2f\n", highest);
    printf("Lowest Salary:   N$%.2f\n", lowest);
}

void budgetReport(const Budget bud[], int count)
{
    int i, exceeded = 0;
    double totalAlloc = 0.0, totalSpent = 0.0;

    printf("\n========== BUDGET REPORT ==========\n");
    if (count == 0) {
        printf("No budgets registered.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        totalAlloc += bud[i].allocated;
        totalSpent += bud[i].expenditure;
    }

    printf("Total Allocated Budget: N$%.2f\n", totalAlloc);
    printf("Total Expenditure:      N$%.2f\n", totalSpent);
    printf("Remaining Budget:       N$%.2f\n", totalAlloc - totalSpent);

    printf("\nDepartments exceeding budget:\n");
    for (i = 0; i < count; i++) {
        if (bud[i].expenditure > bud[i].allocated) {
            printf("  - %s (over by N$%.2f)\n", bud[i].department,
                   bud[i].expenditure - bud[i].allocated);
            exceeded++;
        }
    }
    if (exceeded == 0) printf("  None\n");
}

void supplierReport(const Supplier sup[], int count)
{
    int i;

    printf("\n========== SUPPLIER REPORT ==========\n");
    if (count == 0) {
        printf("No suppliers registered.\n");
        return;
    }

    printf("%-5s %-20s %-25s %-14s %-12s\n",
           "ID", "Name", "Email", "Phone", "Town");
    for (i = 0; i < count; i++) {
        printf("%-5d %-20s %-25s %-14s %-12s\n",
               sup[i].id, sup[i].name, sup[i].email,
               sup[i].phone, sup[i].town);
    }
    printf("Total suppliers: %d\n", count);
}

void assetReport(const Asset assets[], int count)
{
    int i, poor = 0;
    double totalValue = 0.0;

    printf("\n========== ASSET REPORT ==========\n");
    if (count == 0) {
        printf("No assets registered.\n");
        return;
    }

    printf("%-5s %-18s %-12s %-12s %-14s %-10s\n",
           "ID", "Name", "Type", "Value (N$)", "Department", "Condition");
    for (i = 0; i < count; i++) {
        printf("%-5d %-18s %-12s %-12.2f %-14s %-10s\n",
               assets[i].id, assets[i].name, assets[i].type,
               assets[i].value, assets[i].department, assets[i].condition);
        totalValue += assets[i].value;
        if (strcmp(assets[i].condition, "Poor") == 0) poor++;
    }
    printf("Total assets: %d\n", count);
    printf("Total value:  N$%.2f\n", totalValue);
    printf("Assets in Poor condition: %d\n", poor);
}

void displayReports(const Employee emp[], int empCount,
                    const Budget bud[], int budCount,
                    const Supplier sup[], int supCount,
                    const Asset assets[], int assetCount)
{
    int choice;
    char line[20];

    do {
        printf("\n========================================\n");
        printf("               REPORTS\n");
        printf("========================================\n");
        printf("1. Employee Report\n");
        printf("2. Budget Report\n");
        printf("3. Supplier Report\n");
        printf("4. Asset Report\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");

        /* fgets + sscanf so letters or empty input can't break the loop */
        if (fgets(line, sizeof line, stdin) == NULL ||
            sscanf(line, "%d", &choice) != 1) {
            choice = 0;
        }

        switch (choice) {
            case 1: employeeReport(emp, empCount);    break;
            case 2: budgetReport(bud, budCount);      break;
            case 3: supplierReport(sup, supCount);    break;
            case 4: assetReport(assets, assetCount);  break;
            case 5: break;
            default: printf("Invalid choice. Please enter 1-5.\n");
        }
    } while (choice != 5);
}
