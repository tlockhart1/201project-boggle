#include "stack.h"
#include "da.h"

struct stack{
		DA *guts; 
		void (*display)(void *, FILE *);
		void (*free)(void *);
		int debug_flag;
    };

STACK *newSTACK(void){
	STACK *my_stack;
	my_stack = (STACK *) malloc(sizeof(STACK));
	my_stack->display = NULL;
	my_stack->free = NULL;
	my_stack->debug_flag = 0;
	my_stack->guts = newDA();
	return my_stack;
}
void setSTACKdisplay(STACK *items, void (*d)(void *, FILE *)){
	items->display = d;
	setDAdisplay(items->guts, d);
}
void setSTACKfree(STACK *items, void (*f)(void *)){
	items->free = f;
	setDAfree(items->guts, f);
}
void push(STACK *items, void *value){
	insertDAback(items->guts, value);
}
void *pop(STACK *items){
	return removeDAback(items->guts);
}
void *peekSTACK(STACK *items){
	return getDA(items->guts, sizeDA(items->guts)-1);
}
int sizeSTACK(STACK *items){
	return sizeDA(items->guts);
}
void displaySTACK(STACK *items, FILE *where){
	if(items->debug_flag == 0){
		printf("|");
		for(int i = sizeDA(items->guts)-1; i > -1; i--){ 
			items->display(getDA(items->guts, i), where);
			if(i != 0) printf(",");
		}
		printf("|");
	}
	else if(items->debug_flag > 0){
		displayDA(items->guts, where);
	}
}
void freeSTACK(STACK *items){
	freeDA(items->guts);
	free(items);
}
int debugSTACK(STACK *items, int level){
	int debug_prev = items->debug_flag;
	items->debug_flag = level;
	if(level > 1){
		debugDA(items->guts, 1);
	}
	else if(level <= 1){
		debugDA(items->guts, 0);
	}
	return debug_prev;
}
