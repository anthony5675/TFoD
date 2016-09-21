// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,int);
void dropFrom(Set,int);
int  isElem(Set,int);
int  nElems(Set);
void showSet(Set);

#endif
