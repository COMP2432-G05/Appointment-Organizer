#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_SIZE 20

bool isIn(char c_name[NAME_SIZE], struct Appointment *node){
    int i;
    for(i = 0; i < node->all_num; i++)
    {
        if(strcmp(c_name, node->all_name[i]) == 0) { return true; }
    }
    return false;
}

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
}


void logRecords(char name[][NAME_SIZE], char startDate[9], char endDate[9], char algorithms[2][20], int algNum, struct Appointment **head, int userNum) {
    int i;
    char logFile[20];
    int seq = 0;

    struct Appointment tempNode=(struct Appointment)malloc(sizeof(struct Appointment));
    tempNode = *head;
    tempNode = tempNode->next;
    int name_id;
    char *t_sentence;
    int appointmentCountbyUser[userNum];

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
            fputs(": ", fp);
            fputs("\n", fp);
            fputs("Date         Start   End     Type             People\n", fp);
            fputs("=========================================================================\n", fp);

            int appointmentCount = 0;
            while(tempNode != NULL) {
                if(tempNode->able) {
                    name_id = getIDByName(tempNode->host_name, names, userNum);
                    if (isIn(name[i], tempNode)) {
                        strcpy(t_sentence, print_Appointment(tempNode));
                        appointmentCount++;
                    }
                }
                tempNode = tempNode->next;
                fputs(t_sentence, fp);
            }
            fputs("\n", fp);
            fputs("    ", fp);
            fputs(name[i], fp);
            fputs(", you have ", fp);
            fputs(appointmentCount, fp);
            appointmentCountbyUser[i] = appointmentCount;
            fputs(" appointments.\n", fp);

            fputs("                      - End of ", fp);
            fputs(name[i], fp);
            fputs("'s Schedule -\n", fp);
            fputs("=========================================================================\n", fp);
            fputs("\n\n", fp);
        }
    }
    fputs("\n***Rejected List***\n", fp);
    int rejectedNum = 0;
    fputs("=========================================================================", fp);
    while(tempNode != NULL) {
        if(tempNode->able == false) {
            fputs(rejectedNum++, fp);
            fputs(". ", fp);
            strcpy(t_sentence, print_Appointment(tempNode));
            fputs(t_sentence, fp);
        }
        tempNode = tempNode->next;
    }
    fputs("\n\nAltogether there are", fp);
    fputs(rejectedNum, fp);
    fputs("appointments rejected.\n\n", fp);
    fputs("                      - End of Rejected List -", fp);
    fputs("=========================================================================", fp);
    
    fputs("\n\n", fp);

    int numOfReceived = 0;
    while(tempNode != NULL) {
        numOfReceived++;
        tempNode = tempNode->next;
    }

    fputs("*** Performance ***\nTotal\n", fp);

    fputs("Number of Requests Received: ", fp);
    fputs(numOfReceived, fp);
    fputs(" (", fp);
    fputs("100%%)\n", fp);
    fputs("\n", fp);

    fputs("Number of Requests Accepted: ", fp);
    int acceptedNum = numOfReceived - rejectedNum;
    float acceptRate = acceptedNum / (float)numOfReceived;
    char acceptRateStr[20];
    fputs(acceptedNum, fp);
    sprintf(acceptRateStr, " (%.1f%%)\n", acceptRate);
    fputs(acceptRateStr, fp);

    fputs("Number of Requests Rejected: ", fp);
    float rejectedRate = rejectedNum / (float)numOfReceived;
    char rejectedRateStr[20];
    fputs(rejectedNum, fp);
    sprintf(rejectedRateStr, " (%.1f%%)\n", rejectedRate);
    fputs(rejectedRateStr, fp);
    
    fputs("\n\n\n", fp);
    
    fputs("Number of Requests Accepted by Individual:\n", fp);
    for (i = 0; i < userNum; i++) {
        fputs(name[i], fp);
        fputs("   -->  ", fp);
        fputs(appointmentCountbyUser[i], fp);
        fputs("\n", fp);
    }
    fputs("\n\n\n", fp);

    fputs("Utilization of Time Slot:\n", fp);
    for (i = 0; i < userNum; i++) {
        fputs(name[i], fp);
        fputs("   -->  ", fp);
        fputs("\n", fp);
    }
    fputs("\n\n", fp);
}



int main(int argc, char *argv[]) {
    char startDate[9];
    char endDate[9];

    strcpy(startDate, "20230401");
    strcpy(endDate, "20230430");

    int i;
    int childNumber = argc - 3; 
    // names list
    char(*name)[NAME_SIZE] = (char(*)[NAME_SIZE])malloc(sizeof(char) * (argc - 3) * 20);
    for(i = 0; i < childNumber; i++){
        strcpy(name[i], argv[i+3]);
    }

    // char algorithms[2][20] = {"FCFS"};
    char algorithms[2][20] = {"FCFS"};

    int algNum = 1;
    int userNum = 4;

    logRecords(name, startDate, endDate, algorithms, algNum, head, userNum);

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