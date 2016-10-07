// hunter.c
// Implementation of your "Fury of Dracula" hunter AI
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Game.h"
#include "HunterView.h"

void decideHunterMove(HunterView gameState)
{
    PlayerMessage array;
    if (whoAmI(gameState) == PLAYER_LORD_GODALMING) {
        giveMessage(gameState, PLAYER_LORD_GODALMING, array);
        if (whereIs(gameState, PLAYER_LORD_GODALMING) == ST_JOSEPH_AND_ST_MARYS){
            registerBestPlay("SZ", "");
        } else if (whereIs(gameState, PLAYER_LORD_GODALMING) == SZEGED) {
            registerBestPlay("KL","");
        } else if(strcmp((char *)array, "ab") == 0 && whereIs(gameState, PLAYER_DRACULA) != CASTLE_DRACULA) {
            registerBestPlay("GA", "");
        } else if(whereIs(gameState, PLAYER_DRACULA) == CASTLE_DRACULA) {
            registerBestPlay("CD", "ab");  
        } else {
            registerBestPlay("CD", "");
        }
    } else if (whoAmI(gameState) == PLAYER_DR_SEWARD) {
        giveMessage(gameState, PLAYER_DR_SEWARD, array);
        if (giveMeTheRound(gameState) == 0) {
            registerBestPlay("AT", "");          
        } else if (whereIs(gameState, PLAYER_DR_SEWARD) == ST_JOSEPH_AND_ST_MARYS){
            if (strcmp((char *)array, "cdefg") == 0) {
                registerBestPlay("SJ", "cdefg");
            } else {
                registerBestPlay("SJ", "");
            }
        } else if (howHealthyIs(gameState, PLAYER_DR_SEWARD) < GAME_START_HUNTER_LIFE_POINTS) {
            if (strcmp((char *)array, "cdefg") == 0) {
                registerBestPlay(myLocation(gameState), "cdefg");
            } else {
                registerBestPlay(myLocation(gameState), "");
            }     
        } else {
            switch (whereIs(gameState, PLAYER_DR_SEWARD)) {
                case ATHENS:
                    registerBestPlay("VA", ""); break;
                case SALONICA:
                    registerBestPlay("IO", ""); break;
                case VALONA:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("AT", ""); break;
                    } else {
                        registerBestPlay("SA", ""); break;
                    }
                case IONIAN_SEA:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("VA", "cdefg"); break;
                    } else {
                        registerBestPlay("AS", ""); break;
                    }
                case ADRIATIC_SEA:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("IO", "cdefg"); break;
                    } else {
                        registerBestPlay("BI", ""); break;
                    }
                case BARI:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("AS", "cdefg"); break;
                    } else {
                        registerBestPlay("NP", ""); break;
                    }
                case NAPLES:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("BI", "cdefg");  break;
                    } else {
                        registerBestPlay("RO", ""); break;
                    }
                case ROME:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("NP", "cdefg"); break;
                    } else {
                        registerBestPlay("FL", ""); break;
                    }
                case FLORENCE:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("RO", "cdefg"); break;
                    } else {
                        registerBestPlay("VE", ""); break;
                    }
                case VENICE:
                    registerBestPlay("MI", ""); break;
                case MILAN:
                    registerBestPlay("GO", ""); break;
                case GENOA:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("FL", "cdefg"); break;
                    } else {
                        registerBestPlay("TS", ""); break;
                    }
                case TYRRHENIAN_SEA:
                    registerBestPlay("CG", ""); break;
                case CAGLIARI:
                    registerBestPlay("MS", ""); break;
                case MEDITERRANEAN_SEA:
                    registerBestPlay("MR", ""); break;
                case MARSEILLES:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("GO", "cdefg"); break;
                    } else {
                        registerBestPlay("TO", ""); break;
                    }
                case TOULOUSE:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("MR", "cdefg"); break;
                    } else {
                        registerBestPlay("BA", ""); break;
                    }
                case BARCELONA:
                    registerBestPlay("SR", ""); break;
                case SARAGOSSA:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("TO", "cdefg"); break;
                    } else {
                        registerBestPlay("AL", ""); break;
                    }
                case ALICANTE:
                    registerBestPlay("GR", ""); break;
                case GRANADA:
                    registerBestPlay("CA", ""); break;
                case CADIZ:
                    registerBestPlay("LS", ""); break;
                case LISBON:
                    registerBestPlay("SN", ""); break;
                case SANTANDER:
                    registerBestPlay("MA", ""); break;
                case MADRID:
                    registerBestPlay("SR", "cdefg"); break;
                case SARAJEVO:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("VA", "cdefg"); break;
                    } else {
                        registerBestPlay("VA", ""); break;
                    }
            }
        }
    } else if (whoAmI(gameState) == PLAYER_VAN_HELSING) {
        giveMessage(gameState, PLAYER_VAN_HELSING, array);
        if (giveMeTheRound(gameState) == 0) {
            registerBestPlay("VR", "");
        } else if (whereIs(gameState, PLAYER_VAN_HELSING) == ST_JOSEPH_AND_ST_MARYS){
            if (strcmp((char *)array, "cdefg") == 0) {
                registerBestPlay("SJ", "cdefg");
            } else {
                registerBestPlay("ZA", "");
            }
        } else if (howHealthyIs(gameState, PLAYER_VAN_HELSING) < GAME_START_HUNTER_LIFE_POINTS) {
            if (strcmp((char *)array, "cdefg") == 0) {
                registerBestPlay(myLocation(gameState), "cdefg");
            } else {
                registerBestPlay(myLocation(gameState), "");
            }
        } else {
            switch (whereIs(gameState, PLAYER_VAN_HELSING)) {
                case VARNA:
                    registerBestPlay("SO", ""); break;
                case SOFIA:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("VR", "cdefg"); break;    
                    } else {
                        registerBestPlay("BC", ""); break;
                    }
                case BUCHAREST:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("SO", "cdefg"); break;
                    } else {
                        registerBestPlay("BE", ""); break;
                    }
                case BELGRADE:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("BC", "cdefg"); break;
                    } else {
                        registerBestPlay("SJ", ""); break;
                    }
                case SARAJEVO:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("BE", "cdefg"); break;
                    } else {
                        registerBestPlay("ZA", ""); break;
                    }
                case ZAGREB:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("SJ", "cdefg"); break;
                    } else {
                        registerBestPlay("MU", ""); break;
                    }
                case MUNICH:
                    if (strcmp((char *)array, "cdefg") == 0) { 
                        registerBestPlay("ZA", "cdefg"); break;
                    } else {
                        registerBestPlay("NU", ""); break;
                    }
                case NUREMBURG:
                    registerBestPlay("LI", ""); break;
                case LEIPZIG:
                    registerBestPlay("FR", ""); break;
                case FRANKFURT:
                    registerBestPlay("ST", ""); break;
                case STRASBOURG:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("ZU", "cdefg"); break;
                    } else {
                        registerBestPlay("BU", ""); break;
                    }
                case BRUSSELS:
                    registerBestPlay("LE", ""); break;
                case LE_HAVRE:
                    registerBestPlay("NA", ""); break;
                case NANTES:
                    registerBestPlay("BO", ""); break;
                case BORDEAUX:
                    registerBestPlay("CF", ""); break;
                case CLERMONT_FERRAND:
                    registerBestPlay("PA", ""); break;
                case PARIS:
                    registerBestPlay("GE", ""); break;
                case GENEVA:
                    registerBestPlay("ST", "cdefg"); break;
                case ZURICH:
                    registerBestPlay("MI", "cdefg"); break;
                case MILAN:
                    registerBestPlay("VE", "cdefg"); break;
                case VENICE:
                    registerBestPlay("MU", "cdefg"); break;
            }
        }   
    } else if (whoAmI(gameState) == PLAYER_MINA_HARKER) {
        giveMessage(gameState, PLAYER_MINA_HARKER, array);
        if (giveMeTheRound(gameState) == 0) {
            registerBestPlay("CN", "");
        } else if (whereIs(gameState, PLAYER_MINA_HARKER) == ST_JOSEPH_AND_ST_MARYS){
            if (strcmp((char *)array, "cdefg") == 0) {
                registerBestPlay("SZ", "cdefg");
            } else {
                registerBestPlay("SZ", "");
            }
        } else if (howHealthyIs(gameState, PLAYER_MINA_HARKER) < GAME_START_HUNTER_LIFE_POINTS) {
            if (strcmp((char *)array, "cdefg") == 0) {
                registerBestPlay(myLocation(gameState), "cdefg");
            } else {
                registerBestPlay(myLocation(gameState), "");
            }
        } else {
            switch (whereIs(gameState, PLAYER_MINA_HARKER)) {
                case CONSTANTA:
                    registerBestPlay("GA", ""); break;
                case GALATZ:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("CN", "cdefg"); break;
                    } else {
                        registerBestPlay("KL", ""); break;
                    }
                case KLAUSENBURG:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("GA", "cdefg"); break;
                    } else {
                        registerBestPlay("SZ", ""); break;
                    }
                case SZEGED:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("KL", "cdefg"); break;
                    } else {
                        registerBestPlay("BD", ""); break;
                    }
                case BUDAPEST:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("SZ", "cdefg"); break;
                    } else {
                        registerBestPlay("VI", ""); break;
                    }
                case VIENNA:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("BD", "cdefg"); break;
                    } else {
                        registerBestPlay("PR", ""); break;
                    }
                case PRAGUE:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("VI", "cdefg"); break;
                    } else {
                        registerBestPlay("BR", ""); break;
                    }
                case BERLIN:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("PR", "cdefg"); break;
                    } else {
                        registerBestPlay("HA", ""); break;
                    }
                case HAMBURG:
                    if (strcmp((char *)array, "cdefg") == 0) {
                       registerBestPlay("BR", "cdefg"); break;
                    } else {
                        registerBestPlay("CO", ""); break;
                    }
                case COLOGNE:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("HA", "cdefg"); break;
                    } else {
                        registerBestPlay("AM", ""); break;
                    }
                case AMSTERDAM:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("CO", "cdefg"); break;
                    } else {
                        registerBestPlay("NS", ""); break;
                    }
                case NORTH_SEA:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("AM", "cdefg"); break;
                    } else {
                        registerBestPlay("ED", ""); break;
                    }
                case EDINBURGH:
                    registerBestPlay("MN", ""); break;
                case MANCHESTER:
                    registerBestPlay("LV", ""); break;
                case LIVERPOOL:
                    registerBestPlay("SW", ""); break;
                case SWANSEA:
                    registerBestPlay("IR", ""); break;
                case IRISH_SEA:
                    registerBestPlay("DU", ""); break;
                case DUBLIN:
                    registerBestPlay("GW", ""); break;
                case GALWAY:
                    registerBestPlay("AO", ""); break;
                case ATLANTIC_OCEAN:
                    registerBestPlay("EC", ""); break;
                case ENGLISH_CHANNEL:
                    if (strcmp((char *)array, "cdefg") == 0) {
                        registerBestPlay("NS", "cdefg"); break;
                    } else {
                        registerBestPlay("PL", ""); break;
                    }
                case PLYMOUTH:
                    registerBestPlay("LO", ""); break;
                case LONDON:
                    registerBestPlay("EC", "cdefg"); break;
            }
        }
    }
}
