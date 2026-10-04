#include <stdio.h>
#include "budget.h"


int main()
{
    int choice;
    choice = 0; 
    while(choice != 6) {
        printf("\n");
        printf("-----------------------------------------------\n");
        printf("   MUNICIPAL FINANCIAL MANAGEMENT SYSTEM\n");
        printf("-----------------------------------------------\n");
        printf("1. Employee Management\n");
        printf("2. Budget Management\n");
        printf("3. Supplier Management\n");
        printf("4. Asset Management\n");
        printf("5. Reports\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                employeeMenu();
                break;

            case 2:
                budgetMenu();
                break;

            case 3:
                supplierMenu();
                break;

            case 4:
                Asset Management();
                break;

            case 5:
                Reports selected();
                break;

            case 6:
                Exiting the system
                break;

            default:
                printf("Invalid choice. Please enter a number from 1 to 6.\n");
        }
    }

    return 0;
}




