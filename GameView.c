
// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
// #include "Map.h" ... if you decide to use the Map ADT
#include "Map.h"
#define NUM_TRAPS 18
#define NUM_IMVAMP 3
#define MOVE_LENGTH 7

//Helper Functions
static void playerLocation (GameView gameView, char *pastPlays);

/*struct trap{
   int turnPlaced;
   int location;
}
*/
struct gameView {
    Round currRound; //Round consists of 5 player turns;
    PlayerID currPlayer;
    int score;
    int turns;
    //trap trapArray[NUM_TRAPS];
    //trap imVampArray[NUM_IMVAMP];
    PlayerID trail[NUM_PLAYERS][TRAIL_SIZE];
    int health[NUM_PLAYERS];
    LocationID currLocation[NUM_PLAYERS];
    LocationID location[NUM_PLAYERS][TRAIL_SIZE];
    Map m;
};
     

GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    GameView gameView = malloc(sizeof(struct gameView));
    
    gameView->m = newMap();
    //See "The Data" page on webcms for more info.
    gameView->turns = (strlen(pastPlays)+1)/(MOVE_LENGTH + 1);
    gameView->currRound = gameView->turns/NUM_PLAYERS;

    gameView->score = GAME_START_SCORE;
    gameView->score -= gameView->currRound; //-1 loss for each of D's turn
    
    playerLocation(gameView, &pastPlays);

    gameView->currPlayer = (gameView->turns) % 5;

    //Initialising health values.
    int l = 0;    
    for (l = 0; l < NUM_PLAYERS; l++) {
        if (l == PLAYER_DRACULA) {
            gameView->health[l] = GAME_START_BLOOD_POINTS ;
        } else {
            gameView->health[l] = GAME_START_HUNTER_LIFE_POINTS;
        };
    }
    
    int i, j, k = 0;
    while (pastPlays[i] != '\0') {
        //Hunters' Move
        if (i/(MOVE_LENGTH + 1) % NUM_PLAYERS < PLAYER_DRACULA) {
            j = 0;
            //If Hunter was teleported to St Joseph/Mary last round.
            if (gameView->health[i/(MOVE_LENGTH + 1) % NUM_PLAYERS] == 0) {
               gameView->health[i/(MOVE_LENGTH + 1) % NUM_PLAYERS] = GAME_START_HUNTER_LIFE_POINTS;
            }
            while ( j <= 6) {
                switch (pastPlays[i+j]) {
                    //Sea Move by Dracula
                    case 'S':
                        if (pastPlays[i+j+1] == '?') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    //Hunter runs into a trap.
                    case 'T':
                        if (j % 8 == 3) {
                            gameView->health[i/(MOVE_LENGTH + 1) % NUM_PLAYERS] -= LIFE_LOSS_TRAP_ENCOUNTER; 
                        } break;
                    //Hunter runs into Dracula.
                    case 'D':
                        if (j % 8 == 5) {
                            gameView->health[i/(MOVE_LENGTH + 1) % NUM_PLAYERS] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
                        } break;                      
                }
                //Hunter is resting at location.
                if (j % 8 == 1) {
                    if (pastPlays[i+j] == pastPlays[i+j-40] && pastPlays[i+j+1] == pastPlays[i+j+1-40]) {
                        gameView->health[i/(MOVE_LENGTH + 1) % NUM_PLAYERS] += LIFE_GAIN_REST;
                        if (gameView->health[i/(MOVE_LENGTH + 1) % NUM_PLAYERS] > 9) {
                           gameView->health[i/(MOVE_LENGTH + 1) % NUM_PLAYERS] = 9;
                        }
                    }
                }
                j++;
            }
        //Dracula's Move
        } else {
            j = 0;
            while ( j <= 6) {
                switch (pastPlays[i+j]) {
                    //Sea Moves by Dracula.
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
                    case 'T':
                        if (pastPlays[i+j+1] == 'S') {
                            gameView->health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        //Dracula teleports to Castle Dracula.
                        } else if (pastPlays[i+j+1] == 'P') {
                            gameView->health[PLAYER_DRACULA] += LIFE_GAIN_CASTLE_DRACULA;
                        } break;
                    //Vampire matures.
                    case 'V':
                        if (j % 8 == 6) {
                            gameView->score -= SCORE_LOSS_VAMPIRE_MATURES;
                        } break;                             
                }
                j++;
            }   
        }
        //Check if anyone has run out of health.
        for (k = 0; k < NUM_PLAYERS; k++) {
            if (gameView->health[k] <= 0 && k == PLAYER_DRACULA) {
                //GAME OVER?
            } else if (gameView->health[k] <= 0) {
                gameView->score -= SCORE_LOSS_HUNTER_HOSPITAL;
            }
        }
        i += 8;
    }
                                 
    return gameView;
}
     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
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
    return (PlayerID( currentView->turns % NUM_PLAYERS));
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
    
    return currentView->currLocation[player];
}

