#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    char name[21];
    int id;
    char expiryDate[11];
    float price;
};

float avrg_by_prx(struct Product *products, int n, float price) {
    int found = 0;
    float total = 0;
    for (int i = 0; i < n; i++) {
        if (products[i].price < price) {
            found++;
            total += products[i].price;
        }
    }
    total /= found;
    if (!found) {
        return 0;
    }
    return total;
}

int write_textFile(struct Product * products, int n, char* date, float price) {
    int found = 0;
    FILE* fp = fopen("products.txt", "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    int year, month, day;
    int currYear, currMonth, currDay;
    sscanf(date, "%d.%d.%d", &year, &month, &day);
    for (int i = 0; i < n; i++) {
        sscanf(products[i].expiryDate, "%d.%d.%d", &currYear, &currMonth, &currDay);
        if ((currYear < year || (currYear == year && currMonth < month) || (currYear == year && currMonth == month && currDay < day)) && products[i].price > price) {
            found++;
            fprintf(fp, "%s - %d - %s - %.2flv\n", products[i].name, products[i].id, products[i].expiryDate, products[i].price);
        }
    }
    if (!found) {
        return 0;
    }
    fclose(fp);
    return found;
}

void printInfo(int id) {
    int found = 0;
    FILE* fp = fopen("products.bin", "rb");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    int currNameLen;
    char currName[21];
    int currId;
    char currDate[11];
    float currPrice;
    do {
        if (!fread(&currNameLen, sizeof(int), 1, fp) ||
            !fread(currName, sizeof(char), currNameLen, fp) ||
            !fread(&currId, sizeof(int), 1, fp) ||
            !fread(currDate, sizeof(char), 10, fp) ||
            !fread(&currPrice, sizeof(float), 1, fp)) {
            printf("Error reading file\n");
            exit(1);
        }
        currName[currNameLen] = '\0';
        //currDate[10] = '\0';

    }while (currId != id);

    printf("Danni za produkt: %s - izpolzvai predi %s - cena %.2flv", currName, currDate, currPrice);
    fclose(fp);


}

int main() {
    int n;
    scanf("%d", &n);
    fflush(stdin);
    struct Product* products = (struct Product* )malloc(n * sizeof(struct Product));
    if (products == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        printf("name: ");
        fgets(products[i].name, 21, stdin);
        products[i].name[strlen(products[i].name) - 1] = '\0';
        fflush(stdin);
        printf("id: ");
        scanf("%d", &products[i].id);
        fflush(stdin);
        printf("expiryDate: ");
        fgets(products[i].expiryDate, 11, stdin);
        products[i].expiryDate[strlen(products[i].expiryDate) - 1] = '\0';
        fflush(stdin);
        printf("Price: ");
        scanf("%f", &products[i].price);
        fflush(stdin);
    }

    // for (int i = 0; i< n;i++) {
    //     printf("%s - %d - %s - %.2f\n", products[i].name, products[i].id, products[i].expiryDate, products[i].price);
    // }

    FILE* p = fopen("products.bin", "wb");
    if (p == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    fwrite(&products, sizeof(struct Product), n, p);
    fclose(p);
    printInfo(111);
}