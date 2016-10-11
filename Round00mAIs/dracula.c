// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
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



    if (giveMeTheRound(gameState) == 0) {
        registerBestPlay("LS","Mwuhahahaha");   
    } else {
        switch (whereIs(gameState, PLAYER_DRACULA)) {
            case LISBON:
                registerBestPlay("CA", "Im not in dublin"); break;
            
            case CADIZ:
                registerBestPlay("MA", "1"); break;
            case MADRID:
                if(strcmp()){

                }
                else{

                }
                registerBestPlay("AL", ""); break;
            case ALICANTE:
                registerBestPlay("D1", "M"); break;
            case ALICANTE:
                registerBestPlay("GR", ""); break;
            case SOFIA:
                registerBestPlay("VA", ""); break;
            case VALONA:
                registerBestPlay("IO", ""); break;
            case IONIAN_SEA:
                registerBestPlay("AS", ""); break;
            case ADRIATIC_SEA:
                registerBestPlay("BI", ""); break;
            case BARI:
                registerBestPlay("NP", ""); break;
            case NAPLES:
                registerBestPlay("RO", ""); break;
            case ROME:
                registerBestPlay("FL", ""); break;
            case FLORENCE:
                registerBestPlay("VE", ""); break;
            case VENICE:
                registerBestPlay("MU", ""); break;
            case MUNICH:
                registerBestPlay("VI", ""); break;
            case VIENNA:
                registerBestPlay("BD", ""); break;
            case BUDAPEST:
                registerBestPlay("KL", ""); break;               
            case KLAUSENBURG:
                registerBestPlay("CD", ""); break; 
                
        }
    }
}
//TODO move to valid location furthest from any hunter- use breadth first search