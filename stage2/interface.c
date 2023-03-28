#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<stdbool.h>

#define NAME_SIZE 20
#define COLS 5
#define TIMESLOTS_PER_DAY 5
#define BUF_SIZE 80

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

int isAppointmentCommand(char* command){
    int res = 0;
    if( strcmp("privateTime", command) == 0) {
        res = 1;
    }
    else if(strcmp("projectMeeting", command) == 0){
        res = 1;
    }
    else if(strcmp("groupStudy", command) == 0){
        res = 1;
    }
    else if(strcmp("gathering", command) == 0){
        res = 1;
    }

    return res;
}


/*-----------Check Availability----------------*/

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
/*---------------------------------------------*/

//--------------------------ASSIGN & REMOVE FUNCTIONS---------------------------------
int dateToIndex(int date){
    return ( date % 100) -1;
}

int startTimeToIndex(int time){
    /*
        This function returns the index number of the slot that a given time lies in timeTable.
        INPUT:
            time: a numerical representaion of time. e.g. 1800 => 18:00
        OUTPUT:
            Returns numerical index of the time slot it lies in. e.g. 1845 lies in 18:00 - 19:00 => 0(index)
    */
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

int endTimeToIndex(int time){
    /*
        This function returns the index number of the slot that a given time lies in timeTable.
        INPUT:
            time: a numerical representaion of time. e.g. 1800 => 18:00
        OUTPUT:
            Returns numerical index of the time slot it lies in. e.g. 1845 lies in 18:00 - 19:00 => 0(index)
    */
     if(time > 1800 && time <= 1900){
        return 0;
    }
    if(time > 1900 && time <= 2000){
        return 1;
    }
    if(time > 2000 && time <= 2100){
        return 2;
    }
    if(time > 2100 && time <= 2200){
        return 3;
    }
    if(time > 2200 && time <= 2300){
        return 4;
    }

}

void assignFCFS(int timeTable[][TIMESLOTS_PER_DAY], int taskID, int date, int startTime, float duration){
    /*
        This function fills the timeTable with the ID of given appointment details.
        INPUT:
            timeTable: 2D array of availability
            taskID: an integer identifier of a given task.
            date: a numeric notation of date of the appointment. e.g. 20230421 => April 21, 2023
            startTime: a numeric notation of the starting time of the appointment. e.g. 1820 => 18:20
            duration: duration of the appointment. e.g. 4.5 => 4hrs 30 min
        OUTPUT:
            Function will fill the requested timeslots with the ID of the appointment.
    */
    int dayPos = dateToIndex(date);
    int startHours = startTime / 100;
    int startMinutes = startTime % 100;
    int durationHrs = (int) duration;
    int durationMinutes = (int) (( duration - durationHrs) * 60);
    int endHours = startHours + durationHrs;
    int endMinutes = startMinutes + durationMinutes;
    if(endMinutes >= 60){
        endHours += endMinutes / 60;
        endMinutes = endMinutes % 60;
    }
    int endTime = (endHours*100) + endMinutes;

    int i;
    for(i=startTimeToIndex(startTime); i<=endTimeToIndex(endTime); i++){
        timeTable[dayPos][i] = taskID;
    }

    // printf("Time: %d, Duration: %f, End: %d\n", startTime, duration, endTime);

}

void removeAppointmentFSFC(int timeTable[][TIMESLOTS_PER_DAY], int size, int id){
    /*
        This function removes an appointment with a given from the time table.
        It is done by filling the slots which were occupied by the appointment with the -1(tomb value)
        INPUT:
            - timeTable: 2D array representaion of user's timetable.
            - size: number of rows in timetable(days).
            - id: unique identifier of the appointment to be removed.
    */
    int r, c;

    for( r=0; r<size; r++){
        for( c=0; c<TIMESLOTS_PER_DAY; c++){
            if( timeTable[r][c] == id){
                timeTable[r][c] = -1;
            }
        }
    }
}

void printTimeTable(int timeTable[][TIMESLOTS_PER_DAY], int size){
    int colSize = 10;
    printf("%*s", colSize, "Day");
    printf("%*s", colSize, "18-19");
    printf("%*s", colSize, "19-20");
    printf("%*s", colSize, "20-21");
    printf("%*s", colSize, "21-22");
    printf("%*s", colSize, "22-23");
    printf("\n");

    int r, c;
    for( r=0; r<size; r++){
        printf("%*d", colSize, r+1);
        for( c=0; c<TIMESLOTS_PER_DAY; c++ ){
            printf("%*d", colSize, timeTable[r][c]);
        }
        printf("\n");
    }

}

char* getMeeting(int cal[][COLS],int day_num){
    int i,j;
    char *result=(char*)malloc(sizeof(char)*BUF_SIZE);
    strcpy(result,"");
    for(i=0;i<day_num;i++){
        for(j=0;j<COLS;j++){
            if(j==0){
                if(cal[i][0]!=-1){
                    printf("Meeting = %d\n",cal[i][j]);
                    sprintf(result,"%s %d",result,cal[i][j]);
                }
            }
            else if(cal[i][j]!=cal[i][j-1]&&cal[i][j]!=-1){
                sprintf(result,"%s %d",result,cal[i][j]);
            }
        }
    }
    return result;
}
struct Appointment* FAPO(struct Appointment **head,int m_id){
    struct Appointment *second=*head;
    while(second!=NULL){
        if(second->id==m_id){
            return second;
        }
        second=second->next;
    }
    return NULL;
}

char* print_Appointment(struct Appointment *node){
    char *result=(char*)malloc(sizeof(char)*1000);
    strcpy(result, "");

    char type[17];
    if(strcmp(node->type, "privateTime")){
        strcpy(type, "Private Time     ");
    }
    else if(strcmp(node->type, "projectMeeting")){
        strcpy(type, "Project Meeting  ");
    }
    else if(strcmp(node->type, "groupStudy")){
        strcpy(type, "Group Study      ");
    }
    else if(strcmp(node->type, "gathering")){
        strcpy(type, "Gathering        ");
    }
    
    int start_h, start_m, end_h, end_m, dur, carry = 0;
    start_h = (node->startTime) / 100;
    start_m = (node->startTime) - start_h * 100;
    dur = (node->duration) * 60;
    end_m = start_m + dur % 60;
    if(end_m >= 60) { end_m -= 60; carry++; }

    sprintf(result, "%s%c%c%c%c-%c%c-%c%c   %02d:%02d   %02d:%02d   %s", 
        result, node->date[0], node->date[1], node->date[2], node->date[3], node->date[4], node->date[5], 
        node->date[6], node->date[7], start_h, start_m, end_h, end_m, type);

    int i;
    char name[NAME_SIZE];
    for(i = 1; i < node->all_num; i++)
    {
        strcpy(name, node->all_name[i]);
        name[0] -= ('a' - 'A');
        sprintf(result, "%s %s",result, name);
    }

    sprintf(result, "%s\n",result);

    return result;
}
//------------------------------------------------------------------------------------


int main(int argc, char *argv[]) {
    // start the program
    int i;
    int childNumber = argc-3; 
    int general_id=0;
    char pPretext[100];
    sprintf(pPretext, "Parent message: ");

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
    char p2cBuf[BUF_SIZE], c2pBuf[BUF_SIZE];
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
            // -------------------[CHILD] CLOSE UNUSED PIPES-------------------
			close(p2c[i][1]);
            close(c2p[i][0]);
            //-----------------------------------------------------------------
            char p2cBuf[BUF_SIZE];
            char c2pBuf[BUF_SIZE];
            // timeTable
            int counter;
            int j;
            char cPretext[100];
            sprintf(cPretext, "User: %s, message: ", name[i]);
            int (*timetable)[COLS] = malloc(sizeof(int[days][COLS]));
            int (*pTimetable)[COLS] = malloc(sizeof(int[days][COLS]));

            for (counter = 0; counter < days; counter++) {
                for (j = 0; j < COLS; j++) {
                    timetable[counter][j] = -1;
                    pTimetable[counter][j] = -1;
                }
            }

            int n;
            while((n = read(p2c[i][0], p2cBuf, BUF_SIZE)) > 0){
                p2cBuf[n] = 0;
                printf("%s Received message: %s\n", cPretext, p2cBuf);
                if(strcmp(p2cBuf,"FCFS") == 0){
                    write(c2p[i][1],"done",BUF_SIZE);
                }

                else if( strcmp(p2cBuf,"finish") == 0 ){
                    //reset the algorithm
                    printf("%s Finished\n", cPretext);
                    char *child_meeting=getMeeting(timetable,days);
                    strcpy(c2pBuf,child_meeting);
                    write(c2p[i][1], c2pBuf, BUF_SIZE);
                }

                else if(strcmp(p2cBuf,"endProgram") == 0) exit(0);

                else{//when the incoming command is an appointment
                    char temp[80];
                    struct Appointment *appointment=(struct Appointment*)malloc(sizeof(struct Appointment));
                    strcpy(temp,p2cBuf);
                    char *ID=strtok(temp," ");
                    char *information=strtok(NULL,"");
                    appointment=creation(information);
                    appointment->id=atoi(ID);
                    printf("%s appointment %d\n", cPretext, appointment->id);
                    int availability=checkAv(appointment->date,appointment->startTime,appointment->duration,timetable);
                    if(availability==0){
                        write(c2p[i][1], "n", BUF_SIZE);
                        printf("%s Not available\n", cPretext);
                    }
                    else{
                        write(c2p[i][1], "y", BUF_SIZE);
                        printf("%s Available\n", cPretext);
                    }
                    n = read(p2c[i][0],p2cBuf,BUF_SIZE);
                    p2cBuf[n] = 0;
                    if(strcmp(p2cBuf,"schedule")==0){
                        /*to schedule the meeting*/
                        assignFCFS(timetable, appointment->id, atoi(appointment->date), appointment->startTime, appointment->duration);
                        printf("%s Appointment No. %d is scheduled\n", cPretext, appointment->id);
                        printTimeTable(timetable, days);
                        write(c2p[i][1],"finish",BUF_SIZE);
                    }
                    else{
                        printf("%s Appointment No. %d is rejected\n", cPretext, appointment->id);
                        write(c2p[i][1],"finish", BUF_SIZE);
                    }

                }
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

        //-----------------[PARENT] INPUT READING & PARSING -----------------------
        gets(input);
        int len=strlen(input);
        strcpy(p2cBuf, input);
        char temp[BUF_SIZE];
        strcpy(temp,input);
        char* token=strtok (temp, " ");
        //------------------------------------------------------------------------

        if(strcmp("endProgram", input) == 0){ // end Program
            for(i=0;i<childNumber;i++){
                write(p2c[i][1],p2cBuf,len);
            }
            break;
        }

        else if( isAppointmentCommand(token) ) {
            printf("normal command: %s\n", token);
            general_id++;
            struct Appointment *appointment = creation(input);
            appointment->id=general_id;
            insertlast(appointment, &head);
        }

        else if(strcmp("printSchd", token) == 0){
            token = strtok(NULL, " ");
            struct Appointment *second=(struct Appointment*)malloc(sizeof(struct Appointment));
            /* clear function should be here */ 
            int index;
            second=head;
            if(strcmp(token, "FCFS") == 0){

            //---------------------[PARENT] SENDING SCHEDULING SETUP MESSAGE---------------------
                strcpy(p2cBuf, "FCFS");
                for(index = 0; index < childNumber; index++){
                    write(p2c[index][1], p2cBuf, BUF_SIZE);
                    read(c2p[index][0], c2pBuf, BUF_SIZE);
                }
            //-----------------------------------------------------------------------
            
            //------------------------[PARENT] DISPATCHING APPOINTMENTS TO USERS----------------------
                second = second->next;
                int count=0;
                while(second != NULL){
                    count++;
                    printf("%s\n",second->type);
                    char id_str[BUF_SIZE];

                    sprintf(p2cBuf,"%d %s",second->id,second->sentence);
                    printf("%s [Request] %s\n", pPretext, second->sentence);
                    for(index = 0; index < second->all_num; index++){//only read once in the child in the process
                        int ID = getIDByName(second->all_name[index],name, childNumber);
                        write(p2c[ID][1],p2cBuf, BUF_SIZE);
                        read(c2p[ID][0], c2pBuf, BUF_SIZE);
                        if(strcmp(c2pBuf, "n") == 0){//the child cannot schedule the meeting
                            second->able = false;
                            break;
                        }
                    }
                    if(second->able){
                        printf("%s All particants are available\n", pPretext);
                        strcpy(p2cBuf, "schedule");
                    }
                    else{
                        printf("%s All particants are not available\n", pPretext);
                        strcpy(p2cBuf, "reject");
                    }
                    for(index=0; index < second->all_num; index++){//only read once in the child in the process
                        int ID = getIDByName(second->all_name[index],name, childNumber);
                        write(p2c[ID][1], p2cBuf, BUF_SIZE);
                        read(c2p[ID][0], c2pBuf, BUF_SIZE);
                    }
                    second = second->next;
                }
            //----------------------------------------------------------------------------------
            
            }
            // else if(strcmp(token, "PRIORITY") == 0){
                
            // }

            //------------------[PARENT] END-SCHEDULING MESSAGE TO CHILDREN-----------------------------
            strcpy(p2cBuf, "finish");//all the meeting messeages finished sending
            for(index = 0; index < childNumber; index++){
                write(p2c[index][1], p2cBuf, BUF_SIZE);
                read(c2p[index][0], c2pBuf, BUF_SIZE);
                printf("%s\n",c2pBuf);
                char temp[BUF_SIZE];
                strcpy(temp,c2pBuf);
                char *token=strtok(temp," ");
                while(token != NULL){
                    //printf("%s\n", token);
                    int m_id=atoi(token);
                    struct Appointment *meeting=FAPO(&head,m_id);
                    // printf("Arranged_meeting %d\n",meeting->id);
                    token = strtok (NULL, " ");
                    printf("%s\n",print_Appointment(meeting));
                }
            }
            //-------------------------------------------------------------------------------
        }
        else{
            printf("Wrong command!\n");
            continue;
        }
    }
    //----------------[PARENT] CHILDREN COLLECTION ---------------------------------
    for (i = 0; i < childNumber; i++) { 
        wait(NULL);
    }
    //------------------------------------------------------------------------------
    printf("parent jobs finished!!\n");
    exit(0);
}

// get the userId by input na
int getIDByName(char singleName[NAME_SIZE], char names[][NAME_SIZE], int childNumber) {
    int i;
    for (i = 0; i < childNumber; i++) {
        if (strcmp(singleName, names[i]) == 0) {
            return i;
        }
    }
    return -1;
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
    printf("%d %d %d\n", seqFCFS, seqP, seqALL);
}


// create .txt file & store the records to the file
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

