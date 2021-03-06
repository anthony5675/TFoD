
// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
// #include "Map.h" ... if you decide to use the Map ADT
#include "Map.h"
#include "set.h"
#include "queue.h"
#define MOVE_LENGTH 8

//Helper Functions
static void playerLocation (GameView gameView);
static void fillTrails (GameView gameView, char *pastPlays);

/*struct trap{
   int turnPlaced;
   int location;
}
*/
struct gameView {
    Round currRound;
    PlayerID currPlayer;
    int score;
    int turns;
    //trap trapArray[NUM_TRAPS];
    //trap imVampArray[NUM_IMVAMP];
    PlayerID trails[NUM_PLAYERS][TRAIL_SIZE];
    int health[NUM_PLAYERS];
    LocationID currLocation[NUM_PLAYERS];
    Map m;
};


GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    GameView gameView = malloc(sizeof(struct gameView));
    assert(gameView != NULL);
    
    gameView->m = newMap();
    //See "The Data" page on webcms for more info.
    gameView->turns = (strlen(pastPlays)+1)/(MOVE_LENGTH);
    gameView->currRound = gameView->turns/NUM_PLAYERS;
    gameView->score = GAME_START_SCORE;

    fillTrails(gameView, pastPlays);
    playerLocation(gameView);

    gameView->currPlayer = (gameView->turns) % 5;

    //Initialising health values.
    int l = 0;    
    for (l = 0; l < NUM_PLAYERS; l++) {
        if (l == PLAYER_DRACULA) {
            gameView->health[l] = GAME_START_BLOOD_POINTS ;
        } else {
            gameView->health[l] = GAME_START_HUNTER_LIFE_POINTS;
        }
    }
    
    int i = 0, j = 0, k = 0;
    while (pastPlays[i] != '\0') {
        //Hunters' turns.
        if ((i/MOVE_LENGTH) % NUM_PLAYERS < PLAYER_DRACULA) {
            j = 1;
            //If Hunter was teleported to St Joseph/Mary last round.
            if (gameView->health[i/(MOVE_LENGTH) % NUM_PLAYERS] == 0) {
               gameView->health[i/(MOVE_LENGTH) % NUM_PLAYERS] = GAME_START_HUNTER_LIFE_POINTS;
            }
            //Processing the turn.
            while ( j <= 6) {
                switch (pastPlays[i+j]) {
                    //Hunter runs into a trap.
                    case 'T':
                        if (j % 8 > 2) {
                            gameView->health[i/(MOVE_LENGTH) % NUM_PLAYERS] -= LIFE_LOSS_TRAP_ENCOUNTER; 
                        } break;
                    //Hunter runs into Dracula.
                    case 'D':
                        if (j % 8 > 2) {
                            gameView->health[i/(MOVE_LENGTH) % NUM_PLAYERS] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
                        } break;                      
                }
                //Checking if the Hunter is resting.
                if (j % 8 == 1 && (i+j) >= 40) {
                    if (pastPlays[i+j] == pastPlays[i+j-40] && pastPlays[i+j+1] == pastPlays[i+j+1-40]) {
                        gameView->health[i/(MOVE_LENGTH) % NUM_PLAYERS] += LIFE_GAIN_REST;
                        if (gameView->health[i/(MOVE_LENGTH) % NUM_PLAYERS] > 9) {
                           gameView->health[i/(MOVE_LENGTH) % NUM_PLAYERS] = 9;
                        }
                    }
                }
                j++;
            }
        //Dracula's Turn.
        } else {
            j = 1;
            //Processing the turn.
            while ( j <= 6) {
                switch (pastPlays[i+j]) {
                    //Dracula moves to a sea.
                    case 'A':
                        if (pastPlays[i+j+1] == 'S' || pastPlays[i+j+1] == 'O') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'B':
                        if (pastPlays[i+j+1] == 'B' || pastPlays[i+j+1] == 'S') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'E':
                        if (pastPlays[i+j+1] == 'C') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'I':
                        if (pastPlays[i+j+1] == 'O' || pastPlays[i+j+1] == 'R') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;                   
                    case 'M': case 'N':
                        if (pastPlays[i+j+1] == 'S') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'S':
                        if (pastPlays[i+j+1] == '?') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'T':
                        if (pastPlays[i+j+1] == 'S') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        //Dracula teleports to Castle Dracula.
                        } else if (pastPlays[i+j+1] == 'P') {
                            gameView->health[PLAYER_DRACULA] += LIFE_GAIN_CASTLE_DRACULA;
                        } break;
                    case 'D':
                        if (('1' <= pastPlays[i+j+1]) && (pastPlays[i+j+1] <= '5')) {
                            char *abbrev = malloc (3*sizeof(char));
                            assert(abbrev != NULL);
                            abbrev[0] = pastPlays[i+j-40];
                            abbrev[1] = pastPlays[i+j-39];
                            abbrev[2] = '\0';
                            
                            if (strcmp(abbrev, "S?") == 0) {
                               gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA; 
                            } else if (idToType(abbrevToID(abbrev)) == SEA) {
                               gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                            }

                            free(abbrev);
                        } 
                    //A vampire matures.                                          
                    case 'V':
                        if (j % 8 == 6) {
                            gameView->score -= SCORE_LOSS_VAMPIRE_MATURES;
                        } break;                             
                }
                j++;
            }   
        }
        //Checking if the players run out of health.   
        for (k = 0; k < NUM_PLAYERS; k++) {
            if (gameView->health[k] <= 0 && k == PLAYER_DRACULA) {
                //GAME OVER?
            } else if (gameView->health[k] <= 0) {
                gameView->score -= SCORE_LOSS_HUNTER_HOSPITAL;
                gameView->health[k] = GAME_START_HUNTER_LIFE_POINTS;
            }
        }
        i += 8;
        if (pastPlays[i-1] == '\0') {
            break;
        }
    }          
    return gameView;
}
     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    free( toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->currRound;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return ((PlayerID)(currentView->turns % NUM_PLAYERS));
}

