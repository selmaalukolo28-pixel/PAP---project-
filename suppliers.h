#ifndef SUPPLIERS_H
#define SUPPLIERS_H

#define MAX_SUPPLIERS 100
#define NAME_LEN 50
#define EMAIL_LEN 50
#define PHONE_LEN 20
#define TOWN_LEN 30

typedef struct {
    int id;
    char name[NAME_LEN];
    char email[EMAIL_LEN];
    char phone[PHONE_LEN];
    char town[TOWN_LEN];
} Supplier;

void supplierMenu(void);
void addSupplier(void);
void displaySuppliers(void);
void searchSupplier(void);
void compareSuppliers(void);
int  getSupplierCount(void);

#endif