// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#define NUM_TRAPS 2
#define TRAPS 0
#define VAMPS 1
#define MOVE_LENGTH 7
// #include "Map.h" ... if you decide to use the Map ADT

static void calcTraps (char *pastPlays, DracView gameView);
   
struct dracView {
    GameView game;
    int minions[NUM_MAP_LOCATIONS][NUM_TRAPS];
};


// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    DracView dracView = malloc (sizeof (struct dracView));
    dracView->game = newGameView(pastPlays, messages);
    calcTraps (pastPlays, dracView);
    return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    disposeGameView (toBeDeleted->game);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getRound(currentView->game);
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getScore(currentView->game);
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return getHealth(currentView->game, player);
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    int location = getLocation(currentView->game, player);

    int *trail = malloc (TRAIL_SIZE * sizeof(int)); // array of last 6 LocationID
    giveMeTheTrail (currentView, player, trail);

    //Doubleback: 
    //doubleback_n gives returns dracula n moves back.

    if (player == PLAYER_DRACULA) {
        if (location == TELEPORT) {
            location = CASTLE_DRACULA;
        } else if (location == DOUBLE_BACK_1) {
            location = trail[1];
        } else if (location == DOUBLE_BACK_2) {
            location = trail[2];
        } else if (location == DOUBLE_BACK_3) {
            location = trail[3];
        } else if (location == DOUBLE_BACK_4) {
            location = trail[4];
        } else if (location == DOUBLE_BACK_5) {
            location = trail[5];
        }
    } 
    free(trail);
    return location;
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    *end = getLocation(currentView->game, player);

    int *trail = malloc (TRAIL_SIZE * sizeof(int));
    getHistory (currentView->game, player, trail);

    *start = trail[1]; //most recent move;
    free(trail);
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    *numTraps = currentView->minions[where][TRAPS];
    *numVamps = currentView->minions[where][VAMPS];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory (currentView->game, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    LocationID from = whereIs (currentView, PLAYER_DRACULA);
    Round round = giveMeTheRound (currentView);
    return connectedLocations(currentView->game, numLocations,
                               from, PLAYER_DRACULA, round,
                               road, FALSE, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID from = whereIs (currentView, player);
    Round round = giveMeTheRound (currentView);
    if (player != PLAYER_DRACULA) {
    return connectedLocations(currentView->game, numLocations,
                               from, player, round,
                               road, rail, sea);
    } else {
        return whereCanIgo (currentView, numLocations, road, sea);
    }
}

static void calcTraps (char *pastPlays, DracView gameView) {
    char *ptr = pastPlays;
    int i = 0; //location index;
    while (i < NUM_MAP_LOCATIONS) {
        gameView->minions[i][TRAPS] = 0;
        gameView->minions[i][VAMPS] = 0;
        i++;
    }
    if (giveMeTheRound (gameView) == 0) {
        return; //no traps placed
    } 

    while (*ptr != '\0') {
        char *abbrev = malloc(3*sizeof(char));
        abbrev[0] = *(ptr+1);
        abbrev[1] = *(ptr+2);
        abbrev[2] = '\0';
        int locID = abbrevToID(abbrev);

        if (*ptr == 'D') {
            if (*(ptr+3) == 'T') {
                gameView->minions[locID][TRAPS]++;
            } else if (*(ptr+4) == 'V') {
                gameView->minions[locID][VAMPS]++;
            } else if (*(ptr+5) == 'V') { //vampire matures
                LocationID past[TRAIL_SIZE];
                giveMeTheTrail (gameView, PLAYER_DRACULA, past);
                gameView->minions[past[TRAIL_SIZE-1]][VAMPS]--; //location 6 moves ago
            } else if (*(ptr+5) == 'M') { //trap falls off trail
                LocationID past[TRAIL_SIZE];
                giveMeTheTrail (gameView, PLAYER_DRACULA, past);
                gameView->minions[past[TRAIL_SIZE-1]][TRAPS]--;
            }
        } else { //hunters encountering the traps
            int i = 0;
            while (i < 4) { //4 possible encounters after location chars
                if (*ptr+(2+i) == 'T') {
                    gameView->minions[locID][TRAPS]--; //disarm trap
                }  else if (*(ptr+(2+i)) == 'V') {
                    gameView->minions[locID][VAMPS]--;
                }
                i++;
            }
        }
        free(abbrev);
        ptr += (MOVE_LENGTH + 1);
    }
}

