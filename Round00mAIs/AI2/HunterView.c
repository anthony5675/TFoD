// HunterView.c ... HunterView ADT implementation
// COMP1927 16s2 ... basic HunterView (supplied code)
// Code by TheGroup from COMP1927 14s2 (modified by gac & jas)

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Places.h"


// Representation of the Hunter's view of the game

struct hunterView {
    GameView game;
};


// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    HunterView hunterView = malloc(sizeof(struct hunterView));
    hunterView->game = newGameView(pastPlays, messages);
    return hunterView;
}


// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    free(toBeDeleted->game);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    return getRound(currentView->game);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    return getCurrentPlayer(currentView->game);
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    return getScore(currentView->game);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    return getHealth(currentView->game, player);
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    return getLocation(currentView->game, player);
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->game, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
    return whereCanTheyGo(currentView,
	                      numLocations,
	                      getCurrentPlayer(currentView->game),
	                      road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
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
    for(i = 0; i < (*numLocations); i++){
        if (locations[i] == forbidden) continue;
        numValidLocations++;
    }

    index = 0;
    validLocations = malloc(sizeof(LocationID) * numValidLocations);
    for(i = 0; i < numValidLocations; i++){
        if (locations[i] == forbidden) continue;
        validLocations[index] = locations[i];
        index++;
    }

    free(locations);
    *numLocations = numValidLocations;
    return validLocations;
}
