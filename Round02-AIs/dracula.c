// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "DracView.h"

void decideDraculaMove(DracView gameState)
{
    //data for use in decideMove
    /*
    int *hunterPos[5];
    int asize;
    int *validMoves = whereCanIGo(gameState, &asize, 1, 1);
    int i;
    for(i = 0; i < 5;i++){
        hunterPos[i] = whereIs(gameState,i);
    }
    */

    PlayerMessage array;
    giveMessage(gameState, PLAYER_DRACULA, array);
    
    char *forward = "";
    char *backward = "mn";
    char *teleport = "pqr";
    
    if (giveMeTheRound(gameState) == 0) {
        registerBestPlay("AO", forward);   
    } else {
        switch (whereIs(gameState, PLAYER_DRACULA)) {
            case ATLANTIC_OCEAN:
                registerBestPlay("GW", forward); break;
            case GALWAY:
                if(strcmp((char *)array, backward) == 0){
                    registerBestPlay("HI", teleport); break; //If last stop was DUBLIN, hide in GALWAY.
                } else if (strcmp((char *)array, teleport) == 0) {
                    registerBestPlay("TP", forward); break; //If last stop was GALWAY, teleport.
                } else {
                    registerBestPlay("DU", forward); break; //If last stop was ATLANTIC_OCEAN: go to DUBLIN. 
                }
            case DUBLIN:
                registerBestPlay("D2", backward); break; //DOUBLE_BACK to GALWAY.
            case CASTLE_DRACULA:
                registerBestPlay("KL", forward); break;
            case KLAUSENBURG:
                registerBestPlay("BD", forward); break;
            case BUDAPEST:
                registerBestPlay("VI", forward); break;
            case VIENNA:
                registerBestPlay("MU", forward); break;
            case MUNICH:
                registerBestPlay("MI", forward); break;
            case MILAN:
                registerBestPlay("MR", forward); break;
            case MARSEILLES:
                registerBestPlay("TO", forward); break;
            case TOULOUSE:
                registerBestPlay("SR", forward); break;
            case SARAGOSSA:
                registerBestPlay("MA", forward); break;
            case MADRID:
                registerBestPlay("LS", forward); break;
            case LISBON:
                registerBestPlay("AO", forward); break;                
        }
    }
}
//TODO move to valid location furthest from any hunter- use breadth first search
