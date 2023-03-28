#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<stdbool.h>

#define NAME_SIZE 20
#define COLS 5

struct Appointment {
    char sentence[80];
    char type[20]; //type of the event
    int startTime;
    float duration;
    char date[9];
    char host_name[NAME_SIZE];
    char all_name[10][NAME_SIZE];
    int all_num;
    bool able;
    int id;
    struct Appointment* next;
};

void insertlast(struct Appointment *newnode,struct Appointment **head){
    struct Appointment *first=*head;
    struct Appointment *second=first->next;
    while(second!=NULL){
        first=second;
        second=second->next;
    }
    first->next=newnode;
    newnode->next=NULL;
}

struct Appointment* creation(char sentence[]){
    char temp_sen[80];
    struct Appointment *temp=(struct Appointment*)malloc(sizeof(struct Appointment));
    temp->able = true;
    strcpy(temp->sentence, sentence);
    strcpy(temp_sen, sentence);
    char* token = strtok (temp_sen, " ");
    int count = 0;
    while(token != NULL)
    {
        //printf("%s\n", token);
        switch(count)
        {
            case 0: strcpy(temp->type, token); break;
            case 1: strcpy(temp->host_name, token); strcpy(temp->all_name[0], token); break;
            case 2: strcpy(temp->date, token); break;
            case 3: temp->startTime = atoi(token); break;
            case 4: temp->duration = atof(token); break;
            default: strcpy(temp->all_name[count - 4], token); break;
        }
        count++;
        token = strtok (NULL, " ");
    }
    temp->all_num = count - 4;
    temp->next = NULL;
    temp->id=-1;
    return temp;
}

int durationIndex(int time, float duration){
    int startMin = time % 100;
    int hour = (int) duration;
    int min = ((duration-hour) * 60);
    int res = hour;

    if(startMin + min > 60){
        res++;
    }
    return res;
}

int timeToIndex(int time){
    if(time >= 1800 && time < 1900){
        return 0;
    }
    if(time >= 1900 && time < 2000){
        return 1;
    }
    if(time >= 2000 && time < 2100){
        return 2;
    }
    if(time >= 2100 && time < 2200){
        return 3;
    }
    if(time >= 2200 && time < 2300){
        return 4;
    }
}

// Print 2D array
void print2Darray(int timetable[][COLS], int days){
    int i, j;

    for (i = 0; i < days; i++) {
        for (j = 0; j < COLS; j++) {
            printf("%d ", timetable[i][j]);
        }
        printf("\n");
    }
}

// Check the availability of child
int checkAv(char *days, int startTime, float duration, int timetable[][COLS]){
    int day=atoi(days)%100;
    int length = durationIndex(startTime, duration);
    int time = timeToIndex(startTime);
    int i=0;

    do{
        if(timetable[day-1][time] != -1){
            return 0;
        }
        time++;
        i++;
    }while(i<=length);

    return 1;
}

// Check the availability of child with priority
int checkAvP(int day, int startTime, float duration, int timetable[][COLS], int priority){
    int length = durationIndex(startTime, duration);
    int time = timeToIndex(startTime);
    int i=0;

    do{
        if(timetable[day-1][time] < priority){
            return 0;
        }
        time++;
        i++;
    }while(i<=length);

    return 1;
}

// Calculate the total days 20230415 - 20230430
int daysCal(char *startDate, char *endDate){
    int start = atoi(startDate);
    int end = atoi(endDate);
    int days = end-start+1;
    
    return days;
}

