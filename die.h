#include <stdio.h>
#include <stdlib.h>

typedef struct game_die GDIE;

extern GDIE *newGDIE(void);
extern void setGDIEfree(GDIE *, (*)(void *));
extern void setGDIEdisplay(GDIE *, (*)(void *, FILE *));
extern void *getGDIEface(GDIE *, int);
extern void setGDIEfaces(GDIE *, void **, int);
extern void setGDIEface(GDIE *, int);
extern int setGDIEseed(GDIE *, int);
extern void *rollGDIE(GDIE *);
extern void printGDIE(GDIE *, FILE *);
extern void freeGDIE(void *);