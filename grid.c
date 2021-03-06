#include "da.h"
#include "grid.h"
#include <stdlib.h>
#include <stdio.h>

struct grid{
	DA *guts;	
	int rows;
	int cols;
	int current_row;
	int current_col;
	void (*display) (void *, FILE *);
	void (*free) (void *);
};

extern GRID *newGRID(void (*d)(void *, FILE *), void (*f) (void *), int r, int c){
	GRID *this_grid = (GRID *) malloc(sizeof(GRID));
	this_grid->guts = newDA();
	this_grid->rows = r;
	this_grid->cols = c;
	this_grid->display = d;
	this_grid->free = f;
	
	/* initialize each row */
	for(int i = 0; i < r; i++){ 
		DA *col_0 = newDA();
		setDAdisplay(col_0, d);
		setDAfree(col_0, f);
		insertDAback(this_grid->guts, col_0);
	}

	this_grid->current_row = 0;
	this_grid->current_col = 0;

	return this_grid;
}

extern void insertGRIDseq(GRID *this_grid, void *item){
	if(this_grid->current_row == this_grid->rows){
		printf("You're trying to go out of bounds. - insertGRID()\n");
		return;
	}
	insertDA(getDA(this_grid->guts, this_grid->current_row), this_grid->current_col, item);
	this_grid->current_col++;
	if(this_grid->current_col == this_grid->cols){
	       this_grid->current_col = 0;			//exceded bounds of columns
	       this_grid->current_row++;			//therefore go to next row @ col index 0
	}
}

extern int getGRIDcc(GRID *this_grid){		
	return this_grid->current_col;
}

extern int getGRIDcr(GRID *this_grid){
	return this_grid->current_row;	
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

extern void freeGRID(GRID *this_grid){
	for (int i = 0; i < this_grid->rows; i++){
		freeDA(getDA(this_grid->guts, i));	
	}
	freeDA(this_grid->guts);
	free(this_grid);
}


