// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
struct dracView {
    GameView game;
};
     

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    DracView dracView = malloc (sizeof (struct dracView));
    dracView->game = newGameView(pastPlays, dracView);
    return dracView;
}
     
     
// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    disposeGameView (toBeDelted->game);
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
    //doubleback_n gives returns dracula to the nth last move from trail.

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
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
