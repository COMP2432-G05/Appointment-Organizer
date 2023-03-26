#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){
    // printf("hello world\n");
    char command[] = "projectMeeting john 20230402 1900 2.0 paul mary";
    char users[10][25];
    int counter = 0;

    char type[20];
    char initiator[20];
    char date[9];
    char time[5];
    float duration;
    char *parties[20];
    int partiesCounter = 0;

    char* token = strtok (command," ");
    while (token != NULL)
    {

        if( counter == 0){
            strcpy(type, token);
            strcpy(type, token);
        }
        else if( counter == 1 ){
            strcpy(initiator, token);
        }
        else if(counter == 2 ){
            strcpy(date, token);

        }
        else if(counter == 3 ){
            strcpy(time, token);

        }
        else if( counter == 4){
            duration = atof(token);
        }
        else{
            parties[partiesCounter++] = token;
        }
        printf ("%s\n",token);
        token = strtok (NULL, " ");
        counter++;
    }

    printf("Apointment details: \n");
    printf("=================== \n");
    printf("Type:\t\t %s\n", type);
    printf("initiator:\t %s\n", initiator);
    printf("Date:\t\t %s\n", date);
    printf("Time:\t\t %s\n", time);
    printf("Type:\t\t %.1f\n", duration);

    int i;
    printf("Parties: ");
    for(i=0; i<partiesCounter; i++){
        printf("%s ", parties[i]);
    }
    printf("\n");

    return 0;
}