int main(int argc, char *argv[]) {
    // start the program
    int i;
    int childNumber = argc-3; 
    int general_id=0;
    // names list
    char(*name)[NAME_SIZE] = (char(*)[NAME_SIZE])malloc(sizeof(char) * (argc - 3) * NAME_SIZE);
    for(i = 0; i < childNumber; i++){
        strcpy(name[i], argv[i+3]);
    }

    // Dates
    char startDate[9];
    char endDate[9];
    strcpy(startDate, argv[1]);
    strcpy(endDate, argv[2]);

    // calculate days
    int days = daysCal(startDate, endDate);
    
    //The system should return an error message if the number of users is not in the range 3 to 10
    if(childNumber < 3 || childNumber > 10){
        printf("*the number of users is not in the range 3 to 10*\n"); 
        exit(1);
    } 

/*--------pipe-------*/
    int p2c[childNumber][2];	// parent to child
    int c2p[childNumber][2];    // child to parent
    char p2cBuf[80], c2pBuf[80];
    //pipe error 
    for(i = 0; i < childNumber; i++){
        if (pipe(&p2c[i][0]) < 0) {
            printf("Pipe creation error\n");
            exit(1);
        }
        if (pipe(&c2p[i][0]) < 0) {
            printf("Pipe creation error\n");
            exit(1);
        }
    }
/*-------------------*/

    printf("parent process %d:\n", getpid());

	for(i = 0; i < childNumber; i++) {
        pid_t childId = fork();
        
        if(childId < 0) {
            printf("*Create child process falied*\n");
            exit(1);
        }    
        // child process
		else if(childId == 0) {
            // close the unused pipes
			close(p2c[i][1]);
            close(c2p[i][0]);
            char p2cBuf[80];
            char c2pBuf[80];
            // timeTable
            int counter;
            int j;
            //int (*arr)[M] = malloc(sizeof(int[N][M]));
            int (*timetable)[COLS] = malloc(sizeof(int[days][COLS]));
            int (*pTimetable)[COLS] = malloc(sizeof(int[days][COLS]));
            // int (*timetable)[COLS] = (*int[COLS])malloc(sizeof(int) * COLS * days);
            // int (*pTimetable)[COLS] = (*int[COLS])malloc(sizeof(int) * COLS * days);

            //int *a = (int *)malloc(row * col * sizeof(int));

            // int** timetable = (int*)malloc(days * sizeof(int));
            // for (i = 0; i < days; i++){
            //     timetable[i] = (int*)malloc(COLS* sizeof(int));
            // }
            // int** pTimetable = (int*)malloc(days * sizeof(int));
            // for (i = 0; i < days; i++){
            //     pTimetable[i] = (int*)malloc(COLS* sizeof(int));
            // }
            for (counter = 0; counter < days; counter++) {
                for (j = 0; j < COLS; j++) {
                    timetable[counter][j] = -1;
                    pTimetable[counter][j] = -1;
                }
            }
            // printf("Timetable %d\n",i);
            // print2Darray(timetable,days);
            // printf("\n");
            // printf("pTimtable %d\n",i);
            // print2Darray(pTimetable,days);
            int n;
            while(n = read(p2c[i][0], p2cBuf, 80) > 0){
                if(strcmp(p2cBuf,"FCFS") == 0){
                    write(c2p[i][1],"done",80);
                    while(1){
                        read(p2c[i][0],p2cBuf,80);
                        // printf("%s\n",p2cBuf);
                        if(strcmp(p2cBuf,"finish")==0){
                            write(c2p[i][1], "done", 80);
                            break;
                        }
                        else{
                            struct Appointment *appointment=(struct Appointment*)malloc(sizeof(struct Appointment));
                            appointment=creation(p2cBuf);
                            printf("appointment %s\n",appointment->type);
                            write(c2p[i][1], "done", 80);
                            /* check availability */
                            //checkAv(int day, int startTime, float duration, int timetable[][COLS])
                            printf("%s %d\n",name[i],checkAv(appointment->date,appointment->startTime,appointment->duration,timetable));
                        }
                    }
                }
                else if(strcmp(p2cBuf,"endProgram") == 0) exit(0);
            }
		}
        else if(childId > 0) {
            
        }
	}
    
    // Program start
    char input[100];
    printf("~~WELCOME TO APO~~\n");
    int num_meeting=0;
    struct Appointment *head=(struct Appointment*)malloc(sizeof(struct Appointment));
    head->id=-1;
    while(1){ 
        printf("Please enter appointment:\n");
        gets(input);
        int len=strlen(input);
        strcpy(p2cBuf, input);
        char temp[80];
        strcpy(temp,input);
        char* token=strtok (temp, " ");
        if(strcmp("endProgram", input) == 0){ // end Program
            for(i=0;i<childNumber;i++){
                write(p2c[i][1],p2cBuf,len);
            }
            break;
        }
        else if(strcmp("privateTime", token) == 0){
            general_id++;
            struct Appointment *appointment = creation(input);
            appointment->general_id;
            insertlast(appointment, &head);
        }
        else if(strcmp("projectMeeting", token) == 0){
            general_id++;
            struct Appointment *appointment = creation(input);
            appointment->general_id;
            insertlast(appointment, &head);
        }
        else if(strcmp("groupStudy", token) == 0){
            general_id++;
            struct Appointment *appointment = creation(input);
            appointment->general_id;
            insertlast(appointment, &head);
        }
        else if(strcmp("gathering", token) == 0){
            general_id++;
            struct Appointment *appointment = creation(input);
            appointment->general_id;
            insertlast(appointment, &head);
        }
        else if(strcmp("printSchd", token) == 0){
            token = strtok(NULL, " ");
            struct Appointment *second=(struct Appointment*)malloc(sizeof(struct Appointment));
            /* clear function should be here */ 
            int index;
            second=head;
            if(strcmp(token, "FCFS") == 0){
                strcpy(p2cBuf, "FCFS");
                for(index = 0; index < childNumber; index++){
                    write(p2c[index][1], p2cBuf, 80);
                    read(c2p[index][0], c2pBuf, 80);
                }
                second = second->next;
                while(second != NULL){
                    printf("%s\n",second->type);
                    for(index = 0; index < second->all_num; index++){//only read once in the child in the process
                        int ID = getIDByName(second->all_name[index],name, childNumber);
                        write(p2c[ID][1], second->sentence, 80);
                        read(c2p[ID][0], c2pBuf, 80);
                        if(strcmp(c2pBuf, "n") == 0){//the child cannot schedule the meeting
                            second->able = false;
                            break;
                        }
                    }
                    if(second->able){
                        strcpy(p2cBuf, "schedule");
                    }
                    else{
                        strcpy(p2cBuf, "reject");
                    }
                    for(index=0; index < second->all_num; index++){//only read once in the child in the process
                        int ID = getIDByName(second->all_name[index],name, childNumber);
                        write(p2c[ID][1], p2cBuf, 80);
                        read(c2p[ID][0], c2pBuf, 80);
                    }
                    second = second->next;
                }
            }
            // else if(strcmp(token, "PRIORITY") == 0){
                
            // }
            strcpy(p2cBuf, "finish");//all the meeting messeages finished sending
            for(index = 0; index < childNumber; index++){
                write(p2c[index][1], p2cBuf, 80);
                read(c2p[index][0], c2pBuf, 80);
            }
        }
        else{
            printf("Wrong command!\n");
            continue;
        }
    }
    for (i = 0; i < childNumber; i++) { 
        wait(NULL);
    }
    printf("parent jobs finished!!\n");
    exit(0);
}

