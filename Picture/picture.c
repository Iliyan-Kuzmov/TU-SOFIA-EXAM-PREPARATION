#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Picture {
    int id;
    char artistName[31];
    char pictureName[31];
    float price;
};

int sredno(struct Picture* picture, int n, float price) {
    float sredno = 0;
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (picture[i].price > price) {
            found++;
            sredno += picture[i].price;
        }
    }
    sredno /= found;
    if (!found) {
        return 0;
    }
    return sredno;
}

int writeTxt(struct Picture* picture, int n, char bukva) {
    FILE* fp = fopen("info.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (picture[i].artistName[0] == bukva) {
            found++;
            fprintf(fp,"%d - %s - %s - %.2fleva\n", picture[i].id, picture[i].artistName, picture[i].pictureName, picture[i].price);
        }
    }
    if (!found) {
        return 0;
    }
    fclose(fp);
    return found;
}

void readBin(char* artistName) {
    int found = 0;
    FILE* fp = fopen("picture.bin", "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    int currId;
    int currArtistNameLen;
    char currArtistName[31];
    int currPictureNameLen;
    char currPictureName[31];
    float currPrice;
    do {
        if (!fread(&currId, sizeof(int), 1, fp) ||
            !fread(&currArtistNameLen, sizeof(int), 1, fp) ||
            !fread(currArtistName, sizeof(char), currArtistNameLen, fp) ||
            !fread(&currPictureNameLen, sizeof(char), 1, fp) ||
            !fread(currPictureName, sizeof(char), currPictureNameLen, fp)||
            !fread(&currPrice, sizeof(float), 1, fp)) {
            printf("Error reading from file!\n");
            exit(1);
        }
        currArtistName[currArtistNameLen] = '\0';
        currPictureName[currPictureNameLen] = '\0';
    }while (strcmp(artistName, currArtistName) != 0);
    printf("Picture title: %s\nPrice: %.2f\n", currPictureName, currPrice);
    printf("=======================\n");
}

int main() {
    int n;
    scanf("%d", &n);
    // do {
    //     printf("n: ");
    //     scanf("%d", &n);
    // }while (n<=3 || n >= 30);
    fflush(stdin);
    struct Picture* picture = (struct Picture*)calloc(n, sizeof(struct Picture));
    if (picture == NULL) {
        printf("Error callocating memory!\n");
        exit(1);
    }
    for (int i =0;i<n;i++) {
        printf("id: ");
        scanf("%d", &picture[i].id);
        fflush(stdin);
        printf("artistName: ");
        fgets(picture[i].artistName, 31, stdin);
        picture[i].artistName[strlen(picture[i].artistName)-1] = '\0';
        fflush(stdin);
        printf("pictureName: ");
        fgets(picture[i].pictureName, 31, stdin);
        picture[i].pictureName[strlen(picture[i].pictureName)-1] = '\0';
        fflush(stdin);
        printf("price: ");
        scanf("%f", &picture[i].price);
        fflush(stdin);
    }
    for (int i =0; i<n;i++) {
        printf("%d - %s - %s - %.2f\n", picture[i].id, picture[i].artistName, picture[i].pictureName, picture[i].price);
    }
    writeTxt(picture, n, 'a');

    FILE* f = fopen("picture.bin", "wb");
    if (f ==NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fwrite(&picture, sizeof(struct Picture), n , f);
    // struct Picture p1 = {1111, "iliyan", "iliyan", 50.00};
    // struct Picture p2 = {2222, "georgi", "georgi", 100.00};
    // struct Picture p3 = {3333, "iliyan", "georgi", 3000.00};
    // fwrite(&p1, sizeof(struct Picture), 1, f);
    // fwrite(&p2, sizeof(struct Picture), 1, f);
    // fwrite(&p3, sizeof(struct Picture), 1, f);
    fclose(f);
    readBin("iliyan");
}