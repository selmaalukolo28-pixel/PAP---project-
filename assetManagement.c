#include <stdio.h>
#include <string.h>
#define MAX_ASSETS 100
typedef struct {
    int assetID;
    char name[50];
    char type[30];
    float purchaseValue;
    char department[30];
    char condition[20];
} Asset;
Asset assets[MAX_ASSETS];
int assetCount = 0;
void addAsset();
void displayAssets();
void searchAsset();
int main() {
    int choice;

    do {
        printf("\n--- ASSET MANAGEMENT ---\n");
        printf("1. Add Asset\n");
        printf("2. Display Assets\n");
        printf("3. Search Asset\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addAsset(); break;
            case 2: displayAssets(); break;
            case 3: searchAsset(); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while(choice != 4);

    return 0;
}
void addAsset() {
    if(assetCount >= MAX_ASSETS) {
        printf("Asset list full!\n");
        return;
    }

    Asset a;
    printf("Enter Asset ID: ");
    scanf("%d", &a.assetID);
    getchar(); // consume newline

    printf("Enter Asset Name: ");
    fgets(a.name, sizeof(a.name), stdin);
    a.name[strcspn(a.name, "\n")] = '\0'; // remove newline

    printf("Enter Asset Type: ");
    fgets(a.type, sizeof(a.type), stdin);
    a.type[strcspn(a.type, "\n")] = '\0';

    printf("Enter Purchase Value: ");
    scanf("%f", &a.purchaseValue);
    getchar();

    printf("Enter Department: ");
    fgets(a.department, sizeof(a.department), stdin);
    a.department[strcspn(a.department, "\n")] = '\0';

    printf("Enter Condition: ");
    fgets(a.condition, sizeof(a.condition), stdin);
    a.condition[strcspn(a.condition, "\n")] = '\0';

    assets[assetCount++] = a;
    printf("Asset added successfully!\n");
}


void displayAssets() {
    if(assetCount == 0) {
        printf("No assets to display.\n");
        return;
    }

    printf("\n--- Asset List ---\n");
    for(int i = 0; i < assetCount; i++) {
        printf("ID: %d | Name: %s | Type: %s | Value: %.2f | Dept: %s | Condition: %s\n",
               assets[i].assetID, assets[i].name, assets[i].type,
               assets[i].purchaseValue, assets[i].department, assets[i].condition);
    }
}
void searchAsset() {
    int id;
    printf("Enter Asset ID to search: ");
    scanf("%d", &id);

    for(int i = 0; i < assetCount; i++) {
        if(assets[i].assetID == id) {
            printf("Asset Found: %s (%s), Value: %.2f, Dept: %s, Condition: %s\n",
                   assets[i].name, assets[i].type, assets[i].purchaseValue,
                   assets[i].department, assets[i].condition);
            return;
        }
    }
    printf("Asset not found.\n");
}
