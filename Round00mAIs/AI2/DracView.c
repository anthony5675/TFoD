// DracView.c ... DracView ADT implementation
// COMP1927 16s2 ... ... basic DracView (supplied code)
// Code by TheGroup from COMP1927 14s2 (modified by gac & jas)

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"

// Representation of Dracula's view of the game

struct dracView {
    GameView game;
};

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->game = newGameView(pastPlays, messages);

    return dracView;
}

// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    disposeGameView(toBeDeleted->game);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    return getRound(currentView->game);
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    return getScore(currentView->game);
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    return getHealth(currentView->game, player);
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    return getLocation(currentView->game, player);
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
              LocationID *start, LocationID *end)
{
    LocationID trail[TRAIL_SIZE];
    getHistory(currentView->game, player, trail);
    *start = trail[1];
    *end = trail[0];
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                int *numTraps, int *numVamps)
{
    int traps;
    int vamps;

    getMinions(currentView->game, where, &traps, &vamps);

    *numTraps = traps;
    *numVamps = vamps;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->game, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    return whereCanTheyGo(currentView,
 	                      numLocations,
	                      PLAYER_DRACULA,
                          road, FALSE, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    int i, numValidLocations, index;
    LocationID forbidden = NOWHERE;
    LocationID *validLocations;

    LocationID *locations =
		connectedLocations(currentView->game,
		                   numLocations,
		                   getLocation(currentView->game, player),
		                   player,
		                   getRound(currentView->game)+1,
		                   road, rail, sea);

    if (player == PLAYER_DRACULA) forbidden = ST_JOSEPH_AND_ST_MARYS;
    numValidLocations = 0;
    for (i = 0; i < (*numLocations); i++){
        if (locations[i] == forbidden) continue;
        numValidLocations++;
    }

    index = 0;
    validLocations = malloc(sizeof(LocationID) * numValidLocations);
    for (i = 0; i < numValidLocations; i++) {
        if (locations[i] == forbidden) continue;
        validLocations[index] = locations[i];
        index++;
    }

    free(locations);
    *numLocations = numValidLocations;
    return validLocations;
}
