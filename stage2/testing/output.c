#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void appendFile(char filename[], char content[]){
    FILE *pFile;
    char buffer[256];

    pFile=fopen(filename, "a");
    if(pFile==NULL) {
        perror("Error opening file.");
    }
    strcpy(buffer, "testing\n");
    fprintf(pFile, "%s", buffer);
    fclose(pFile);
}

int main(){
    FILE *pFile;
    char buffer[256];

    pFile=fopen("./myfile.txt", "a");
    if(pFile==NULL) {
        perror("Error opening file.");
    }
    strcpy(buffer, "testing\n");
    fprintf(pFile, "%s", buffer);
    fclose(pFile);
    return 0;
}