#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "suppliers.h"

static Supplier suppliers[MAX_SUPPLIERS];
static int supplierCount = 0;

/* ---------- helpers ---------- */

static void readLine(const char *prompt, char *buf, int size)
{
    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\n")] = '\0';
}

/* Keeps asking until the text is not empty */
static void readRequired(const char *prompt, char *buf, int size)
{
    do {
        readLine(prompt, buf, size);
        if (strlen(buf) == 0)
            printf("  Error: this field cannot be empty.\n");
    } while (strlen(buf) == 0);
}

static int readInt(const char *prompt)
{
    char line[32];
    int value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1 && value > 0)
            return value;
        printf("  Error: enter a positive whole number.\n");
    }
}

static int isValidEmail(const char *email)
{
    const char *at = strchr(email, '@');
    return at != NULL && at != email && strchr(at, '.') != NULL;
}

static int isValidPhone(const char *phone)
{
    size_t i;
    if (strlen(phone) < 7)
        return 0;
    for (i = 0; i < strlen(phone); i++) {
        if (!isdigit((unsigned char)phone[i]) && phone[i] != '+' && phone[i] != ' ')
            return 0;
    }
    return 1;
}

static int findSupplierById(int id)
{
    int i;
    for (i = 0; i < supplierCount; i++)
        if (suppliers[i].id == id)
            return i;
    return -1;
}

/* Case-insensitive substring match */
static int containsIgnoreCase(const char *text, const char *part)
{
    char a[NAME_LEN + 1], b[NAME_LEN + 1];
    size_t i;
    strncpy(a, text, NAME_LEN); a[NAME_LEN] = '\0';
    strncpy(b, part, NAME_LEN); b[NAME_LEN] = '\0';
    for (i = 0; a[i]; i++) a[i] = (char)tolower((unsigned char)a[i]);
    for (i = 0; b[i]; i++) b[i] = (char)tolower((unsigned char)b[i]);
    return strstr(a, b) != NULL;
}

static void printSupplier(const Supplier *s)
{
    printf("%-6d %-20s %-25s %-15s %-12s\n",
           s->id, s->name, s->email, s->phone, s->town);
}

static void printHeader(void)
{
    printf("%-6s %-20s %-25s %-15s %-12s\n",
           "ID", "Name", "Email", "Phone", "Town");
    printf("---------------------------------------------------------------------------\n");
}

/* ---------- main features ---------- */

void addSupplier(void)
{
    Supplier s;

    if (supplierCount >= MAX_SUPPLIERS) {
        printf("Supplier list is full.\n");
        return;
    }

    printf("\n--- Add Supplier ---\n");

    do {
        s.id = readInt("Supplier ID: ");
        if (findSupplierById(s.id) != -1)
            printf("  Error: that ID already exists.\n");
    } while (findSupplierById(s.id) != -1);

    readRequired("Supplier name: ", s.name, NAME_LEN);

    do {
        readRequired("Email: ", s.email, EMAIL_LEN);
        if (!isValidEmail(s.email))
            printf("  Error: invalid email (example: name@company.com).\n");
    } while (!isValidEmail(s.email));

    do {
        readRequired("Telephone: ", s.phone, PHONE_LEN);
        if (!isValidPhone(s.phone))
            printf("  Error: digits only (+ and spaces allowed), min 7 characters.\n");
    } while (!isValidPhone(s.phone));

    readRequired("Town/Location: ", s.town, TOWN_LEN);

    suppliers[supplierCount++] = s;
    printf("Supplier added successfully.\n");
}

void displaySuppliers(void)
{
    int i;
    printf("\n--- Registered Suppliers ---\n");
    if (supplierCount == 0) {
        printf("No suppliers registered yet.\n");
        return;
    }
    printHeader();
    for (i = 0; i < supplierCount; i++)
        printSupplier(&suppliers[i]);
    printf("Total suppliers: %d\n", supplierCount);
}

void searchSupplier(void)
{
    char term[NAME_LEN];
    int choice, i, found = 0;

    printf("\n--- Search Supplier ---\n");
    printf("1. By ID\n2. By name\n3. By town\n");
    choice = readInt("Choice: ");

    switch (choice) {
    case 1: {
        int id = readInt("Enter ID: ");
        int idx = findSupplierById(id);
        if (idx == -1) {
            printf("No supplier with that ID.\n");
        } else {
            printHeader();
            printSupplier(&suppliers[idx]);
        }
        break;
    }
    case 2:
    case 3:
        readRequired("Search text: ", term, NAME_LEN);
        printHeader();
        for (i = 0; i < supplierCount; i++) {
            const char *field = (choice == 2) ? suppliers[i].name : suppliers[i].town;
            if (containsIgnoreCase(field, term)) {
                printSupplier(&suppliers[i]);
                found++;
            }
        }
        if (found == 0)
            printf("No matching suppliers found.\n");
        break;
    default:
        printf("Invalid choice.\n");
    }
}

/* Compare two suppliers: are they in the same town? same name? */
void compareSuppliers(void)
{
    int idA, idB, a, b;

    printf("\n--- Compare Suppliers ---\n");
    if (supplierCount < 2) {
        printf("You need at least 2 suppliers to compare.\n");
        return;
    }
    idA = readInt("First supplier ID: ");
    idB = readInt("Second supplier ID: ");
    a = findSupplierById(idA);
    b = findSupplierById(idB);

    if (a == -1 || b == -1) {
        printf("One or both IDs were not found.\n");
        return;
    }

    printHeader();
    printSupplier(&suppliers[a]);
    printSupplier(&suppliers[b]);

    if (strcmp(suppliers[a].town, suppliers[b].town) == 0)
        printf("Result: both suppliers are in %s.\n", suppliers[a].town);
    else
        printf("Result: different towns (%s vs %s).\n",
               suppliers[a].town, suppliers[b].town);

    if (strcmp(suppliers[a].name, suppliers[b].name) == 0)
        printf("Warning: both have the same name (possible duplicate).\n");
}

int getSupplierCount(void)
{
    return supplierCount;
}

void supplierMenu(void)
{
    int choice;
    do {
        printf("\n========== SUPPLIER MANAGEMENT ==========\n");
        printf("1. Add supplier\n");
        printf("2. Display suppliers\n");
        printf("3. Search supplier\n");
        printf("4. Compare two suppliers\n");
        printf("5. Back to main menu\n");
        choice = readInt("Enter your choice: ");

        switch (choice) {
        case 1: addSupplier();      break;
        case 2: displaySuppliers(); break;
        case 3: searchSupplier();   break;
        case 4: compareSuppliers(); break;
        case 5: break;
        default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
}