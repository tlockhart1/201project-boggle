#include "da.h"
#include "grid.h"
#include <stdlib.h>
#include <stdio.h>

struct grid{
	DA *guts;	
	int rows;
	int cols;
	void (*display) (void *, FILE *);
};

extern GRID *newGRID(void (*d)(void *, FILE *), void (*f) (void *), int r, int c){
	GRID *this_grid = (GRID *) malloc(sizeof(GRID));
	this_grid->guts = newDA();
	this_grid->rows = r;
	this_grid->cols = c;
	this_grid->display = d;
	
	/* initialize each row */
	for(int i = 0; i < r; i++){ 
		DA *col_0 = newDA();
		setDAdisplay(col_0, d);
		setDAfree(col_0, f);
		insertDAback(this_grid->guts, col_0);
	}

	return this_grid;
}

extern void fillGRIDrow(GRID *this_grid, void *items, int row){
	if(row >= 0 && row <= this_grid->rows)
		insertDAback(getDA(this_grid->guts, row), items);
	else printf("col is outside bounds. - fillGRIDrow\n");
}	

extern void *getGRIDcell(GRID *this_grid, int r, int c){
	return getDA(getDA(this_grid->guts, r), c);
}

extern int getGRIDrows(GRID *this_grid){
	return this_grid->rows;
}

extern int getGRIDcols(GRID *this_grid){
	return this_grid->cols;	
}

extern void printGRID(GRID *this_grid, FILE *where){
	for (int i = 0; i < this_grid->rows; i++){
		for(int j = 0; j < this_grid->cols; j++){
			this_grid->display(getDA(getDA(this_grid->guts, i), j), where);
		}
		fprintf(where, "\n");
	}
}

extern void freeGRID(void *this_grid){
	GRID *a = (GRID*)this_grid;
	for (int i = 0; i < a->rows; i++){
		freeDA(getDA(a->guts, i));	
	}
	freeDA(a->guts);
	free(a);
}


