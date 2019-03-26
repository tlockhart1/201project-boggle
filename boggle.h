#include <stdlib.h>
#include <stdio.h>

typedef struct boggle BOGG;

extern BOGG *newBOGG(int, int, int);
extern char getBOGGchar(BOGG *, int, int);
extern void solveBOGG(BOGG *);
extern int scoreBOGGword(BOGG *, char *);
extern void printBOGG(BOGG *);
extern void displayBOGG(BOGG *, FILE *where);
