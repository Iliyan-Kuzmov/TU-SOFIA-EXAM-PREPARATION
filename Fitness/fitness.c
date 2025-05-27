#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Member {
    char name[56];
    char id[7];
    float price;
    int locker;
};

void addMember(struct Member* members, int* size) {
    struct Member newMember;
    printf("Enter name: ");
    fgets(newMember.name, 56, stdin);
    strtok(newMember.name, "\n");
    fflush(stdin);
    printf("Enter id: ");
    fgets(newMember.id, 7, stdin);
    newMember.id[strlen(newMember.id)-1]='\0';
    fflush(stdin);
    printf("Enter price: ");
    scanf("%f", &newMember.price);
    fflush(stdin);
    printf("Enter locker: ");
    scanf("%d", &newMember.locker);
    fflush(stdin);
    members = (struct Member*)realloc(members, (*size+1)* sizeof(struct Member));
    if (members == NULL) {
        printf("Error reallocating memory\n");
        exit(1);
    }
    members[*size] = newMember;
    (*size)++;

    FILE* fp = fopen("membersText.txt", "a");
    if (fp == NULL) {
        printf("Error opening file membersText.txt\n");
        exit(1);
    }
    fflush(stdin);
    fprintf(fp, "%d;%s;%s;%.2f;%d\n", strlen(newMember.name), newMember.name, newMember.id, newMember.price, newMember.locker);
    free(members);
    fclose(fp);
}

void podSredna(struct Member* members, int size) {
    int sredna = 0;
    for (int i = 0; i < size; i++) {
        sredna+=members[i].price;
    }
    sredna /= size;
    for (int i = 0; i < size; i++) {
        if (members[i].price < sredna) {
            printf("%s - %s - %.2f\n", members[i].name, members[i].id, members[i].price);
        }
    }
}

void readBin(char* id, int size) {
    FILE* fp = fopen("members.bin", "rb");
    if (fp == NULL) {
        printf("Error opening file members.bin\n");
        exit(1);
    }
    int currNameLen;
    char currName[56];
    char currId[7];
    float currPrice;
    int currLocker;
    int found = 0;
    do {
        if (!fread(&currNameLen, sizeof(int), 1, fp) || !fread(currName, sizeof(char), currNameLen, fp) || !fread(currId, sizeof(char), 6, fp) || !fread(&currPrice, sizeof(float), 1, fp) || !fread(&currLocker, sizeof(int), 1, fp)) {
            printf("Error reading bin file\n");
            exit(1);
        }
        currName[currNameLen] = '\0';
        currId[6] = '\0';
        if (strcmp(currId, id) == 0) {
            found = 1;
            break;
        }
    }while (1);
    if (!found) {
        printf("ne e nameren\n");
        exit(1);
    }
    printf("Bin Names: %s\nBin ID: %s\nBin Price: %.2f\nBin Locker: %d", currName, currId, currPrice, currLocker);
    fclose(fp);
}

int main() {
    struct Member* members = NULL;
    int size = 0;
    FILE* fp = fopen("membersText.txt", "w");
    if (fp == NULL) {
        printf("Error opening file membersText.txt\n");
        exit(1);
    }
    addMember(members, &size);
    addMember(members, &size);
    fclose(fp);
}