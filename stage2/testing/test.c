#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*-------------CONSTANTS-----------*/
#define MAX_LINE 300
#define MAX_MSG_SIZE 80
#define MAX_CARDS_IN_HAND 13
#define MAX_CARDS_IN_DECK 52

/*--------------------------HELPER FUNCTIONS--------------------------------------------------*/
/*
    This function returns the value of card. The value is computed based on the following mapping:
        SUITS: S: 400, H: 300, C: 200, D: 100
        RANKS: A: 14, K: 13, Q: 12, J: 11, T: 10, 9...2: their face value.
    Card value = suit value + rank value
    ==> I chose the value of the suits to be in 100s because the ranks exceeds 10.
*/
int getCardValue(char card[3]){
    char suit= card[0];
    char rank = card[1];

    int value = 0;
    switch (suit)
    {
        case 'S': value = 400; break;
        case 'H': value = 300; break;
        case 'C': value = 200; break;
        case 'D': value = 100; break;
        
        default:
            printf("Child, pid %d: %s has Invalid suit!\n", getpid(), card);
            return -1;
    }
    switch (rank)
    {
    case 'A': value += 14; break;
    case 'K': value+=13; break;
    case 'Q': value+=12; break;
    case 'J': value+=11; break;
    case 'T': value+=10; break;
    case '9': value += 9; break;
    case '8': value += 8; break;
    case '7': value += 7; break;
    case '6': value += 6; break;
    case '5': value += 5; break;
    case '4': value += 4; break;
    case '3': value += 3; break;
    case '2': value += 2; break;
        
    default:
        printf("Child, pid %d: Invalid suit!\n", getpid());
        return -1;
    }

    return value;
}

/*
    This function extracts the cards from parent's deck according to children's position.
    It is run in the child process. if child ID is 0, it will extract the cards in position 0, 4, 8, 12.
*/
void receiveCards(char hand[][3], char deck[][3], int playerID){
    int i, index=0;
    for( i=playerID; i<MAX_CARDS_IN_DECK; i+=4){
        strcpy(hand[index++], deck[i]);
    }
}

/*
    This function prints the cards in an array.
*/
void printCards(char cards[][3], int size){
    int i;
    for(i=0; i<size; i++){
        // printf("%s-%d ", cards[i], getCardValue(cards[i]));
        printf("%s ", cards[i]);

    }
    printf("\n");
}

/*
    This function prints elements of an array integer.
*/
void printIntArray( int arr[], int size){
    int i;
    printf("<");
    for( i=0; i<size; i++){
        printf("%d ", arr[i]);
    }
    printf(">\n");
}

/*
    This function returns the position card with lowest rank in deck/hand, irrespective of suit
*/
int findRankMin(char cards[][3], int size){
    int pos = -1; int i; int min = 1000;
    for(i=size -1; i>=0; i--){
        int value = getCardValue(cards[i]);
        int rank = value % 100;
        if(rank < min){
            min = rank;
            pos = i;
        }
    }
    return pos;
}

/*
    This function return the position of a given cards, like "SQ", in the given array of cards(deck/hand)
*/
int findCard(char cards[][3], char card[3], int size){
    int pos = -1; int i;
    for(i=0; i<size; i++){
        if( strcmp(cards[i], card) == 0){
            pos = i;
            break;
        }
    }
    return pos;
}

/*
    This function returns the position of lowest card of a given suit from a given array of cards(deck/hand)
*/
int findSuitMin( char cards[][3], int size, char suit ){
    int pos = -1; int i; int min = 1000;
    for(i=size-1; i>=0; i--){
        int value = getCardValue(cards[i]);
        if(  cards[i][0] == suit && value < min){
            min = value;
            pos = i;
        }
    }
    return pos;
}

/*
    This function returns the position of highest card of a given suit from a given array of cards(deck/hand)
*/
int findSuitMax( char cards[][3], int size, char suit ){
    int pos = -1; int i; int max = -1;
    for(i=0; i<size; i++){
        int value = getCardValue(cards[i]);
        if(  cards[i][0] == suit && value > max){
            max = value;
            pos = i;
        }
    }
    return pos;
}

