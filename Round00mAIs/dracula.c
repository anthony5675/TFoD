// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"

void decideDraculaMove(DracView gameState)
{
    if (giveMeTheRound(gameState) == 0) {
        registerBestPlay("CD","Mwuhahahaha");
    } else {
        switch (whereIs(gameState, PLAYER_DRACULA)) {
            case CASTLE_DRACULA:
                registerBestPlay("GA", ""); break;
            case GALATZ:
                registerBestPlay("BC", ""); break;
            case BUCHAREST:
                registerBestPlay("SO", ""); break;
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
