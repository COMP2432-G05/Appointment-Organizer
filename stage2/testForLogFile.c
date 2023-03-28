#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 20

void readFileSeq() {
    int i = 0;

    FILE *rd = rd;
    rd = fopen("./seq.txt", "r");

    fscanf(rd, "%d", &i);
    while (!feof(rd)) {
        printf("%d ", i);
        fscanf(rd, "%d", &i);
    }
    printf("\n");
    
    fclose(rd);
}

void updateFileSeq(int algMode) {
    int i = 0;

    FILE *rd = rd;
    rd = fopen("./seq.txt", "r");
    
    int count;
    char *seqFCFS; // 0
    char *seqP; // 1
    char *seqALL; //2

    fscanf(rd, "%d", &i);
    while (!feof(rd)) {
        printf("%d ", i);
        fscanf(rd, "%d", &i);
    }
    // printf("%c\n", i);
    // for (count = 0; count < 3; count++) {
    //     if (count == 0) {
    //         printf("%c", *(&i));
    //         seqFCFS = *(&i);
    //     }
    //     if (count == 1) {
    //         printf("%c", *(&i + 1));
    //         seqP = *(&i + 1);
    //     }
    //     if (count == 2) {
    //         printf("%c", *(&i + 2));
    //         seqALL = *(&i + 2);
    //     }
    //     count++;
    // }
    // sprintf(buffer, "%d", counter);
    // fputs(buffer, fptOut);
    
    fclose(rd);

    // printf("%s %s %s\n", seqFCFS, seqP, seqALL);
}

void logRecords(char name[][NAME_SIZE], char startDate[9], char endDate[9], int countAppointmentFromUser, char algorithms[0][20], int algNum) {
    int userNum = 4;
    int i;
    char logFile[20];

    if (algNum == 1) {
        int seq = 1;
        sprintf(logFile, "./G05_%02d_%s.txt", seq, algorithms[0]);
        
    } else if (algNum == 2) {
        int seq = 1;
        sprintf(logFile, "./G05_%02d_ALL.txt", seq, algorithms[0]);
    }

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
        fputs("\n\n", fp);
        fputs("***Appointment Schedule***\n\n", fp);

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

    strcpy(startDate, "20230401");
    strcpy(endDate, "20230430");

    daysCal(startDate,endDate);
    // // start the program
    int i;
    int childNumber = argc - 3; 
    // names list
    char(*name)[NAME_SIZE] = (char(*)[NAME_SIZE])malloc(sizeof(char) * (argc - 3) * 20);
    for(i = 0; i < childNumber; i++){
        strcpy(name[i], argv[i+3]);
    }

    char algorithms[2][20] = {"FCFS"};

    int algNum = 1;

    // logRecords(name, startDate, endDate, 999, algorithms, algNum);

    updateFileSeq(0);

    return 0;
}



        //     char algorithms[2][20];
        //     int algNum;
        //     if(strcmp(token, "FCFS") == 0) {
        //         **algorithms = (char**){"FCFS"};
        //         algNum = 1;
        //     } else if (strcmp(token, "PRIORITY") == 0) {
        //         **algorithms = (char**){"PRIORITY"};
        //         algNum = 1;
        //     } else if (strcmp(token, "ALL") == 0) {
        //         **algorithms = (char**){"FCFS", "PRIORITY"};
        //         algNum = 2;
        //     }
        //   // write nothing to file (debugging)
        //     logRecords(name, startDate, endDate, 999, algorithms, algNum); // 999 is an example, need use total tasks to replace it.