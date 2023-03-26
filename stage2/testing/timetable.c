#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TIMESLOTS_PER_DAY 5

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
    return ( date % 200) -1;
}

void assign(int timeTable[][TIMESLOTS_PER_DAY], int taskID, int date, int startTime, float duration){
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

void removeAppointment(int timeTable[][5], int size, int id){
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
int main(){

    int timeTable[30][TIMESLOTS_PER_DAY];
    clearTimeTable(timeTable, 30);
    printf("=====BEFORE ASSIGNING\n");
    printTimeTable(timeTable, 30);
    assign(timeTable, 1, 20230420, 1930, 2.5);
    assign(timeTable, 2, 20230410, 1830, 1.5);
    assign(timeTable, 3, 20230429, 1830, 1);

    printf("=====AFTER ASSIGNING\n");
    printTimeTable(timeTable, 30);

    removeAppointment(timeTable, 30, 1);
    removeAppointment(timeTable, 30, 3);

    printf("=====AFTER REMOVING 1 2\n");
    printTimeTable(timeTable, 30);


    return 0;
}