// int getIDByName(char target_name[], char names[][NAME_SIZE],int childNumber){
//     // int i;
//     // for (i = 0; i < childNumber; i++) {
//     //     char str1[20] = {(*name)[i] , '\0'};
//     //     char str2[20] = "";
//     //     strcpy(str2, str1);
//     //     if (strcmp(singleName, str2) == 0) {
//     //         return i;
//     //     }
//     // }
//     // return -1;
//     int index=0;
//     for(index=0;index<childNumberindex++){
//         if(strcmp(names[index],target_name)==0){
//             return index;
//         }
//     }
// }

int getIDByName(char singleName[NAME_SIZE], char names[][NAME_SIZE], int childNumber) {
    int i;
    for (i = 0; i < childNumber; i++) {
        printf("%s\n",names[i]);
        if (strcmp(singleName, names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// scheduling the appointment [from user]
void scheduling(task, timetable) {
    
}

// Scheduling kernel - use at least 2 scheduling algorithms [from program]
void schedulingKernel() {

}

// FCFS Scheduling Algorithm
// void schedulingAlgorithm1(int timetable[][]) {
//     int i;
//     Appointment *rejectList[list_length()];
    
//     while (list_next() == NULL) {
//         Appointment *task = list_next();
//         if (timeChecking(task -> startTime, task -> duration, timetable)) {
//             scheduling(*task, timetable);
//         } else {
//             *(rejectList + i) = task;
//             i++;
//             // rescheduling();
//         }
//     }
// }

// void schedulingAlgorithm1_SendingFromParent(LinkList list) {
//     int id;
//     while (list.next == NULL) {
//         if (getIDByName(list.name)) {
//             Appointment *task = list.next;
//             write(p2c[id][1],buf,len);
//         }
//     }
// }

// void schedulingAlgorithm1_ReceiverFromChild(int timetable[][]) {
//     int i;
//     Appointment *rejectList[list_length()];
//     read(p2c[i][1], buf, len);

//     if (timeChecking(task.startTime, task.duration, timetable)) {
//         scheduling(buf);
//     } else {
//         *(rejectList + i) = task;
//         i++;
//             // rescheduling();
//     }
// }


// Priority Scheduling Algorithm
void schedulingAlgorithm2() {
    
}

// checking the appointment time range is avaible or not [from program]
int timeChecking() {

}

void rescheduling() {

}



// cancel the appointment
void cancelAppointment() {

}

// create .txt file & store the records to the file
void logRecords() {
    char *logFile = "./output/log.txt";
    FILE *fp = fopen(logFile, "a+");

    
    
}

void logAppointmentSchedule() {

}

// log the rejected list and attach the reason why reject
void logRejectedList() {

}

// print the analyze performance report for user
void performanceReport() {

}