// Get the current score
int getScore(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->health[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    //for debugging purposes
    //copying currLocation array
    /*int i = 0;
    LocationID testArray[NUM_PLAYERS];
    while (i < NUM_PLAYERS) {
        testArray[i] = currentView->currLocation[i];
        i++;
    }
    testArray[0] = testArray[0];
    */
    return currentView->currLocation[player];
}

static void playerLocation (GameView gameView) {
    int i = 0;

    while (i < NUM_PLAYERS) {
        gameView->currLocation[i] = gameView->trails[i][0];
        if (gameView->turns == 0) {
            gameView->currLocation[i] = UNKNOWN_LOCATION;
        }
        i++;    
    }
}

static void fillTrails (GameView gameView, char *pastPlays){
    //Filling the trail arrays.
    int m = 0, n = 0;
    int o = (gameView->turns % NUM_PLAYERS) - 1;
    int index = strlen(pastPlays) - 6;
    if (gameView->turns == 0) {
        o = PLAYER_LORD_GODALMING;
        index = 0;
    }
    if (o == -1) {
        o = PLAYER_DRACULA;
    }
    char *abbrev = malloc (3*sizeof(char));
    assert(abbrev != NULL);
    abbrev[2] = '\0';
    
    char *checkCity = "C?";
    char *checkSea = "S?";
    char *checkTeleport = "TP";
    char *checkHide = "HI";
    
    while (m < TRAIL_SIZE) {
        n = 0;
        while (n < NUM_PLAYERS) {
            if (index != 0) {
                abbrev[0] = pastPlays[index];
                abbrev[1] = pastPlays[index + 1];
                gameView->trails[o][m] = abbrevToID(abbrev); 
                
                if (o == PLAYER_DRACULA) { //Dracula's turn
                    if (strcmp (abbrev, checkCity) == 0) { //string comparison, 0 is equal
                        gameView->trails[o][m] = CITY_UNKNOWN;
                    } else if (strcmp (abbrev, checkSea) == 0) {
                        gameView->trails[o][m] = SEA_UNKNOWN;
                    } else if (abbrev[0] == 'D') { //double back
                        if (abbrev[1] == '1') {
                            gameView->trails[o][m] = DOUBLE_BACK_1;
                        } else if (abbrev[1] == '2') {
                            gameView->trails[o][m] = DOUBLE_BACK_2;
                        } else if (abbrev[1] == '3') {
                            gameView->trails[o][m] = DOUBLE_BACK_3;
                        } else if (abbrev[1] == '4') {
                            gameView->trails[o][m] = DOUBLE_BACK_4;
                        } else if (abbrev[1] == '5') {
                            gameView->trails[o][m] = DOUBLE_BACK_5;
                        }
                    } else if (strcmp (abbrev, checkTeleport) == 0) {
                        gameView->trails[o][m] = TELEPORT;
                    } else if (strcmp (abbrev, checkHide) == 0) {
                        gameView->trails[o][m] = HIDE;
                    } else {
                        gameView->trails[o][m] = abbrevToID(abbrev);
                    }
                }
            } else {
                gameView->trails[o][m] = UNKNOWN_LOCATION;
            }
            n++; o--;
            if (o < 0) {
                o = PLAYER_DRACULA;                        
            }
            index -= 8;
            if (index < 1) {
                index = 0;
            } 
        }
        m++;
    }
    
    free(abbrev);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    int i = 0;
    while (i < TRAIL_SIZE) {
        trail[i] = currentView->trails[player][i];
        i++;
    }
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{

    Set mainset = newSet();
    insertInto(mainset,from);
    Queue q = newQueue();
    int qval;
    int qdepth;
    LocationID curID = from;
    int neibsize;
    LocationID *locarray = malloc(sizeof(LocationID)*numV(currentView->m));
    assert(locarray != NULL);
    if(rail == TRUE && player != PLAYER_DRACULA){//adds rails
        enterQueue(q,curID,0);
        while(!emptyQueue(q)){
            qdepth = getDepth(q);
            qval = leaveQueue(q);
            if (qdepth < (round+player)%4){
                neibsize = typeNeighbours(currentView->m, qval, RAIL, locarray);
                int i;
                for (i = 0; i < neibsize; i++){
                    if(!isElem(mainset,locarray[i])){
                        insertInto(mainset,locarray[i]);
                        enterQueue(q,locarray[i],qdepth+1);
                    }

                }
            }
        }
    }
    if(sea){
        neibsize = typeNeighbours(currentView->m, from, BOAT, locarray);
        int i;
        for (i = 0; i < neibsize; i++){
            if(!isElem(mainset,locarray[i] && (player != PLAYER_DRACULA || locarray[i] != ST_JOSEPH_AND_ST_MARYS))){
                insertInto(mainset,locarray[i]);
            }
        } 
    }
    if(road){
        neibsize = typeNeighbours(currentView->m, from, ROAD, locarray);
        int i;
        for (i = 0; i < neibsize; i++){
            if(!isElem(mainset,locarray[i] && (player != PLAYER_DRACULA || locarray[i] != ST_JOSEPH_AND_ST_MARYS))){
                insertInto(mainset,locarray[i]);
            }
        }
    }



    *numLocations = nElems(mainset);

    LocationID *giveSet = getSet(mainset);
    disposeSet(mainset);
    disposeQueue(q);
    free(locarray);
    return giveSet;
}
