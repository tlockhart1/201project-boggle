#include <stdlib.h>
#include <stdio.h>
#include "da.h"

typedef struct boggle BOGG;

extern BOGG *newBOGG(int, int, int);
extern void loadBOGGdict(BOGG *);
extern char getBOGGchar(BOGG *, int, int);
extern void solveBOGG(BOGG *, int, int, char *, int);
extern int scoreBOGGwords(BOGG *, DA *);
extern void printBOGG(BOGG *);
extern char *accessBOGGsolved(BOGG *, int);
extern int sizeBOGGsolved(BOGG *);
extern DA *getBOGGsolved(BOGG *);
extern void displayBOGG(BOGG *, FILE *where);
extern int getBOGGcols(BOGG *);
extern int getBOGGrows(BOGG *);