/*
    This function returns the position of lowest value card in a given array of cards(deck/hand)
*/
int findMin( char cards[][3], int size){
    int pos = -1; int i; int min = 1000;
    for(i=0; i<size; i++){
        int value = getCardValue(cards[i]);
        if( value < min){
            min = value;
            pos = i;
        }
    }
    return pos;
}

/*
    This function returns the card with highest rank in a given array of cards.
    NOTE:If there are more than one highest cards in different suits for selection, 
        just select S before C before D (a simple tie-breaking rule).
*/  
int findHighest(char cards[][3], int size){
    int pos = -1;
    int maxRank = -1; int maxValue = -1;
    int i;

    for( i=0; i<size; i++){
        int value = getCardValue(cards[i]);
        int rank = value % 100;
        if( rank > maxRank){
            pos = i;
            maxRank = rank;
            maxValue = value;
        }
        else if( rank == maxRank && value > maxValue ){
            pos = i;
            maxRank = rank;
            maxValue = value;
        }
    }
    return pos;

}

/*
    This function removes a card at given position from hand/deck.
    It is achieved by shift the element before the target one position to left
    Example:
        Input:
            cards: [HA,SQ,C3,H4,D9,S3]
            pos: 2
        Output:
            cards: [HA,SQ,H4,D9,S3,S3]
    NOTE: For safety reasons, i did not decrement the array upper bound variable(limit) in the function; i.e. variable size
          because it might be being used elsewhere for other purposes. Instead, moving the limit pointer is done in the calling function.
    Example:
        remove(hand, 2, handSize);
        handSize--; //Move the limit pointer after removal of a card
*/
void removeCard(char cards[][3], int pos, int size){
    int i;
    char temp[3];
    
    for( i=pos; i< size - 1; i++){
        strcpy(cards[i], cards[i+1]);
    }
}

/*
    This function returns the position of a card to play in case of discard.
    - If there is a chance of “discard” (i.e., no card with the same suit as the lead card) during playing, SQ will be chosen for discard. 
    - If there is no SQ, then the highest H card will be discarded. 
    - If there is no H card, the highest card in the remaining hand will be discarded. 
    - If there are more than one highest cards in different suits for discard, just discard S before C before D (a simple tie-breaking rule).
*/
int findDiscard(char cards[][3], int size){
    int pos = -1;
    if( findCard(cards, "SQ", size) != -1){
        pos = findCard(cards, "SQ", size);
        return pos;
    }

    if( findSuitMax(cards, size, 'H') != -1 ){
        pos = findSuitMax(cards, size, 'H');
        return pos;
    }

    pos = findHighest(cards,size);
    return pos;
}

/*
    This function extracts the cards played by a child from message sent in pipe.
    The card is copied to output char array.
    Example:
        Input:
            - message: "play HT"
        output: "HT"
    It can also be used to extract the lead from the message sent by parent to child.
    Example:
        Input:
            - message: "played H3 SQ"
        output: "H3"
*/
void extractChildCard(char message[MAX_MSG_SIZE], char output[3]){
    char temp[MAX_MSG_SIZE];
    strcpy(temp, message);
    char *p = strtok(temp, " \n\r");
    char array[2][25];
    int index=0;
    while (p != NULL && index < 2) {
        strcpy(array[index++], p);
        // printf("####### %s\n", p);
        p = strtok(NULL, " \n\r");
    }
    strcpy(output, array[1]);
    
}

/*
    This functions arranges/sort the cards in a given array(deck/hand) in descending order based on their values.
    The calculation of card values is explained in getCardValue() function.
*/
void arrangeCards(char cards[][3], int size){
    int i,j;
    char temp[3];
    for(i=0; i<size; i++){
    for(j=0; j<(size-1-i); j++){
      if( getCardValue(cards[j]) < getCardValue(cards[j+1]) ){
        //swap cards[j] and cards[j+1]
        strcpy(temp, cards[j]);
        strcpy(cards[j], cards[j+1]);
        strcpy(cards[j+1], temp);
      }
    }
  }
}

