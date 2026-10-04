#include <stdio.h>
#include <string.h>
#include "assets.h"

void addAsset(Asset assets[], int *count) {
    printf("Enter Asset ID: ");
    scanf("%d", &assets[*count].assetID);

    printf("Enter Asset Name: ");
    scanf("%s", assets[*count].name);

    printf("Enter Asset Type: ");
    scanf("%s", assets[*count].type);

    printf("Enter Purchase Value: ");
    scanf("%f", &assets[*count].purchaseValue);

    printf("Enter Department: ");
    scanf("%s", assets[*count].department);

    printf("Enter Condition: ");
    scanf("%s", assets[*count].condition);

    (*count)++;
}

void displayAssets(Asset assets[], int count) {
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Name: %s | Type: %s | Value: %.2f | Dept: %s | Condition: %s\n",
               assets[i].assetID, assets[i].name, assets[i].type,
               assets[i].purchaseValue, assets[i].department, assets[i].condition);
    }
}

void searchAsset(Asset assets[], int count) {
    int id;
    printf("Enter Asset ID to search: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (assets[i].assetID == id) {
            printf("Found Asset: %s (%s)\n", assets[i].name, assets[i].type);
            return;
        }
    }
    printf("Asset not found.\n");
}
