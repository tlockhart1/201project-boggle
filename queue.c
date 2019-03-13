#include "queue.h"
#include "cda.h"
struct queue{
		CDA *guts;  
		int debug_flag;
		void (*display)(void *, FILE *);
		void (*free)(void *);
	};

QUEUE *newQUEUE(void){
	QUEUE *queue = (QUEUE *) malloc(sizeof(QUEUE));
	queue->display = NULL;
	queue->free = NULL;
	queue->guts = newCDA();
	queue->debug_flag = 0;
	return queue;
}
void setQUEUEdisplay(QUEUE *items, void (*d)(void *, FILE *)){
	items->display = d;
	setCDAdisplay(items->guts, d);
}
void setQUEUEfree(QUEUE *items, void (*f)(void*)){
	items->free = f;
	setCDAfree(items->guts, f);
}
void enqueue(QUEUE *items, void *value){
	insertCDAback(items->guts, value);
}
void *dequeue(QUEUE *items){
	return removeCDAfront(items->guts);
}
void *peekQUEUE(QUEUE *items){
	return getCDA(items->guts, 0);
}
int sizeQUEUE(QUEUE *items){
	return sizeCDA(items->guts);
}
void displayQUEUE(QUEUE *items, FILE *where){
	if(items->debug_flag == 0){
		if(sizeCDA(items->guts) == 0) printf("<>");	
		else{
			printf("<");
				for(int i = 0; i < sizeCDA(items->guts); i++){
					items->display(getCDA(items->guts, i), where);
					if(i != sizeCDA(items->guts)-1) printf(",");
				}
			printf(">");
		}
	}
	else if(items->debug_flag > 0 ){
		displayCDA(items->guts, where);
	}
}
int debugQUEUE(QUEUE *items, int level){
	int prev_flag = items->debug_flag;
	items->debug_flag = level;
	if(level > 1){
		debugCDA(items->guts, 1);
	}
	else if(level <= 1){
		debugCDA(items->guts, 0);
	}
	return prev_flag;
}
void freeQUEUE(QUEUE *items){
	freeCDA(items->guts);
	free(items);	
}
