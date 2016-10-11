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
    LocationID myTrail[TRAIL_SIZE]; // real locations
};

// Sets dracula's trail to real places (resolving TP's, HI's and Dn's)
static void setMyTrail(char *pastPlays, LocationID *myTrail)
{
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) myTrail[i] = NOWHERE;
    char *end;
    for (end = pastPlays; *end != '\0'; end++) /*skip*/ ;
    end--;
    // we jump from Drac move to Drac move
    char *p;
    for (p = &pastPlays[32]; p < end; p += 40) {
        if (*p == '\0') break;
        LocationID realLoc;
        if      (p[1] == 'T' && p[2] == 'P') realLoc = CASTLE_DRACULA;
        else if (p[1] == 'H' && p[2] == 'I') realLoc = myTrail[0];
        else if (p[1] == 'D' && p[2] == '1') realLoc = myTrail[0];
        else if (p[1] == 'D' && p[2] == '2') realLoc = myTrail[1];
        else if (p[1] == 'D' && p[2] == '3') realLoc = myTrail[2];
        else if (p[1] == 'D' && p[2] == '4') realLoc = myTrail[3];
        else if (p[1] == 'D' && p[2] == '5') realLoc = myTrail[4];
        else {
            // must be a real location
            char place[3] = { p[1], p[2], '\0' };
            realLoc = abbrevToID(place);
            if (realLoc == NOWHERE) fprintf(stderr,"Ooops: %s\n",place);
        }
        // insert location at front of trail
        for (i = TRAIL_SIZE-1; i > 0; i--) myTrail[i] = myTrail[i-1];
        myTrail[0] = realLoc;
    }
#if 0
    fprintf(stderr,"myTrail:");
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (myTrail[i] == NOWHERE) break;
        fprintf(stderr," %s",idToAbbrev(myTrail[i]));
    }
    fprintf(stderr,"\n");
#endif
    return;
}

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->game = newGameView(pastPlays, messages);
    setMyTrail(pastPlays, dracView->myTrail);
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
    if (player == PLAYER_DRACULA)
        return currentView->myTrail[0];
    else
        return getLocation(currentView->game, player);
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
              LocationID *start, LocationID *end)
{
    LocationID trail[TRAIL_SIZE];
    if (player == PLAYER_DRACULA) {
        *start = currentView->myTrail[1];
        *end   = currentView->myTrail[0];
    }
    else {
        getHistory(currentView->game, player, trail);
        *start = trail[1];
        *end = trail[0];
    }
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                int *numTraps, int *numVamps)
{
    int traps, vamps;
    getMinions(currentView->game, where, &traps, &vamps);
    *numTraps = traps;
    *numVamps = vamps;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE])
{
    if (player != PLAYER_DRACULA)
        getHistory(currentView->game, player, trail);
    else {
        int i;
        for (i = 0; i < TRAIL_SIZE; i++)
            trail[i] = currentView->myTrail[i];
    }
}

// Fills the trail array with the location ids of the last 6 moves
void giveMeTheMoves(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->game, player, trail);
}

//// Functions that query the map to find information about connectivity

// Helper function for whereCanIgo
static int onTrail(LocationID *trail, LocationID loc)
{
    int i;
    for (i = 1; i < TRAIL_SIZE; i++)
        if (trail[i] == loc) return 1;
    return 0;
}

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    LocationID *locations =
        connectedLocations(currentView->game,
                           numLocations,
                           currentView->myTrail[0],
                           PLAYER_DRACULA,
                           getRound(currentView->game),
                           road, FALSE, sea);

    // find any locations that are not allowed (e.g. hospital or in trail)
    // connectedLocations() actually removes the hospital
    // ... checking again here provides some safety and also some documentation
    int i, nValid = 0;
    int *isValid = calloc(*numLocations,sizeof(int)); // assume !valid
    for (i = 0; i < *numLocations; i++) {
        if (locations[i] != ST_JOSEPH_AND_ST_MARYS
             && !onTrail(currentView->myTrail,locations[i])) {
            isValid[i] = 1;
            nValid++;
        }
    }

    // make new locations array with only valid locations
    LocationID *validLocs = malloc(nValid*sizeof(LocationID));
    int j = 0;
    for (i = 0; i < *numLocations; i++) {
        if (isValid[i]) validLocs[j++] = locations[i];
    }

    free(locations);
    *numLocations = nValid;
    return validLocs;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    if (player == PLAYER_DRACULA)
        return whereCanIgo(currentView, numLocations, road, sea);

    LocationID *locations =
        connectedLocations(currentView->game,
                           numLocations,
                           getLocation(currentView->game, player),
                           player,
                           getRound(currentView->game),
                           road, rail, sea);
    return locations;
}

//Returns the desired message from the previous round corresponding to the specified player, into a provided array.
void giveMessage (DracView currentView, PlayerID player, PlayerMessage array) {

    getMessage(currentView->game, player, array);
}
