#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct IceCream {
    char id[3];
    char name[21];
    int weight;
    float pricePerKg;
};

int pricePerLetter(struct IceCream* icecreams, int n, char bukva) {
    float total = 0;
    int found= 0;
    for (int i = 0; i < n; i++) {
        if (icecreams[i].name[0] == bukva) {
            found = 1;
            total += icecreams[i].weight * icecreams[i].pricePerKg;
        }
    }
    if (!found) {
        printf("nqma takiva");
        exit(1);
    }
    return total;
}

int writeInfo(struct IceCream* icecreams, int n, float price, int weight) {
    int found = 0;
    FILE* fp = fopen("info.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        if (icecreams[i].pricePerKg < price && icecreams[i].weight > weight) {
            found = 1;
            printf("%s - %s - %d - %.2fleva\n", icecreams[i].id, icecreams[i].name, icecreams[i].weight, icecreams[i].pricePerKg);
        }
    }
    if (!found) {
        return 0;
    }
    return found;
}

void readBin(char* id) {
    FILE* fp = fopen("icecream.bin", "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    char currId[3];
    int currNameLen;
    char currName[21];
    int currWeight;
    float currPrice;
    do {
        if (!fread(currId, sizeof(char), 2, fp) || !fread(&currNameLen, sizeof(int), 1, fp) || !fread(currName, sizeof(char), currNameLen, fp) || !fread(&currWeight, sizeof(int), 1, fp) || !fread(&currPrice, sizeof(float), 1, fp)) {
            printf("Error reading from file!\n");
            exit(1);
        }
        currId[2] = '\0';
        currName[currNameLen] = '\0';
    }while (strcmp(currId, id) != 0);
    printf("Ice cream - %s\n", currName);
    printf("Price - %.2f", currPrice);
    fclose(fp);

}

int main() {
    int n;
    scanf("%d", &n);
    // do {
    //     printf("n: ");
    //     scanf("%d", &n);
    // }while (n<5 || n > 15);
    fflush(stdin);
    struct IceCream* icecreams = (struct IceCream*)malloc(n * sizeof(struct IceCream));
    if (icecreams == NULL) {
        printf("Memory allocation error!\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        printf("Enter id: ");
        fgets(icecreams[i].id, 3, stdin);
        fflush(stdin);
        printf("Enter name: ");
        fgets(icecreams[i].name, 21, stdin);
        icecreams[i].name[strlen(icecreams[i].name) - 1] = '\0';
        fflush(stdin);
        printf("Enter weight: ");
        scanf("%d", &icecreams[i].weight);
        fflush(stdin);
        printf("Enter price: ");
        scanf("%f", &icecreams[i].pricePerKg);
        fflush(stdin);
    }
    for (int i =0 ;i < n;i++) {
        printf("%s;%s;%d;%.2f\n",icecreams[i].id, icecreams[i].name, icecreams[i].weight, icecreams[i].pricePerKg);
    }

    pricePerLetter(icecreams, n, 'a');

    FILE* f = fopen("icecream.bin", "wb");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    int currNameLen;
    for (int i= 0 ; i< n ;i++) {
        currNameLen = strlen(icecreams[i].name);
        if (!fwrite(icecreams[i].id, sizeof(char), 2, f)||
            !fwrite(&currNameLen, sizeof(int), 1, f)||
            !fwrite(icecreams[i].name, sizeof(char), currNameLen, f)||
            !fwrite(&icecreams[i].weight, sizeof(int), 1, f)||
            !fwrite(&icecreams[i].pricePerKg, sizeof(float), 1, f)) {
            printf("Error writing to file!\n");
            exit(1);
        }
    }
    fclose(f);
    readBin("A2");

    free(icecreams);

}