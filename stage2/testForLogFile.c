#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 20

int readFileSeq(int algMode) {
    int i = 0;

    FILE *rd = rd;
    rd = fopen("./seq.txt", "r");
    
    int count = 0;
    int seqFCFS = 0; // 0
    int seqP = 0; // 1
    int seqALL = 0; //2

    fscanf(rd, "%d", &i);
    while (!feof(rd)) {        
        if (count == 0) {
            seqFCFS = i;
        }
        if (count == 1) {
            seqP = i;
        }
        if (count == 2) {
            seqALL = i;
        }
        fscanf(rd, "%d", &i);
        count++;
    }

    if (algMode == 0) {
        return seqFCFS;
    }
    if (algMode == 1) {
        return seqP;
    }
    if (algMode == 2) {
        return seqALL;
    }
    
    fclose(rd);

    return -1;
}

void updateFileSeq(int algMode) {
    int i = 0;

    FILE *rd = rd;
    FILE *wr = wr;
    rd = fopen("./seq.txt", "r");
    
    int count = 0;
    int seqFCFS = 0; // 0
    int seqP = 0; // 1
    int seqALL = 0; //2
    char buffer[20];

    fscanf(rd, "%d", &i);
    while (!feof(rd)) {        
        if (count == 0) {
            seqFCFS = i;
        }
        if (count == 1) {
            seqP = i;
        }
        if (count == 2) {
            seqALL = i;
        }
        fscanf(rd, "%d", &i);
        count++;
    }

    if (algMode == 0) {
        seqFCFS++;
    }
    if (algMode == 1) {
        seqP++;
    }
    if (algMode == 2) {
        seqALL++;
    }

    wr = fopen("./seq.txt", "w");
    sprintf(buffer, "%d %d %d\n", seqFCFS, seqP, seqALL);
    fputs(buffer, wr);
    fclose(rd);
    fclose(wr);
    printf("%d %d %d\n", seqFCFS, seqP, seqALL);
}

void logRecords(char name[][NAME_SIZE], char startDate[9], char endDate[9], int countAppointmentFromUser, char algorithms[0][20], int algNum) {
    int userNum = 4;
    int i;
    char logFile[20];
    int seq = 0;

    if (algNum == 1) {
        if (strcmp(algorithms[0], "FCFS") == 0) {
            seq = readFileSeq(0);
        } else {
            seq = readFileSeq(1);
        }
    } else if (algNum == 2) {
        seq = readFileSeq(2);
    }

    if (algNum == 1) {
        sprintf(logFile, "./G05_%02d_%s.txt", seq, algorithms[0]);
        if (strcmp(algorithms[0], "FCFS") == 0) {
            updateFileSeq(0);
        } else {
            updateFileSeq(1);
        }
        
    } else if (algNum == 2) {
        int seq = 1;
        sprintf(logFile, "./G05_%02d_ALL.txt", seq, algorithms[0]);
        updateFileSeq(2);
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

    // char algorithms[2][20] = {"FCFS"};
    char algorithms[2][20] = {"ALL"};

    int algNum = 1;

    logRecords(name, startDate, endDate, 999, algorithms, algNum);

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