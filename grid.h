#include <stdlib.h>
#include <stdio.h>

typedef struct grid GRID;

extern GRID *newGRID(void (*)(void *, FILE *), void (*) (void *), int r, int c);
extern void *getGRIDcell(GRID *, int r, int c);
extern void printGRID(GRID *, FILE *);
extern void *insertGRIDcell(GRID *, int r, int c);
extern void insertGRIDseq(GRID *, void *item);
extern int getGRIDcc(GRID *);
extern int getGRIDcr(GRID *);
extern int getGRIDrows(GRID *);
extern int getGRIDcols(GRID *);
extern void freeGRID(GRID *);

