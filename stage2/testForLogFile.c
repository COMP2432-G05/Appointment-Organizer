#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 20

void logRecords(char name[][NAME_SIZE], char startDate[9], char endDate[9], int countAppointmentFromUser, char algorithms[0][20], int algNum) {
    int userNum = 4;
    int i;

    char *logFile = "./log.txt";
    FILE *fp = fp;
    fopen(logFile, "w");
    fp = fopen(logFile, "a+");

    int algI;
    for (algI = 0; algI < algNum; algI++) {
        fputs("Period: ", fp);
        for (i = 0; i < 8; i++) {
            if (i == 4 || i == 6) {
                fputs("-", fp);
                char temp[2];
                temp[0] = startDate[i];
                temp[1] = '\0';
                fputs(temp, fp);
            } else {
                char temp[2];
                temp[0] = startDate[i];
                temp[1] = '\0';
                fputs(temp, fp);
            }
        }
        fputs(" to ", fp);
        for (i = 0; i < 9; i++) {
            if (i == 4 || i == 6) {
                fputs("-", fp);
                char temp[2];
                temp[0] = endDate[i];
                temp[1] = '\0';
                fputs(temp, fp);
            } else {
                char temp[2];
                temp[0] = endDate[i];
                temp[1] = '\0';
                fputs(temp, fp);
            }
        }
        fputs("\nAlgorithm used: ", fp);
        fputs(algorithms[algI], fp);
        fputs("\n", fp);
        fputs("***Appointment Schedule***\n", fp);

        for (i = 0; i < userNum; i++) {
            fputs("    ", fp);
            fputs(name[i], fp);
            fputs(", you have ", fp);
            fputs("999", fp);
            // fputs(countAppointmentFromUser, fp);
            fputs(" appointments.\n", fp);
            fputs("Date      Start      End      Type      People\n", fp);
            fputs("=========================================================================\n", fp);
            fputs("\n", fp);
            fputs("\n", fp);
            fputs("\n", fp);

            fputs("                      - End of ", fp);
            fputs(name[i], fp);
            fputs("'s Schedule -\n", fp);
            fputs("=========================================================================\n", fp);
            fputs("\n", fp);
            fputs("\n", fp);
        }
    }
    
}

// Calculate the total days 20230415 - 20230430
int daysCal(char *startDate, char *endDate){
    int start = atoi(startDate);
    int end = atoi(endDate);
    int days = end-start+1;
    
    return days;
}

int main(int argc, char *argv[]) {
    char startDate[9];
    char endDate[9];

    daysCal(startDate,endDate);
    // // start the program
    int i;
    int childNumber = argc - 3; 
    // names list
    char(*name)[NAME_SIZE] = (char(*)[NAME_SIZE])malloc(sizeof(char) * (argc - 3) * 20);
    for(i = 0; i < childNumber; i++){
        strcpy(name[i], argv[i+3]);
    }

    char algorithms[2][20] = {"FCFS", "alg"};

    size_t algNum;
    algNum = sizeof(algorithms)/sizeof(algorithms[0]);

    logRecords(name, startDate, endDate, 999, algorithms, algNum);

    return 0;
}