/*
    This function calculates the score of the cards in a given array(dec/hand).
    Rules:
    - Card in "H" suit count 1 point.
    - "SQ" counts 13 points.
    - Other cards count 0 points.
    Therefore, in a deck of 52 cards, the possible points is 26 points.
*/
int countScores(char cards[][3], int size){
    int scores = 0;
    int i;
    for(i=0; i<size; i++){
        if( strcmp(cards[i], "SQ") == 0){
            scores += 13;
        }
        else if( cards[i][0] == 'H'){
            scores += 1;
        }
    }
    return scores;
}

/*
    This function return the position/ID of winning player in a round given an array of all cards played and the position of lead card.
    It is done by comparing the values of played cards excluding discards; i.e. for dicard, the value is zero.

*/
int findRoundWinner(char cards[][3], int lead, int size){
    int i;
    int winner = lead; 
    int winnerScore = getCardValue(cards[lead]); 
    char leadSuit = cards[lead][0];
    for( i=1; i<4; i++){
        int pos = (i+lead) % 4; 
        char card[3];
        strcpy(card, cards[pos]); 
        int value = getCardValue(card);
        if( card[0] == leadSuit && value> winnerScore){
            winner = pos;
            winnerScore = value;
        }
    }

    return winner;
}

/*
    This function returns the position/ID of the big winner.
    Bigger winner is the player whi have won all "H" cards and "SQ"; i.e the player will 26 points.
*/
int findBigWinner(int scores[], int size){
    int pos = -1;
    int i;
    for( i=0; i<size; i++){
        if( scores[i] == 26 ){
            pos = i;
            break;
        }
    }
    return pos;
}
/*-----------------------------------------------------------------------------------------------*/

