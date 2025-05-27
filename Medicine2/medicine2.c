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

struct Medicine* prediData(struct Medicine* medicines, int n, char* date) {
    int found = 0;
    struct Medicine* expired = (struct Medicine*)malloc(n* sizeof(struct Medicine));
    if (expired == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }
    int month, year;
    int currMonth, currYear;
    sscanf(date, "%d.%d", &month, &year);
    for (int i =0 ;i<n;i++) {
        sscanf(medicines[i].expiryDate, "%d.%d", &currMonth, &currYear);
        if (currYear<year || (currYear==year && currMonth<month)) {
            expired[found++] = medicines[i];
        }
    }
    expired = realloc(expired, found * sizeof(struct Medicine));
    if (expired == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }
    if (!found) {
        return NULL;
    }
    return expired;
}

int writeText(struct Medicine* medicines, int n, float min, float max) {
    int found = 0;
    FILE* fp = fopen("offer.txt", "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    for (int i = 0; i< n;i++) {
        if (medicines[i].price > min && medicines[i].price < max) {
            found++;
            fprintf(fp, "%s\n%s\n%lld\n%.2fleva\n");
            fprintf(fp, "\n");
        }
    }
    if (!found) {
        return 0;
    }
    fclose(fp);
    return found;
}

void deleteMedicine(struct Medicine* medicines, int* n, char* name, char* date) {
    for (int i = 0;i<*n;i++) {
        if (strcmp(medicines[i].name, name)== 0 && strcmp(medicines[i].expiryDate, date)==0){
            for (int j =i;j<*n;j++) {
                medicines[j] = medicines[j+1];
            }
            medicines= realloc(medicines, --(*n) * sizeof(struct Medicine));
            if (medicines == NULL) {
                printf("Error allocating memory\n");
                exit(1);
            }
        }
    }
}


int main() {
    FILE* fp = fopen("medicines.bin", "rb");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    int count = size / sizeof(struct Medicine);
    struct Medicine* medicines = (struct Medicine*)malloc(count * sizeof(struct Medicine));
    if (medicines == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }
    if (!fread(&medicines, sizeof(struct Medicine), count, fp)) {
        printf("Error reading from file\n");
        exit(1);
    }
    fclose(fp);

}