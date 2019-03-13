#include "da.h"

struct da{
	void (*display)(void *, FILE *);
	void (*free)(void *);
	int debug_flag;
	void **list;
	int in_size, mem_size;
	};

DA *newDA(void){
	DA *object;
	object = (DA *) malloc(sizeof(DA));
	object->display = NULL;
	object->free = NULL;
	object->debug_flag = 0;
	object->in_size = 0;
	object->mem_size = 1;
	object->list = (void*)malloc (sizeof(void*) * object->mem_size); 
	return object;
}
void setDAdisplay(DA *items, void (*d)(void *, FILE *)){
	items->display = d;
}
void setDAfree(DA *items, void (*f)(void *)){
	items->free = f;
}
int debugDA(DA *items, int level){
	int prev_flag = items->debug_flag;
	items->debug_flag = level;
	return prev_flag;
}
void growDA(DA *items){
	items->mem_size = items->mem_size*2;
	void **newlist; newlist = (void*) malloc(sizeof(void*) * items->mem_size);
	for(int i = 0; i < items->in_size; i++) newlist[i] = items->list[i];
	free(items->list);
	items->list = newlist;
}
void shrinkDA(DA *items){
	items->mem_size = items->mem_size/2;
	if(sizeDA(items) == 0 && items->mem_size == 2) items->mem_size = 1;
	void **newlist; newlist = (void*) malloc(sizeof(void*) * items->mem_size);
	for(int i = 0; i < items->in_size; i++) newlist[i] = items->list[i];
	free(items->list);
	items->list = newlist;
}
void insertDA(DA *items, int index, void *value){
	if(items->in_size == items->mem_size) growDA(items);
	if(index == sizeDA(items)) items->list[items->in_size] = value;
	else{
		for(int i = sizeDA(items); i > index; i--) items->list[i] = items->list[i-1];	
		items->list[index] = value;
	}
	items->in_size++;
}
void *removeDA(DA *items, int index){
	void *value = getDA(items, index);
	if(items->in_size > 0){
		items->in_size--;
		for (int i = index; i < sizeDA(items); i++) items->list[i] = items->list[i+1];
		//items->in_size--;
		if(items->in_size < (items->mem_size/4)){ 
			if((items->mem_size/4) < 1)
				return value; 
			else shrinkDA(items);
		}
		return value;
	}
	else return NULL;
}
void unionDA(DA *recipient, DA *donor){
	//void (*m)(void *, FILE *);
	//void (*r)(void *);
	int p = sizeDA(donor);
	//m = donor->display;
	//r = donor->free;
	//p = donor->debug_flag;
	for(int i = 0; i < p; i++) insertDAback(recipient, donor->list[i]);	
	free(donor->list);
	donor->mem_size = 1;
	donor->in_size = 0;
	donor->list = malloc(sizeof(void*)*donor->mem_size);
}
void *getDA(DA *items, int index){
	if(index >= 0 && index < items->in_size) return items->list[index];
	else return NULL;
}
void *setDA(DA *items, int index, void *value){
	if(index == items->in_size){
		insertDAback(items, value);
		return NULL; 				//changed to NULL since nothing is replace// old ---> items->list[index];
	}else if(index < items->in_size
			 && index > -1) return items->list[index] = value;
	else
		return NULL;
}
int sizeDA(DA *items){
	return items->in_size;
}
void displayDA(DA *items, FILE *fp){
	printf("[");
	for(int i = 0; i < sizeDA(items); i++){
		if(items->display == NULL) fprintf(fp,"%p", items->list[i]);
		else items->display(items->list[i], fp);
		if(i != sizeDA(items)-1) printf(",");	
	}
	if(items->debug_flag > 0){
		if(sizeDA(items) == 0) fprintf(fp,"[%d]", items->mem_size - items->in_size);
		else fprintf(fp,",[%d]", items->mem_size - items->in_size); 
	}
	printf("]");

}
void freeDA(DA *items){
	if(items->free != NULL){
		for(int i = 0; i < sizeDA(items); i++) items->free(items->list[i]);
	}
	free(items->list);
	free(items);
}
