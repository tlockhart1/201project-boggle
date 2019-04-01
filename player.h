#include <stdio.h>
#include <stdlib.h>
#include "da.h"

typedef struct player PLAYR;

extern PLAYR *newPLAYR(char *, char *);
extern int getPLAYRwins(PLAYR *);
extern int getPLAYRlosses(PLAYR *);
extern char *getPLAYRname(PLAYR *);
extern void setPLAYRwins(PLAYR *, int);
extern void setPLAYRlosses(PLAYR *, int);
extern void incrementPLAYRwins(PLAYR *);
extern void incrementPLAYRlosses(PLAYR *);
extern void updatePLAYRs(PLAYR *, PLAYR *, int, int);
extern void writePLAYRlog(PLAYR *);
extern void freePLAYR(void *);
extern DA *getPLAYRlog(PLAYR *);
extern void printPLAYR(void *, FILE *);
