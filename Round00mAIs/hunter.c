// hunter.c
// Implementation of your "Fury of Dracula" hunter AI
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Game.h"
#include "HunterView.h"

static LocationID isDraculaNear (LocationID *vicinity, int numLocations, LocationID draculaL); //Figures out if dracula is reachable from my location.
static int notMyPath (LocationID thePath[], LocationID myLocations[], int numLocations); //Checks if my location is/isn't in my path.
static LocationID *myPath (LocationID sewa[], LocationID vanh[], LocationID mina[], PlayerID player); //Gives me my path.

void decideHunterMove(HunterView gameState)
{
    PlayerMessage array; //array to store the message
    char *forward = "";
    char *backward = "cdefg";
    int numLocations = 0;

    //Paths
    LocationID sewa[23] = {ALICANTE, ATHENS, BARCELONA, BARI, CADIZ, CAGLIARI, FLORENCE, GENOA, GRANADA, LISBON, MADRID, MARSEILLES, MILAN, NAPLES, ROME, SALONICA, SANTANDER, SARAGOSSA, ST_JOSEPH_AND_ST_MARYS, SARAJEVO, TOULOUSE, VALONA, VENICE};
    LocationID vanh[23] = {BELGRADE, BORDEAUX, BRUSSELS, BUCHAREST, CLERMONT_FERRAND, COLOGNE, FRANKFURT, GENEVA, LE_HAVRE, LEIPZIG, MILAN, MUNICH, NANTES, NUREMBURG, PARIS, SARAJEVO, SOFIA, ST_JOSEPH_AND_ST_MARYS, STRASBOURG, VARNA, VENICE, ZAGREB, ZURICH};
    LocationID mina[20] = {AMSTERDAM, BERLIN, BUDAPEST, COLOGNE, CONSTANTA, DUBLIN, EDINBURGH, GALATZ, GALWAY, HAMBURG, KLAUSENBURG, LIVERPOOL, LONDON, MANCHESTER, PLYMOUTH, PRAGUE, ST_JOSEPH_AND_ST_MARYS, SWANSEA, SZEGED, VIENNA};


    //Get the message.
    giveMessage(gameState, whoAmI(gameState), array);

    //General player moves.
    //Round 0 moves.
    if (giveMeTheRound(gameState) == 0) {
        if (whoAmI(gameState) == PLAYER_LORD_GODALMING) {
            registerBestPlay("CD", forward);  
        } else if (whoAmI(gameState) == PLAYER_DR_SEWARD) {
            registerBestPlay("AT", forward); 
        } else if (whoAmI(gameState) == PLAYER_VAN_HELSING) {
            registerBestPlay("VR", forward);
        } else {
            registerBestPlay("CN", forward);
        }
    //If dracula is where I am, stay.
    } else if (whereIs(gameState, PLAYER_DRACULA) == whereIs(gameState, whoAmI(gameState))) {
        registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_DRACULA)), (char *)array);
    //If dracula is reachable from where I am, go there.
    } else if (isDraculaNear (whereCanIgo(gameState, &numLocations, TRUE, TRUE, FALSE), numLocations, whereIs(gameState, PLAYER_DRACULA)) == 0) {
        if (notMyPath(myPath(sewa, vanh, mina, whoAmI(gameState)), whereCanIgo(gameState, &numLocations, TRUE, TRUE, FALSE), numLocations) == 1) {
            registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_DRACULA)), idToAbbrev(whereIs(gameState, whoAmI(gameState))));
        } else {
            registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_DRACULA)), (char *)array);
        }
    //Return to my path if I've lost dracula.
    } else if (strcmp((char *)array, forward) != 0 && strcmp((char *)array, backward) != 0) {
        if (isDraculaNear(whereCanIgo(gameState, &numLocations, TRUE, TRUE, FALSE), numLocations, abbrevToID((char *)array)) == 1) {
            registerBestPlay(idToAbbrev(whereIs(gameState, whoAmI(gameState))), (char *)array);
        } else {
            registerBestPlay((char *)array, forward);
        }
    //Specific player moves.
    } else if (whoAmI(gameState) == PLAYER_LORD_GODALMING) {
        giveMessage(gameState, PLAYER_LORD_GODALMING, array);
        if (whereIs(gameState, PLAYER_LORD_GODALMING) == ST_JOSEPH_AND_ST_MARYS){ //If i get tped to the hospital.
            registerBestPlay("SZ", forward);
        } else if (whereIs(gameState, PLAYER_LORD_GODALMING) == SZEGED) { //On my way back from the hospital.
            registerBestPlay("KL",forward);
        } else if(whereIs(gameState, PLAYER_DRACULA) == CASTLE_DRACULA) {
            registerBestPlay("CD", forward);
        } else if(howHealthyIs(gameState, PLAYER_LORD_GODALMING) < GAME_START_HUNTER_LIFE_POINTS && whereIs(gameState, PLAYER_DRACULA) != CASTLE_DRACULA) {
            registerBestPlay("GA", forward);  
        } else {
            registerBestPlay("CD", forward);
        }
    } else if (whoAmI(gameState) == PLAYER_DR_SEWARD) {
        giveMessage(gameState, PLAYER_DR_SEWARD, array);
        if (whereIs(gameState, PLAYER_DR_SEWARD) == ST_JOSEPH_AND_ST_MARYS){
            if (strcmp((char *)array, backward) == 0) {
                registerBestPlay("SJ", backward);
            } else {
                registerBestPlay("SJ", forward);
            }    
        } else if (howHealthyIs(gameState, PLAYER_DR_SEWARD) < 3) {
            if (strcmp((char *)array, backward) == 0) {
                registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_DR_SEWARD)), backward);
            } else {
                registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_DR_SEWARD)), forward);
            }     
        } else {
            switch (whereIs(gameState, PLAYER_DR_SEWARD)) {
                case ATHENS:
                    registerBestPlay("VA", forward); break;
                case SALONICA:
                    registerBestPlay("IO", forward); break;
                case VALONA:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("AT", forward); break;
                    } else {
                        registerBestPlay("SA", forward); break;
                    }
                case IONIAN_SEA:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("VA", backward); break;
                    } else {
                        registerBestPlay("AS", forward); break;
                    }
                case ADRIATIC_SEA:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("IO", backward); break;
                    } else {
                        registerBestPlay("BI", forward); break;
                    }
                case BARI:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("AS", backward); break;
                    } else {
                        registerBestPlay("NP", forward); break;
                    }
                case NAPLES:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("BI", backward);  break;
                    } else {
                        registerBestPlay("RO", forward); break;
                    }
                case ROME:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("NP", backward); break;
                    } else {
                        registerBestPlay("FL", forward); break;
                    }
                case FLORENCE:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("RO", backward); break;
                    } else {
                        registerBestPlay("VE", forward); break;
                    }
                case VENICE:
                    registerBestPlay("MI", forward); break;
                case MILAN:
                    registerBestPlay("GO", forward); break;
                case GENOA:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("FL", backward); break;
                    } else {
                        registerBestPlay("TS", forward); break;
                    }
                case TYRRHENIAN_SEA:
                    registerBestPlay("CG", forward); break;
                case CAGLIARI:
                    registerBestPlay("MS", forward); break;
                case MEDITERRANEAN_SEA:
                    registerBestPlay("MR", forward); break;
                case MARSEILLES:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("GO", backward); break;
                    } else {
                        registerBestPlay("TO", forward); break;
                    }
                case TOULOUSE:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("MR", backward); break;
                    } else {
                        registerBestPlay("BA", forward); break;
                    }
                case BARCELONA:
                    registerBestPlay("SR", forward); break;
                case SARAGOSSA:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("TO", backward); break;
                    } else {
                        registerBestPlay("AL", forward); break;
                    }
                case ALICANTE:
                    registerBestPlay("GR", forward); break;
                case GRANADA:
                    registerBestPlay("CA", forward); break;
                case CADIZ:
                    registerBestPlay("LS", forward); break;
                case LISBON:
                    registerBestPlay("SN", forward); break;
                case SANTANDER:
                    registerBestPlay("MA", forward); break;
                case MADRID:
                    registerBestPlay("SR", backward); break;
                case SARAJEVO:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("VA", backward); break;
                    } else {
                        registerBestPlay("VA", forward); break;
                    }
                default:
                    registerBestPlay(idToAbbrev(whereCanIgo(gameState, &numLocations, TRUE, TRUE, FALSE)[rand() % numLocations]), (char *)array); break;
            }
        }
    } else if (whoAmI(gameState) == PLAYER_VAN_HELSING) {
        giveMessage(gameState, PLAYER_VAN_HELSING, array);
        if (whereIs(gameState, PLAYER_VAN_HELSING) == ST_JOSEPH_AND_ST_MARYS){
            if (strcmp((char *)array, backward) == 0) {
                registerBestPlay("SJ", backward);
            } else {
                registerBestPlay("ZA", forward);
            }
        } else if (howHealthyIs(gameState, PLAYER_VAN_HELSING) < 3) {
            if (strcmp((char *)array, backward) == 0) {
                registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_VAN_HELSING)), backward);
            } else {
                registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_VAN_HELSING)), forward);
            }
        } else {
            switch (whereIs(gameState, PLAYER_VAN_HELSING)) {
                case VARNA:
                    registerBestPlay("SO", forward); break;
                case SOFIA:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("VR", backward); break;    
                    } else {
                        registerBestPlay("BC", forward); break;
                    }
                case BUCHAREST:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("SO", backward); break;
                    } else {
                        registerBestPlay("BE", forward); break;
                    }
                case BELGRADE:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("BC", backward); break;
                    } else {
                        registerBestPlay("SJ", forward); break;
                    }
                case SARAJEVO:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("BE", backward); break;
                    } else {
                        registerBestPlay("ZA", forward); break;
                    }
                case ZAGREB:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("SJ", backward); break;
                    } else {
                        registerBestPlay("MU", forward); break;
                    }
                case MUNICH:
                    if (strcmp((char *)array, backward) == 0) { 
                        registerBestPlay("ZA", backward); break;
                    } else {
                        registerBestPlay("NU", forward); break;
                    }
                case NUREMBURG:
                    registerBestPlay("LI", forward); break;
                case LEIPZIG:
                    registerBestPlay("FR", forward); break;
                case FRANKFURT:
                    registerBestPlay("ST", forward); break;
                case STRASBOURG:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("ZU", backward); break;
                    } else {
                        registerBestPlay("BU", forward); break;
                    }
                case BRUSSELS:
                    registerBestPlay("LE", forward); break;
                case LE_HAVRE:
                    registerBestPlay("NA", forward); break;
                case NANTES:
                    registerBestPlay("BO", forward); break;
                case BORDEAUX:
                    registerBestPlay("CF", forward); break;
                case CLERMONT_FERRAND:
                    registerBestPlay("PA", forward); break;
                case PARIS:
                    registerBestPlay("GE", forward); break;
                case GENEVA:
                    registerBestPlay("ST", backward); break;
                case ZURICH:
                    registerBestPlay("MI", backward); break;
                case MILAN:
                    registerBestPlay("VE", backward); break;
                case VENICE:
                    registerBestPlay("MU", backward); break;
                default:
                    registerBestPlay(idToAbbrev(whereCanIgo(gameState, &numLocations, TRUE, TRUE, FALSE)[rand() % numLocations]), (char *)array); break;
            }
        }   
    } else if (whoAmI(gameState) == PLAYER_MINA_HARKER) {
        giveMessage(gameState, PLAYER_MINA_HARKER, array);
        if (whereIs(gameState, PLAYER_MINA_HARKER) == ST_JOSEPH_AND_ST_MARYS){
            if (strcmp((char *)array, backward) == 0) {
                registerBestPlay("SZ", backward);
            } else {
                registerBestPlay("SZ", forward);
            }
        } else if (howHealthyIs(gameState, PLAYER_MINA_HARKER) < 3) {
            if (strcmp((char *)array, backward) == 0) {
                registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_MINA_HARKER)), backward);
            } else {
                registerBestPlay(idToAbbrev(whereIs(gameState, PLAYER_MINA_HARKER)), forward);
            }
        } else {
            switch (whereIs(gameState, PLAYER_MINA_HARKER)) {
                case CONSTANTA:
                    registerBestPlay("GA", forward); break;
                case GALATZ:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("CN", backward); break;
                    } else {
                        registerBestPlay("KL", forward); break;
                    }
                case KLAUSENBURG:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("GA", backward); break;
                    } else {
                        registerBestPlay("SZ", forward); break;
                    }
                case SZEGED:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("KL", backward); break;
                    } else {
                        registerBestPlay("BD", forward); break;
                    }
                case BUDAPEST:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("SZ", backward); break;
                    } else {
                        registerBestPlay("VI", forward); break;
                    }
                case VIENNA:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("BD", backward); break;
                    } else {
                        registerBestPlay("PR", forward); break;
                    }
                case PRAGUE:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("VI", backward); break;
                    } else {
                        registerBestPlay("BR", forward); break;
                    }
                case BERLIN:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("PR", backward); break;
                    } else {
                        registerBestPlay("HA", forward); break;
                    }
                case HAMBURG:
                    if (strcmp((char *)array, backward) == 0) {
                       registerBestPlay("BR", backward); break;
                    } else {
                        registerBestPlay("CO", forward); break;
                    }
                case COLOGNE:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("HA", backward); break;
                    } else {
                        registerBestPlay("AM", forward); break;
                    }
                case AMSTERDAM:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("CO", backward); break;
                    } else {
                        registerBestPlay("NS", forward); break;
                    }
                case NORTH_SEA:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("AM", backward); break;
                    } else {
                        registerBestPlay("ED", forward); break;
                    }
                case EDINBURGH:
                    registerBestPlay("MN", forward); break;
                case MANCHESTER:
                    registerBestPlay("LV", forward); break;
                case LIVERPOOL:
                    registerBestPlay("SW", forward); break;
                case SWANSEA:
                    registerBestPlay("IR", forward); break;
                case IRISH_SEA:
                    registerBestPlay("DU", forward); break;
                case DUBLIN:
                    registerBestPlay("GW", forward); break;
                case GALWAY:
                    registerBestPlay("AO", forward); break;
                case ATLANTIC_OCEAN:
                    registerBestPlay("EC", forward); break;
                case ENGLISH_CHANNEL:
                    if (strcmp((char *)array, backward) == 0) {
                        registerBestPlay("NS", backward); break;
                    } else {
                        registerBestPlay("PL", forward); break;
                    }
                case PLYMOUTH:
                    registerBestPlay("LO", forward); break;
                case LONDON:
                    registerBestPlay("EC", backward); break;
                default:
                    registerBestPlay(idToAbbrev(whereCanIgo(gameState, &numLocations, TRUE, TRUE, FALSE)[rand() % numLocations]), (char *)array); break;
            }
        }
    }
}

//Figures out if dracula is reachable from my location.
static int isDraculaNear (LocationID vicinity[], int numLocations, LocationID draculaL) {

    int p = 0;
    while (p < numLocations) {
        if (vicinity[p] == draculaL) {
            return 0;
        }
        p++;
    }
    return 1;
}

//Checks if my location is/isn't in my path.
static int notMyPath (LocationID thePath[], LocationID myLocations[], int numLocations) {
    
    int q = 0, r = 0;
    while (r < numLocations) {
        while (q < sizeof(thePath)) {
            if (myLocations[r] == thePath[q]) {
                return 0;
            }
            q++;
        }
        r++;
    }
    return 1;
}

//Gives me my path.
static LocationID *myPath (LocationID sewa[], LocationID vanh[], LocationID mina[], PlayerID player) {

    if (player == PLAYER_DR_SEWARD) {
        return sewa;
    } else if (player == PLAYER_VAN_HELSING) {
        return vanh;
    } else if (player == PLAYER_MINA_HARKER){
        return mina;
    }
}
