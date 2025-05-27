#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Treatment {
    int id;
    char date[11];
    char name[31];
    char diagnose[51];
};

int count_treatment(struct Treatment* treatment, int n, char* name, char* diagnose) {
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(treatment[i].name, name) == 0 && strcmp(treatment[i].diagnose, diagnose) == 0) {
            found++;
        }
    }
    if (!found) {
        return 0;
    }
    printf(found);
}

struct Treatment* add_treatment(struct Treatment* treatment, int* n) {
    struct Treatment newTreatment;
    printf("new id: ");
    scanf("%d", &newTreatment.id);
    fflush(stdin);
    printf("new date: ");
    fgets(newTreatment.date, 11, stdin);
    newTreatment.date[strlen(newTreatment.date) - 1] = '\0';
    fflush(stdin);
    printf("new name: ");
    fgets(newTreatment.name, 31, stdin);
    newTreatment.name[strlen(newTreatment.name) - 1] = '\0';
    fflush(stdin);
    printf("new diagnose: ");
    fgets(newTreatment.diagnose, 51, stdin);
    newTreatment.diagnose[strlen(newTreatment.diagnose) - 1] = '\0';
    fflush(stdin);

    treatment = (struct Treatment*) realloc(treatment, (*n +1 )* sizeof(struct Treatment));
    if (treatment == NULL) {
        printf("Error reallocating memory\n");
        exit(1);
    }
    treatment[*n] = newTreatment;
    (*n)++;
    return treatment;
}

int write_text_file(struct Treatment* treatment, int n, char* diagnose) {
    int found = 0;
    FILE* fp = fopen("illness.txt", "w");
    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }
    for (int i = 0 ; i<n;i++) {
        if (strcmp(treatment[i].diagnose, diagnose) == 0) {
            found++;
            fprintf(fp,"Bolnichen prestoi na %s\nza lechenie na %s:\n%s\n", treatment[i].name, treatment[i].diagnose, treatment[i].date);
            fprintf(fp,"\n");
        }
    }
    if (!found) {
        return 0;
    }
    fclose(fp);
    return found;
}

int main() {
    FILE* f = fopen("history.bin", "wb");
    if (!f) {
        printf("Error opening file!\n");
        exit(1);
    }
    int m = 3;
    fwrite(&m, sizeof(int), 1, f);
    struct Treatment t1 = {1, "2020.1.1", "iliyan", "hiv"};
    fwrite(&t1, sizeof(struct Treatment), 1, f);
    struct Treatment t2 = {2, "2021.1.1", "gosho", "paraliza"};
    fwrite(&t2, sizeof(struct Treatment), 1, f);
    struct Treatment t3 = {3, "2023.1.1", "ico", "hiv"};
    fwrite(&t3, sizeof(struct Treatment), 1, f);
    fclose(f);
    FILE* fp = fopen("history.bin", "rb");
    if (fp == NULL) {
        printf("Error opening bin file\n");
        exit(1);
    }
    int n = 0;
    if (!fread(&n, sizeof(int), 1, fp)) {
        printf("Error reading n from file\n");
    }
    struct Treatment* treatment = (struct Treatment*)malloc(sizeof(struct Treatment) * n);
    if (treatment == NULL) {
        printf("Error allocating treatment structure\n");
        exit(1);
    }

    if (!fread(treatment, sizeof(struct Treatment), n , fp)) {
        printf("Error reading treatment structure\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        printf("%d - %s - %s - %s\n", treatment[i].id, treatment[i].date, treatment[i].name, treatment[i].diagnose);
    }
    add_treatment(treatment, &n);
    for (int i = 0; i < n; i++) {
        printf("%d - %s - %s - %s\n", treatment[i].id, treatment[i].date, treatment[i].name, treatment[i].diagnose);
    }
    write_text_file(treatment, n , "hiv");
    fclose(fp);
}