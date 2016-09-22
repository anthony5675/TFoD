// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "set.h"
#include "Places.h"


#define strEQ(s,t) (s == t)
#define strLT(s,t) (s < t)

typedef struct Node *Link;

typedef struct Node {
	int val;
	Link  next;
} Node;
	
typedef struct SetRep {
	int   nelems;
	Link  elems;
} SetRep;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,int);
void dropFrom(Set,int);
int  isElem(Set,int);
int  nElems(Set);

static Link newNode(int);
static void disposeNode(Link);
static int  findNode(Link,int,Link *,Link *);


// newSet()
// - create an initially empty Set
Set newSet()
{
	Set new = malloc(sizeof(SetRep));
	assert(new != NULL);
	new->nelems = 0;
	new->elems = NULL;
	return new;
}

// disposeSet(Set)
// - clean up memory associated with Set
void disposeSet(Set s)
{
	if (s == NULL) return;
	Link next, curr = s->elems;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);	
		curr = next;
	}
}

// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto(Set s, int str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (found) return; // already in Set
	Link new = newNode(str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	}
	else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}

// dropFrom(Set,Str)
// - ensure that Str is not in Set
void dropFrom(Set s, int str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,int,&curr,&prev);
	if (!found) return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	disposeNode(curr);
}

// isElem(Set,Str)
// - check whether Str is contained in Set
int isElem(Set s, int str)
{
	assert(s != NULL);
	Link curr, prev;
	return findNode(s->elems,str,&curr,&prev);
}

// nElems(Set)
// - return # elements in Set
int  nElems(Set s)
{
	assert(s != NULL);
	return s->nelems;
}

// showSet(Set)
// - display Set (for debugging)
void showSet(Set s)
{
	Link curr;
	if (s->nelems == 0)
		printf("Set is empty\n");
	else {
		printf("Set has %d elements:\n",s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			printf("[%03d] %d\n", id, curr->val);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static Link newNode(int str)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = val;
	new->next = NULL;
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
	free(curr->val);
	free(curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode(Link list, int str, Link *cur, Link *pre)
{
	Link curr = list, prev = NULL;
	while (curr != NULL && strLT(curr->val,str)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr; *pre = prev;
	return (curr != NULL && strEQ(str,curr->val));
}

int *getSet(Set s){
	int *locs = malloc(sizeof(int)*nElems(s))
	int i = 0;
	Link elems;
	for(elems = s->elems; elems != NULL; elems = elems->next){
		locs[i] = elems->val;
		i++;
	}
	return locs;

}