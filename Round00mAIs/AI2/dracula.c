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

    PlayerMessage array;
    giveMessage(gameState, PLAYER_DRACULA, array);
    
    char *forw = "";
    char *backw = "mn";
    
    if (giveMeTheRound(gameState) == 0) {
        registerBestPlay("LS", forw);   
    } else {
        switch (whereIs(gameState, PLAYER_DRACULA)) {
            case LISBON:
                registerBestPlay("CA", forw); break;
            case CADIZ:
                registerBestPlay("MA", forw); break;
            case MADRID:
                if(strcmp((char *)array, backw) == 0){
                    registerBestPlay("GR", backw);
                } else {
                    registerBestPlay("AL", forw);
                }
                break;
            case ALICANTE:
                registerBestPlay("D1", backw); break;
            case GRANADA:
                if(strcmp((char *)array, backw) == 0){
                    registerBestPlay("HI", forw);//hides in granada
                }
                else{
                    registerBestPlay("TP", forw);//tps from granada to CD
                }
            case CASTLE_DRACULA:
                registerBestPlay("KL", forw); break;
            case KLAUSENBURG:
                registerBestPlay("BD", forw); break;
            case BUDAPEST:
                registerBestPlay("VI", forw); break;
            case VIENNA:
                registerBestPlay("MU", forw); break;
            case MUNICH:
                registerBestPlay("ZU", forw); break;
            case ZURICH:
                registerBestPlay("GE", forw); break;
            case GENEVA:
                registerBestPlay("CF", forw); break;
            case CLERMONT_FERRAND:
                registerBestPlay("TO", forw); break;
            case TOULOUSE:
                registerBestPlay("SR", forw); break;
            case SARAGOSSA:
                registerBestPlay("SN", forw); break;
            case SANTANDER:
                registerBestPlay("LS", forw); break;                
        }
    }
}
//TODO move to valid location furthest from any hunter- use breadth first search
