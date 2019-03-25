#include <stdio.h>
#include <stdlib.h>

typedef struct player PLAYR;

extern *PLAYR newPLAYR(char *);
extern int getPLAYRwins(PLAYR *);
extern int getPLAYRlosses(PLAYR *);
extern void setPLAYRwins(PLAYR *, int);
extern void setPLAYRlosses(PLAYR *, int);
extern void freePLAYR(PLAYR *);
