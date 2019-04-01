#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "player.h"
#include "die.h"
#include "boggle.h"
#include "trie.h"
#include <string.h>

void printString(void *, FILE *);

int main(){
	int row = 10;
	int col = 10;
	BOGG *b = newBOGG(1000, row, col);
	loadBOGGdict(b);
	char *word = malloc(row*col+1);
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			solveBOGG(b, i, j, word, 0);
		}
	}	
	
	displayBOGG(b, stdout);
	printf("%d\n", sizeBOGGsolved(b));

	return 0;
}

void printString(void *item, FILE *where){
	char *str = (char*)item;
	fprintf(where, "%s", str);
}
