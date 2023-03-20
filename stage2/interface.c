#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    printf("fk\n");
    // start the program
    int i;
    int childNumber = argc-3; 
    // names list
    char(*name)[20] = (char(*)[20])malloc(sizeof(char) * (argc - 3) * 20);
    for(i = 0; i < childNumber; i++){
        strcpy(name[i], argv[i+3]);
    }
    char startDate[8];
    char endDate[8];
    strcpy(startDate, argv[1]);
    strcpy(endDate, argv[2]);

    if(childNumber < 3 || childNumber > 10){
        printf("*the number of users is not in the range 3 to 10*\n"); //The system should return an error message if the number of users is not in the range 3 to 10
        exit(1);
    } 

    //pipe
    int p2c[childNumber][2];	// parent to child
    int c2p[childNumber][2];    // child to parent
    char buf[80], buf2[80];

    //pipe error 
    for(i=0;i<childNumber;i++){
        if (pipe(&p2c[i][0]) < 0) {
            printf("Pipe creation error\n");
            exit(1);
        }
        if (pipe(&c2p[i][0]) < 0) {
            printf("Pipe creation error\n");
            exit(1);
        }
    }
    
    printf("fk3\n");

    printf("parent process %d:\n", getpid());

    
	for(i = 0; i < childNumber; i++) {
        pid_t childId = fork();
        
        if(childId < 0) {
            printf("*Create child process falied*\n");
            exit(1);
        }    
		else if(childId == 0) {
			close(p2c[i][1]);
            
            while(1){
                printf("child process %d, from User %s\n",getpid(), name[i]);
                int n;
                if((n = read(p2c[i][0],buf,80)) > 0){ // read from pipe
                    buf[n] = 0;
                    
                    printf("<child> message [%s] of size %d bytes received\n",buf,n);

                    if(strcmp(buf,"endProgram") == 0) break;
                }
            }

			exit(0);
		}
        else if(childId > 0) {

        }
	}
    
    // Program start
    char input[100];
    printf("~~WELCOME TO APO~~\n");
    
    while(1){ 
        printf("Please enter appointment:\n");
        gets(input);
        int len=strlen(input);
        strcpy(buf, input);
       
        if(strcmp("endProgram", input) == 0){ // end Program
            for(i=0;i<childNumber;i++){
                write(p2c[i][1],buf,len);
            }
            break;
        } 

        judgeInputStatement(input);
    }
    
    for (i = 0; i < childNumber; i++) { 
        wait(NULL);
    }
    printf("parent jobs finished!!\n");

    exit(0);
}

// trying !!!
// struct Appointment {
//     char[][] type;
//     int startTime;
//     int date;
// }

// input module for user input
void inputModule() {
    
}

// input module for file input
void inputModuleForBatch() {

}

// call the functions by partent by user input [from program]
// identify varible --> is the first statement user input
void judgeInputStatement(char identify, char others) {
    switch(identify) {
        case 'privateTime':
            printf("privateTime");

        break;

        case 'projectMeeting':
            printf("projectMeeting");
        break;

        case 'groupStudy': 
            printf("groupStudy");
        break;
            
        case 'gathering':
            printf("gathering");
        break;

        case 'printSchd':
            printf("printSchd");
        break;

        case 'endProgram':
            printf("endProgram");
        break;
    }
}



// scheduling the appointment [from user]
void scheduling() {

}

// Scheduling kernel - use at least 2 scheduling algorithms [from program]
void schedulingKernel() {

}

// FCFS Scheduling Algorithm
void schedulingAlgorithm1(float[] burstTime, int[] arriveTime, int[] peopleId) {
    int i;
    int j;
    char queue[];
    for(i = 0; i < n; i++) {
        printf("P[%d]:", processID[i]);
    }

    read(p[0], msg, MSGSIZE);
    for (i = 0; i < sizeof(peopleId); i++) {
        if (msg == peopleId[i]) {
            scheduling();
        }
    }
    
}

// Priority Scheduling Algorithm
void schedulingAlgorithm2() {

}

// checking the appointment time range is avaible or not [from program]
int timeRangeChecking() {

}

void rescheduling() {

}



// cancel the appointment
void cancelAppointment() {

}



// create .txt file & store the records to the file
void logRecords() {

}

void logAppointmentSchedule() {

}

// log the rejected list and attach the reason why reject
void logRejectedList() {

}

// print the analyze performance report for user
void performanceReport() {

}