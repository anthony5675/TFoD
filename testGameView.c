// testGameView.c ... test the GameView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

int main()
{
    int i;
    GameView gv;
    
    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 0);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");        
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);    
    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    printf("passed\n");
    disposeGameView(gv);


    printf("Testing to see it is turn zero until everyone has had a turn");
    PlayerMessage messages6[] = {"Hello","Rubbish","Stuff",""};
    gv = newGameView("GST.... SAO.... HZU.... MBB....", messages6);
    assert(getRound(gv) == 0);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == UNKNOWN_LOCATION);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");
 
    printf("Testing hunter dieing\n");
    PlayerMessage messages8[] = {"","","","","","","","","","",
                                 "","","","","",""};
    gv = newGameView("GKL.... SKL.... HKL.... MAO.... DGA.... "
                     "GGAD... SKL.... HKL.... MAO.... DBC.... "
                     "GBCD... SKL.... HKL.... MAO.... DSO.... "
                     "GSOD...", messages8);
    // dracs health should be lower
    printf("drac health = %d\n", getHealth(gv, PLAYER_DRACULA));
    assert(getHealth(gv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS - 3 * LIFE_LOSS_HUNTER_ENCOUNTER);
    // the hunter should be zero
    printf("%d\n", getHealth(gv, PLAYER_LORD_GODALMING));
    assert(getHealth(gv, PLAYER_LORD_GODALMING)  <= 0);
    // the game score shoouldn't have changed yet
    assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_HUNTER_HOSPITAL - 3);
    // it should show that the hunter is in the hospital
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == ST_JOSEPH_AND_ST_MARYS);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");
 
    printf("Testing hunter dieing - one turn later\n");
    PlayerMessage messages9[] = {"","","","","","","","","","",
                                 "","","","","","","","","","",
                                 ""};
    gv = newGameView("GKL.... SKL.... HKL.... MAO.... DGA.... "
                     "GGAD... SKL.... HKL.... MAO.... DBC.... "
                     "GBCD... SKL.... HKL.... MAO.... DSO.... "
                     "GSOD... SKL.... HKL.... MAO.... DSA.... "
                     "GZA....", messages9);
    assert(getHealth(gv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS - 3 * LIFE_LOSS_HUNTER_ENCOUNTER);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_HUNTER_HOSPITAL - 4);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == ZAGREB);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");
 
 
    printf("Testing the score decreases after the first turn\n");
    PlayerMessage messages10[] = {"","","","",""};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DCD....", messages10);
    assert(getScore(gv) == GAME_START_SCORE - 1);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");
 
 
    printf("Testing the score decreases after the second turn\n");
    PlayerMessage messages11[] = {"","","","",""};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DCD.... "
                     "GST.... SAO.... HZU.... MBB.... DHI....", messages11);
    assert(getScore(gv) == GAME_START_SCORE - 2);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");
 
    printf("Testing the score decreases for mature vampire\n");
    PlayerMessage messages12[] = {"","","","","","","","","","",
                                  "","","","","","","","","","",
                                  "","","","","","","","","",""};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DCFV... "
                     "GST.... SAO.... HZU.... MBB.... DBO.... "
                     "GST.... SAO.... HZU.... MBB.... DTO.... "
                     "GST.... SAO.... HZU.... MBB.... DBA.... "
                     "GST.... SAO.... HZU.... MBB.... DSR.... "
                     "GST.... SAO.... HZU.... MBB.... DHI..V.", messages12);
    assert(getScore(gv) == GAME_START_SCORE - 6 - SCORE_LOSS_VAMPIRE_MATURES);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");
 
    printf("checking resting repair some health and not exceeding 9 life points\n");
    PlayerMessage messages13[] = {"","","","","","","","","","",""};
    gv = newGameView("GKL.... SKL.... HKL.... MAO.... DGA.... "
                     "GGAD... SKL.... HKL.... MAO.... DBC.... "
                     "GGA....", messages13);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) ==
            GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER + LIFE_GAIN_REST);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");
 
    printf("Testing that no actions occur after someone is dead\n");
    PlayerMessage messages14[] = {"","","","","","","","","","",
                                  "","","","","",""};
    gv = newGameView("GBO.... SAO.... HZU.... MBB.... DCF.... "
                     "GCFD... SAO.... HZU.... MBB.... DBO... "
                     "GBOD... SAO.... HZU.... MBB.... DTOT... "
                     "GTODT..", messages14);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 2 * LIFE_LOSS_HUNTER_ENCOUNTER);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");   
    return 0;
}

