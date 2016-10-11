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
                registerBestPlay("CA", ""); break;
            case CADIZ:
                registerBestPlay("MA", "1"); break;
            case MADRID:
                if(strcmp(message,"m") == 0){
                    registerBestPlay("GR","");
                }
                else{
                    registerBestPlay("AL", "");
                }
                break;
            case ALICANTE:
                registerBestPlay("D1", "m"); break;
            case GRANADA:
                if(strcmp(message,"HI") == 0){
                    registerBestPlay("HI","");//hides in granada
                }
                else{
                    registerBestPlay("TP", "");//tps from granada to CD
                }
            case CASTLE_DRACULA:
                registerBestPlay("KL", ""); break;
            case KLAUSENBERG:
                registerBestPlay("BD", ""); break;
            case BUDAPEST:
                registerBestPlay("VI", ""); break;
            case VIENNA:
                registerBestPlay("MU", ""); break;
            case MUNICH:
                registerBestPlay("ZU", ""); break;
            case ZURICH:
                registerBestPlay("GE", ""); break;
            case GENEVA:
                registerBestPlay("CF", ""); break;
            case CLERMONT_FERRAND:
                registerBestPlay("TO", ""); break;
            case TOULOUSE:
                registerBestPlay("SR", ""); break;
            case SARAGOSSA:
                registerBestPlay("SN", ""); break;
            case SANTANDER:
                registerBestPlay("LS", ""); break;                
        }
    }
}
//TODO move to valid location furthest from any hunter- use breadth first search