int main(){
    int i, j, k; // common counters
    int x; //temporary variable
    int childPIDs[4]; // children process PIDs
    char input[MAX_LINE];
    char deck[52][3];
    int fd[4][2][2]; //pipes
    

/*-------------[PARENT] READ INPUT-----------*/
    int n=read(STDIN_FILENO,input,MAX_LINE);
    input[n] = 0;
    // printf("cards: %s\n", input);
/*------------------------------------*/

    
/*-------------[PARENT] DECK FORMATION----------*/
    char *card = strtok(input, " \n\r");
    int index=0;
    while (card != NULL) {
        strcpy(deck[index++], card);
        card = strtok(NULL, " \n\r");
    }
/*------------------------------------*/

/*--------------[PARENT] PIPE CREATION--------------------------*/

    for(i=0; i<4; i++){
        for(j=0;j<2;j++){
            pipe(fd[i][j]);
        }
    }
/*-----------------------------------------------------*/

/*-------------[PARENT] CHILDREN CREATION---------------------*/
    int id;
    for(id=0; id<4; id++){
        int pid= fork();
        // Error in creating child
        if(pid<0){
            printf("Error occured in forking!\n");
            exit(1);
        }
    
    /*---------------------------[CHILD] PROCESS IMPLEMENTATION--------------------------------------------*/
        else if(pid==0){
            char cReadBuffer[MAX_MSG_SIZE], cWriteBuffer[MAX_MSG_SIZE];
            char cPrintPrefix[30];

            // Creating the pre-text string for child process. E.g: Child 3, pid 20061:
            x = snprintf(cPrintPrefix, sizeof(cPrintPrefix) -1, "Child %d, pid %d:", id+1, getpid());
        /*------------------[CHILD] CLOSING UNUSED PIPE ENDS------------------*/
            for(k=0; k<4; k++){
                close(fd[k][0][1]);
                close(fd[k][1][0]);
            }
        /*---------------------------------------------------------*/
            char hand[MAX_CARDS_IN_HAND][3]; //creating hand array(holding player's cards)
        /*------------------[CHILD] READING FROM PIPE--------------*/
            int bytes, status;
            int handSize = MAX_CARDS_IN_HAND; // initialized to 13
            while ((bytes = read(fd[id][0][0], cReadBuffer, MAX_MSG_SIZE)) > 0) { // read from pipe
                cReadBuffer[bytes] = 0;
                if(strcmp(cReadBuffer, "arrange") == 0){
                    /*------------------PCHILD] INPUT VISUALIZATION------------------*/
                        // Receive the cards from parent's deck
                        receiveCards(hand, deck, id);
                        printf("Child %d, pid %d: received ", id+1, getpid());
                        printCards(hand, MAX_CARDS_IN_HAND);

                        // Arrange cards in descending order
                        arrangeCards(hand, MAX_CARDS_IN_HAND);
                        printf("Child %d, pid %d: arranged ", id+1, getpid());
                        printCards(hand, MAX_CARDS_IN_HAND);

                        // Send message to parent the you have finished arranging the cards; hence ready to play.
                        status = snprintf(cWriteBuffer, MAX_MSG_SIZE - 1, "roger");
                        write(fd[id][1][1], cWriteBuffer, MAX_MSG_SIZE);
                        
                    /*---------------------------------------------------------*/
                }
                else if( strcmp(cReadBuffer, "done") == 0 ){
                    //Breaks the reading loop once the parent announces the end of game.
                    break;
                }
                else if( strcmp(cReadBuffer, "tolead") == 0 ){ // When the child has to lead the round
                    int pos = findRankMin(hand, handSize); //Find the card with minimum card

                    //composing the message to be sent to parent. e.g: "play H3"
                    status = snprintf(cWriteBuffer, MAX_MSG_SIZE - 1, "play %s", hand[pos]);
                    printf("%s play %s\n", cPrintPrefix, hand[pos]);
                    write(fd[id][1][1], cWriteBuffer, MAX_MSG_SIZE); //send the card to parent; i.e. play the card.
                    
                    //remove the played card the hand
                    removeCard(hand, pos, handSize);
                    handSize--; //shift the right boundary of the hand.
                }
                else{
                    char leadCard[3];
                    extractChildCard(cReadBuffer, leadCard);
                    int pos = findSuitMin(hand, handSize, leadCard[0]);

                    //Checking if there is a discard situation
                    if( pos == -1){
                        pos = findDiscard(hand, handSize); //find discard card to play
                    }

                    //Composing the message to be sent to parent. e.g: "play H4"
                    status = snprintf(cWriteBuffer, MAX_MSG_SIZE - 1, "play %s", hand[pos]);
                    printf("%s play %s\n", cPrintPrefix, hand[pos]);
                    write(fd[id][1][1], cWriteBuffer, MAX_MSG_SIZE);//send the card to parent; i.e. play the card.
                    
                    //remove the played card from the hand
                    removeCard(hand, pos, handSize);
                    handSize--;//shift the right boundary of the hand.
                    
                }
            }

        /*--------------------------------------------------------*/
        /*------------------[CHILD] CLOSING UTILIZED PIPE ENDS--------------*/
            for(i=0; i<4; i++){
                close(fd[k][0][0]);
                close(fd[k][1][1]);
            }
        /*--------------------------------------------------------*/




            exit(0);
        }
    /*----------------------------------------------------------------------------------------------*/

        else{
            childPIDs[id] = pid;
        }
    }
/*------------------[PARENT] BUFFER DECLARATION-------------------*/
    char pWriteBuffer[MAX_MSG_SIZE], pReadBuffer[MAX_MSG_SIZE];
    char pPrintPrefix[30];
    //creating parent's print prefix. e.g: Parent pid 8520:
    x = snprintf(pPrintPrefix, sizeof(pPrintPrefix) -1, "Parent pid %d:", getpid());

/*----------------------------------------------------------------*/

/*------------------[PARENT]CLOSING UNUSED PIPE ENDS--------------*/
    for(i=0; i<4; i++){
        close(fd[i][0][0]);
        close(fd[i][1][1]);
    }
/*------------------------------------------------------------------*/

/*----------[PARENT] CHILD IDs PRINTING----------------------------*/
    printf("Parent pid %d, child players are ", getpid());
    for(i=0; i<4; i++){
        printf("%d ", childPIDs[i]);
    }
    printf("\n");
/*------------------------------------------------------------------*/

/*----------[PARENT] GAME INITIALIZATION ---------------------------*/
    int status, bytes;
    status = snprintf(pWriteBuffer, MAX_MSG_SIZE -1, "arrange");
    
    //Instructing all players to arrange their cards
    for(i=0; i<4; i++){
        write(fd[i][0][1], pWriteBuffer, MAX_MSG_SIZE);
    }
    //Receiving the confirmation from all player after arranging their cards.
    for(i=0; i<4; i++){
        bytes = read(fd[i][1][0], pReadBuffer, MAX_MSG_SIZE);
        pReadBuffer[bytes] = 0;
    }
/*-------------------------------------------------------------------------*/
/*----------[PARENT] GAME PLAYING-----------------------------------------*/
    int round = 0, leadPlayer = 0;
    int scores[4]={0,0,0,0};
    do{
        char playedCards[4][3];
        char msg[MAX_MSG_SIZE] = "played";

        printf("%s round %d child %d to lead\n", pPrintPrefix, round+1, leadPlayer+1);

        for(i=0; i<4; i++){
            int playerID = (leadPlayer + i) % 4;
            if( i== 0){
                strcpy(pWriteBuffer,"tolead");
                printf("%s child %d to lead\n", pPrintPrefix, playerID + 1);
            }
            else{
                strcpy(pWriteBuffer,msg);
            }
            write(fd[playerID][0][1], pWriteBuffer, MAX_MSG_SIZE);
            if((bytes = read(fd[playerID][1][0], pReadBuffer, MAX_MSG_SIZE)) > 0){
               pReadBuffer[bytes]=0;
               extractChildCard(pReadBuffer, playedCards[playerID]);
               strcat(msg, " ");
               strcat(msg, playedCards[playerID]);
               printf("%s child %d plays %s\n", pPrintPrefix, playerID + 1, playedCards[playerID]);
            }
            else{
                printf("%s Bytes: %d, Error in reading\n", pPrintPrefix, bytes);
            }
            
        }
        //find the id of the player who won the round
        int winner = findRoundWinner(playedCards, leadPlayer, 4);
        //The total score of the cards played on the round
        int roundScore = countScores(playedCards, 4);
        //Next lead is the winner of the round
        leadPlayer = winner;
        //Add the round score to the scores of winner
        scores[winner] += roundScore;

        printf("%s child %d wins the trick\n", pPrintPrefix, winner+1);
        round++;//Next round
    }
    while(round<13);
    //Get the position/ID of big winner
    int bigWinner = findBigWinner(scores, 4);
    if( bigWinner != -1){ // if there is a big winner
        i = bigWinner;
        scores[bigWinner] = 0; //score of big winner is set to 0
        for( j=1; j<4; j++){
            scores[(i+j) % 4] = 26; // scores of the rest is set to 26
        }
    }
    printf("%s Game completed\n", pPrintPrefix);
    printf("%s score ", pPrintPrefix);
    printIntArray(scores, 4); // print game scores
/*-------------------------------------------------------------------------*/

/*----------[PARENT] GAME CLOSING-----------------------------------------*/
    status = snprintf(pWriteBuffer, MAX_MSG_SIZE -1, "done");
    // Send signal to children that the game has ended
    for(i=0; i<4; i++){
        write(fd[i][0][1], pWriteBuffer, MAX_MSG_SIZE);
    }
/*------------------------------------------------------------------*/
/*------------------[PARENT]CLOSING UTILIZED PIPE ENDS--------------*/
    for(i=0; i<4; i++){
        close(fd[i][0][1]);
        close(fd[i][1][0]);
    }
/*------------------------------------------------------------------*/

/*-------------CHILDREN COLLECTION---------------------*/
    //collecting child processes
    for(i=0; i<4; i++){
        wait(NULL);
    }
/*----------------------------------------------*/



    return 0;
}


