#ifndef __STRING_INCLUDED__
#define __STRING_INCLUDED__

#include <stdio.h>

typedef struct STRING STRING;

extern STRING *newSTRING(char*);
extern char *getSTRING(STRING *);
extern char *setSTRING(STRING *, char*);
extern void displaySTRING(void *, FILE *);
extern void freeSTRING(void *);
extern int compareSTRING(void*, void*);

#endif
