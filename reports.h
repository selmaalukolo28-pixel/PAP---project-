#ifndef REPORTS_H
#define REPORTS_H

#include "employees.h"
#include "budget.h"
#include "suppliers.h"
#include "assets.h"

/* Each report takes the array and how many items are actually in it. */
void employeeReport(const Employee emp[], int count);
void budgetReport(const Budget bud[], int count);
void supplierReport(const Supplier sup[], int count);
void assetReport(const Asset assets[], int count);

/* Menu for option 5 in the main menu. */
void displayReports(const Employee emp[], int empCount,
                    const Budget bud[], int budCount,
                    const Supplier sup[], int supCount,
                    const Asset assets[], int assetCount);

#endif
