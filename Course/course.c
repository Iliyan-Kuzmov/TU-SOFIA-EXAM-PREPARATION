#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Course {
    char name[51];
    char startDate[11];
    int lectures;
    float price;
};

void namalqCena(struct Course* courses, int n, int index) {
    if (index >= n || index < 0) {
        printf("index out of range\n");
        exit(1);
    }
    courses[index].price *= 0.9;
    printf("%.2f - %s - %s\n", courses[index].price, courses[index].name, courses[index].startDate);
}

int writeOffer(struct Course* courses, int n, float min , float max) {
    int found = 0;
    FILE* fp = fopen("offer.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        if (courses[i].price> min && courses[i].price < max) {
            fprintf(fp,"%s\n%s\n%d lekcii\n%.2fleva\n", courses[i].name, courses[i].startDate, courses[i].lectures, courses[i].price);
            fprintf(fp, "\n");
            found++;
        }
    }
    if (!found) {
        return 0;
    }

    fclose(fp);
    return found;
}

struct Course* deleteCourse(struct Course* courses, int* n, char* name, char* date) {
    int found = 0;
    int year, month, day;
    int currYear, currMonth, currDay;
    sscanf(date, "%d.%d.%d", &year, &month, &day);
    for (int i =0;i<*n;i++) {
        sscanf(courses[i].startDate, "%d.%d.%d", &currYear, &currMonth, &currDay);
        if ((currYear < year || (currYear == year && currMonth < month) || (currYear == year && currMonth == month && currDay < day)) && strcmp(name, courses[i].name) == 0) {
            found = 1;
            for (int j =i;j<*n;j++) {
                courses[j] = courses[j+1];
            }
            courses = (struct Course*) realloc(courses, --(*n) * sizeof(struct Course));
            if (courses == NULL) {
                printf("Memory reallocating error!\n");
                exit(1);
            }
        }
    }
    if (!found) {
        return 0;
    }
    return courses;
}

int main() {
    FILE* f = fopen("courses.bin", "wb");
    if (!f) {
        printf("Error opening file!\n");
        exit(1);
    }
    struct Course c1 = {"Introduction to computer networks", "2023.12.15", 25, 799.90};
    struct Course c2 = {"Java programming in the cloud", "2023.04.10", 30, 1249.99};
    struct Course c3 = {"C programming", "2023.08.12", 35, 1499.99};
    int m= 3;
    fwrite(&m, sizeof(int), 1, f);
    fwrite(&c1, sizeof(struct Course), 1, f);
    fwrite(&c2, sizeof(struct Course), 1, f);
    fwrite(&c3, sizeof(struct Course), 1, f);
    fclose(f);

    FILE* fp = fopen("courses.bin", "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    int n = 0;
    struct Course* courses = NULL;
    if (!fread(&n, sizeof(int), 1, fp)) {
        printf("Error reading from file!\n");
        exit(1);
    }
    courses = (struct Course*) realloc(courses,sizeof(struct Course) * n);
    if (courses == NULL) {
        printf("Error allocating memory for courses!\n");
        exit(1);
    }
    if (!fread(courses, sizeof(struct Course), n, fp)) {
        printf("Error reading from file!\n");
        exit(1);
    }
    for (int i = 0; i< n;i++) {
        printf("%s - %s - %d - %.2f\n", courses[i].name, courses[i].startDate, courses[i].lectures, courses[i].price);
    }
    namalqCena(courses,n,1);
    writeOffer(courses, n, 900, 1500);
    fclose(fp);
}