static void playerLocation (GameView gameView, char *pastPlays) {
    char *ptr = pastPlays;

    /*
    EXAMPLE:
    GMN.... SPL.... HAM....
    G is ptr
    M is ptr+1
    N is ptr+2
    this function grabs location and puts into location array
     */

    while (*ptr != '\0') {
        if (*ptr == 'G') {
            char *abbrev = malloc (3*sizeof(char));
            abbrev[0] = *(ptr+1);
            abbrev[1] = *(ptr+2);
            abbrev[2] = '\0';
            gameView->currLocation[PLAYER_LORD_GODALMING] = abbrevToID(abbrev);
            free(abbrev);
        } else if (*ptr == 'S') {
            char *abbrev = malloc (3*sizeof(char));
            abbrev[0] = *(ptr+1);
            abbrev[1] = *(ptr+2);
            abbrev[2] = '\0';
            gameView->currLocation[PLAYER_DR_SEWARD] = abbrevToID(abbrev);
            free(abbrev);
        } else if (*ptr == 'H') {
            char *abbrev = malloc (3*sizeof(char));
            abbrev[0] = *(ptr+1);
            abbrev[1] = *(ptr+2);
            abbrev[2] = '\0';
            gameView->currLocation[PLAYER_VAN_HELSING] = abbrevToID(abbrev);
            free(abbrev);
        } else if (*ptr == 'M') {
            char *abbrev = malloc (3*sizeof(char));
            abbrev[0] = *(ptr+1);
            abbrev[1] = *(ptr+2);
            abbrev[2] = '\0';
            gameView->currLocation[PLAYER_MINA_HARKER] = abbrevToID(abbrev);
            free(abbrev);
        } else if (*ptr == 'D') {
            char *abbrev = malloc (3*sizeof(char));
            abbrev[0] = *(ptr+1);
            abbrev[1] = *(ptr+2);
            abbrev[2] = '\0';

            char *checkCity = "C?";
            char *checkSea = "S?";
            char *checkTeleport = "TP";
            char *checkHide = "HI";

            if (strcmp (abbrev, checkCity) == 0) { //string comparison, 0 is equal
                gameView->currLocation[PLAYER_DRACULA] = CITY_UNKNOWN;
            } else if (strcmp (abbrev, checkSea) == 0) {
                gameView->currLocation[PLAYER_DRACULA] = SEA_UNKNOWN;
            } else if (abbrev[0] == 'D') { //double back
                if (abbrev[1] == '1') {
                    gameView->currLocation[PLAYER_DRACULA] = DOUBLE_BACK_1;
                } else if (abbrev[1] == '2') {
                    gameView->currLocation[PLAYER_DRACULA] = DOUBLE_BACK_2;
                } else if (abbrev[1] == '3') {
                    gameView->currLocation[PLAYER_DRACULA] = DOUBLE_BACK_3;
                } else if (abbrev[1] == '4') {
                    gameView->currLocation[PLAYER_DRACULA] = DOUBLE_BACK_4;
                } else if (abbrev[1] == '5') {
                    gameView->currLocation[PLAYER_DRACULA] = DOUBLE_BACK_5;
                }
            } else if (strcmp (abbrev, checkTeleport) == 0) {
                gameView->currLocation[PLAYER_DRACULA] == TELEPORT;
            } else if (strcmp (abbrev, checkHide) == 0) {
                gameView->currLocation[PLAYER_DRACULA] =HIDE;
            } else {
                gameView->currLocation[PLAYER_DRACULA] = abbrevToID(abbrev);
            }
            free(abbrev);
        }
        ptr += (MOVE_LENGTH + 1);
    }
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
   assert(0 <= player < 5);
   assert(0 <= LocationID < currentView->m->nV);
   int numlocs = 0;
   int type;
   VList curLink= currentView->m->connections[from];
   while(curLink != NULL){
      if((rail == TRUE && curLink->type == RAIL) || 
        (road == TRUE && curLink->type == ROAD) || 
        (boat == TRUE && curLink->type = BOAT && player != PLAYER_DRACULA)){
         numlocs++;
      }
   LocationID 
   VList curLink= currentView->m->connections[from];
   /*
   while(curLink != NULL){
      if((rail == TRUE && curLink->type == RAIL) || (road == TRUE && curLink->type == ROAD || (boat == TRUE && curLink->type = BOAT &&player !=    PLAYER_DRACULA)){
         numlocs++;
      }
      else if(road == TRUE && curLink->type == ROAD){
         numlocs++;
      }
      else if(boat == TRUE && curLink->type = BOAT &&player != PLAYER_DRACULA){
         numlocs++;
      }
      curLink = curLink->next;
   }
   *numLocations = numlocs;
*/

    return NULL;
}
