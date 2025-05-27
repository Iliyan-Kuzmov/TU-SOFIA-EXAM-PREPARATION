#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Medicine {
    char name[31];
    char expiryDate[8];
    long long id;
    float price;
    int quantity;
};

void namalqCena(struct Medicine* medicines, int n, char* date) {
    int found=1;
    int month, year;
    int currMonth, currYear;
    sscanf(date,"%d.%d", &month, &year);
    for (int i =0;i<n;i++) {
        sscanf(medicines[i].expiryDate,"%d.%d", &currMonth, &currYear);
        if (currYear < year || (currYear == year && currMonth < month)) {
            float oldPrice = medicines[i].price;
            medicines[i].price *=0.8;
            printf("%s - %s - %.2fleva - %.2fleva\n", medicines[i].name, medicines[i].expiryDate, oldPrice, medicines[i].price);
            found = 1;
        }
    }
    if (!found) {
        printf("nqma takiva");
        return;
    }
}

void writeBin(struct Medicine* medicines, int n, int quantity) {
    FILE* fp = fopen("offer.bin", "wb");
    if (fp == NULL) {
        printf("Error opening file");
        exit(1);
    }
    int currNameLen;
    for (int i = 0; i < n; i++) {
        if (medicines[i].quantity > quantity) {
            currNameLen = strlen(medicines[i].name);
            if (!fwrite(&currNameLen, sizeof(int), 1, fp) ||
                !fwrite(medicines[i].name, sizeof(char), currNameLen, fp) ||
                !fwrite(medicines[i].expiryDate, sizeof(char), 7, fp) ||
                !fwrite(&medicines[i].id, sizeof(long long), 1, fp) ||
                !fwrite(&medicines[i].price, sizeof(float), 1, fp) ||
                !fwrite(&medicines[i].quantity, sizeof(int), 1, fp)) {
                printf("Error writing file");
                exit(1);
            }
        }
    }
    fclose(fp);
}

void deleteMedicine(struct Medicine* medicines, int* n, long long id) {
    int found = 0;
    for (int i = 0; i < *n;i++) {
        if (medicines[i].id == id) {
            found = 1;
            for (int j = i; j < *n; j++) {
                medicines[j] = medicines[j + 1];
            }
            medicines = (struct Medicine*) realloc(medicines, --(*n) * sizeof(struct Medicine));
            break;
        }
    }
    if (!found) {
        printf("nqma takiva");
        return;
    }
}

int main() {
    FILE* f = fopen("medicines.txt", "w");
    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }
    fprintf(f, "Nurofen;10.2024;1234567890123;10.50;10\n");
    fprintf(f, "Citrovit;12.2024;1234567890124;6.50;20\n");
    fprintf(f, "Maria Med;02.2025;1234567890125;14.50;30\n");
    fclose(f);

    FILE* fp = fopen("medicines.txt", "r");
    if (fp == NULL) {
        printf("Error in opening file\n");
        exit(1);
    }
    struct Medicine* medicines = NULL;
    char line[256];
    int size = 0;
    char currName[31];
    char currDate[8];
    long long currId;
    float currPrice;
    int currQuantity;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%30[^;];%7[^;];%lld;%f;%d", currName, currDate, &currId, &currPrice, &currQuantity)!=5) {
            printf("Error in sscanf\n");
            exit(1);
        }
        size++;
        medicines = (struct Medicine*)realloc(medicines,(size) * sizeof(struct Medicine));
        if (medicines == NULL) {
            printf("Error in realloc\n");
            exit(1);
        }

        strcpy(medicines[size-1].name, currName);
        strcpy(medicines[size-1].expiryDate, currDate);
        medicines[size-1].id = currId;
        medicines[size-1].price = currPrice;
        medicines[size-1].quantity = currQuantity;
    }

    for (int i =0 ;i <size; i++) {
        printf("%s;%s;%lld;%.2f,%d\n", medicines[i].name, medicines[i].expiryDate, medicines[i].id, medicines[i].price, medicines[i].quantity);
    }
    //writeBin(medicines, size, 5);
    printf("\n");
    deleteMedicine(medicines, &size, 1234567890123);
    for (int i = 0; i < size; i++) {
        printf("%s;%s;%lld;%.2f,%d\n", medicines[i].name, medicines[i].expiryDate, medicines[i].id, medicines[i].price, medicines[i].quantity);
    }
    fclose(fp);
}