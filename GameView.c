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
     

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    GameView gameView = malloc(sizeof(struct gameView));
    
    gameView->m = newMap();
    //See "The Data" page on webcms for more info.
    gameView->turns = (strlen(pastPlays)+1)/(MOVE_LENGTH + 1);
    gameView->currRound = gameView->turns/NUM_PLAYERS;

    gameView->score = GAME_START_SCORE;
    gameView->score -= gameView->currRound; //-1 loss for each of D's turn
    
    //New Stuff
    gameView->currPlayer = turns % 5;
    
    int i = 0;
    while (pastPlays[i] != '\0') {
        if (i % NUM_PLAYERS < PLAYER_DRACULA) {
            j = 0;
            while ( j <= 6) {
                switch (pastPlays[i+j]) {
                    case 'S':
                        if (pastPlays[i+j+1] == '?') {
                            health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        }
                    case 'T':
                        gameView->health[((i+4)/MOVE_LENGTH + 1 % NUM_PLAYERS)] -= LIFE_LOSS_TRAP_ENCOUNTER; 
                        break;
                    case 'D':
                        health[((i+4)/MOVE_LENGTH + 1 % 5)] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                        health[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
                        break;                         
                }
                j++;
            }
        } else {
            j = 0;
            while ( j <= 6) {
                switch (pastPlays[i+j]) {
                    case 'A':
                        if (pastPlays[i+j+1] == 'S' || pastPlays[i+j+1] == 'O') {
                            health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'B':
                        if (pastPlays[i+j+1] == 'B' || pastPlays[i+j+1] == 'S') {
                            health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'E':
                        if (pastPlays[i+j+1] == 'C') {
                            health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'I':
                        if (pastPlays[i+j+1] == 'O' || pastPlays[i+j+1] == 'R') {
                            health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;                   
                    case 'M':
                        if (pastPlays[i+j+1] == 'S') {
                            health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break; 
                    case 'N':
                        if (pastPlays[i+j+1] == 'S') {
                            health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break; 
                    case 'T':
                        if (pastPlays[i+j+1] == 'S') {
                            health[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                        } break;
                    case 'V':
                        if (i % 8 == 6) {
                            gameView->score -= SCORE_LOSS_VAMPIRE_MATURES;
                        } break;                             
                }
                j++;
            }
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
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return currentView->currLocation[player];
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
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
