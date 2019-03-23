#include <stdlib.h>
#include <stdio.h>
#include "grid.h"

void printString(void *, FILE *);

int main(){
	char *c [5] = { "a", "b", "c", "d", "e" };
	GRID *check = newGRID(printString, NULL, 5, 5);
	
	for (int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			fillGRIDrow(check, c[j], i);
		}
	}

	printGRID(check, stdout);

	return 0;
}

void printString(void *item, FILE *where){
	char *str = (char*)item;
	fprintf(where, "%s", str);
}
