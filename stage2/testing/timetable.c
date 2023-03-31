#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TIMESLOTS_PER_DAY 5
#define MAX_TIME_SLOTS 155
#define MAX_CHAR_SLOTNO 4
#define MAX_MSG_SIZE 2000
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

void clearTimeTable(int timeTable[][TIMESLOTS_PER_DAY], int size){
    int r, c;
    for( r=0; r<size; r++){
        for( c=0; c<TIMESLOTS_PER_DAY; c++ ){
            timeTable[r][c] = -1;
        }
    }
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

int dateToIndex(int date){
    return ( date % 100) -1;
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

    printf("Time: %d, Duration: %f, End: %d\n", startTime, duration, endTime);

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

void assingAppointmentPriority(int timeTable[][TIMESLOTS_PER_DAY], int priorityTable[][TIMESLOTS_PER_DAY], int taskID, int date, int startTime, float duration, int priority){
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
        priorityTable[dayPos][i] = priority;
    }

    printf("Time: %d, Duration: %f, End: %d\n", startTime, duration, endTime);
}

void removeAppointmentPriority(int timeTable[][TIMESLOTS_PER_DAY], int priorityTable[][TIMESLOTS_PER_DAY], int size, int id){
    int r, c;

    for( r=0; r<size; r++){
        for( c=0; c<TIMESLOTS_PER_DAY; c++){
            if( timeTable[r][c] == id){
                timeTable[r][c] = -1;
                priorityTable[r][c] = -1;

            }
        }
    }
}

int getAvailableSlots(int timeTable[][TIMESLOTS_PER_DAY], int size, int output[MAX_TIME_SLOTS]){
    int count = 0;
    int i, j;
    for( i=0; i< size; i++){
        for(j=0; j<TIMESLOTS_PER_DAY; j++){
            if( timeTable[i][j] == -1){
                output[count++] = (i*10) + j;
            }
        }
    }
    return count;
}

int getRowFromSlotNo(int slotNo){
    return slotNo/10;
}

int getColumnFromSlotNo(int slotNo){
    return slotNo%10;
}

void fillAvailability(int map[][TIMESLOTS_PER_DAY], int availabiltArray[], int n){
    int i;
    for( i=0; i<n; i++){
        int slotNo = availabiltArray[i];
        int r = getRowFromSlotNo(slotNo);
        int c = getColumnFromSlotNo(slotNo);
        map[r][c]++;
    }
}

void getStringFromIntArray(int array[], int size, char output[]){
    int i;
    output[0] = 0;

    for( i=0; i<size; i++ ){
        if( i!=0){
            strcat(output, " ");
        }

        sprintf(output,"%s%d", output, array[i]);
    }
}

int getIntArrayFromString(char str[], int output[]){
    int count = 0;
    char temp[MAX_MSG_SIZE];
    strcpy(temp, str);
    char* token;
    token = strtok(temp, " ");
    while( token != NULL){
        output[count++] = atoi(token);
        token = strtok(NULL, " ");
    }
    return count;
}

void printIntArray(int array[], int size){
    int i;
    for(i=0; i<size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

int findRescheduleSlot(int map[][TIMESLOTS_PER_DAY], int days, int target, int n){
    int count=0;
    int isFound = 0;
    int slotNo=-1;
    int r, c;
    for(r=0; (r<days) && (isFound==0); r++){
        for(c=0; (c<TIMESLOTS_PER_DAY) && (isFound==0); c++){
            if( map[r][c] == target){
                count++;
            }
            else{
                count = 0;
            }
            if(count==n){
                isFound = 1;
                slotNo = (r*10) + (c-(n-1));
            }
        }
        count=0;
    }
    return  slotNo;
}

void fillTimeTable(int timeTable[][TIMESLOTS_PER_DAY], int days, int taskID, int slotNo, int n){
    int row = getRowFromSlotNo(slotNo);
    int col = getColumnFromSlotNo(slotNo);

    int i=0;

    for(i=0; i<n; i++){
        timeTable[row][col+i] = taskID;
    }
}

int main(){

    // int timeTable[30][TIMESLOTS_PER_DAY];
    // int priorityTable[30][TIMESLOTS_PER_DAY];
    int days = 30;
    int i;
     
    int (*timeTable)[TIMESLOTS_PER_DAY] = malloc(sizeof(int[days][TIMESLOTS_PER_DAY]));
    int (*priorityTable)[TIMESLOTS_PER_DAY] = malloc(sizeof(int[days][TIMESLOTS_PER_DAY]));
    int (*availabilityMap)[TIMESLOTS_PER_DAY] = malloc(sizeof(int[days][TIMESLOTS_PER_DAY]));
    clearTimeTable(timeTable, days);
    clearTimeTable(priorityTable, days);

    printf("=====BEFORE ASSIGNING\n");
    printf("----->TIME TABLE\n");
    printTimeTable(timeTable, days);
    printf("----->PRIORITY TABLE\n");
    printTimeTable(priorityTable, days);
    // assignFCFS(timeTable, 1, 20230420, 1930, 2.5);
    // assignFCFS(timeTable, 2, 20230410, 1830, 1.5);
    // assignFCFS(timeTable, 3, 20230429, 1830, 1);
    
    assingAppointmentPriority(timeTable, priorityTable, 1, 20230420, 1930, 2.5, 1);
    assingAppointmentPriority(timeTable, priorityTable, 2, 20230410, 1830, 1.5, 5);
    assingAppointmentPriority(timeTable, priorityTable, 3, 20230429, 1830, 1,4);

    printf("=====AFTER ASSIGNING\n");
    printf("----->TIME TABLE\n");
    int output[MAX_TIME_SLOTS];
    printTimeTable(timeTable, days);
    printf("----->INITIAL MAP\n");
    printTimeTable(availabilityMap, days);
    int n = getAvailableSlots(timeTable, days, output);
    char outputStr[MAX_MSG_SIZE];
    printIntArray(output, n);
    fillAvailability(availabilityMap, output, n);
    printf("----->INITIAL MAP\n");
    printTimeTable(availabilityMap, days);
    getStringFromIntArray(output, n, outputStr);
    printf("String: %s\n", outputStr);
    int replica[MAX_TIME_SLOTS];
    int outSize = getIntArrayFromString(outputStr, replica);
    printf("Replica array: ");
    printIntArray(replica, outSize);
    int slotsNeeded = 3;
    int taskToReschedule = 12;
    int commonSlot = findRescheduleSlot(availabilityMap,days,0,slotsNeeded);
    printf("Target slot: %d\n", commonSlot);
    fillTimeTable(timeTable, days, taskToReschedule, commonSlot,slotsNeeded);

    printf("----->AFTER RESCHEDULING\n");
    printTimeTable(timeTable, days);
    // printf("----->PRIORITY TABLE\n");
    // printTimeTable(priorityTable, days);
    // removeAppointmentFSFC(timeTable, 30, 1);
    // removeAppointmentFSFC(timeTable, 30, 3);

    // removeAppointmentPriority(timeTable, priorityTable, 30, 1);
    // removeAppointmentPriority(timeTable, priorityTable, 30, 2);

    // printf("=====AFTER REMOVING 1 2\n");
    // printf("----->TIME TABLE\n");
    // printTimeTable(timeTable, days);
    // printf("----->PRIORITY TABLE\n");
    // printTimeTable(priorityTable, days);


    return 0;
}