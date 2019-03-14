#include "tnode.h"
#include <stdlib.h>

struct tnode{
	void *value;
	int debug_level;
	TNODE *right;
	TNODE *left;
	TNODE *parent;
	void (*display)(void *value, FILE *fp);
	void (*free)(void *value);
};

extern TNODE *newTNODE(void *v, TNODE *l, TNODE *r, TNODE *p){
	TNODE *n = malloc(sizeof(TNODE));
	n->debug_level = 0;
	setTNODEleft(n, l);
	setTNODEright(n, r);
	setTNODEparent(n, p);
	setTNODEvalue(n, v);
	n->display = NULL;
	n->free = NULL;
	return n;
}

extern void setTNODEdisplay(TNODE *n, void (*d)(void *value, FILE *fp)){
	n->display = d;	
}

extern void setTNODEfree(TNODE *n, void (*f)(void *value)){
	n->free = f;
}

extern void *getTNODEvalue(TNODE *n){
	return n->value;
}

extern void setTNODEvalue(TNODE *n, void *replacement){
	n->value = replacement;
}

extern TNODE *getTNODEleft(TNODE *n){
	return n->left;
}

extern void setTNODEleft(TNODE *n, TNODE *replacement){
	//free?
	n->left = replacement;
}

extern TNODE *getTNODEright(TNODE *n){
	return n->right;
}

extern void setTNODEright(TNODE *n, TNODE *replacement){
	//free?
	n->right = replacement;
}

extern TNODE *getTNODEparent(TNODE *n){
	return n->parent;
}

extern void setTNODEparent(TNODE *n, TNODE *replacement){
	//free?
	n->parent = replacement;
}

extern void displayTNODE(TNODE *n, FILE *fp){
	if(n->display == NULL) fprintf(fp, "buttholelol");// fprintf(fp, "&%p", &n->value);
	else if(n->display && n->debug_level > 0){
	       	n->display(n, fp);
		fprintf(fp, "&%p", n->value);
	}
	else n->display(n->value, fp);
}

extern int debugTNODE(TNODE *n, int level){
	int old = n->debug_level;
	n->debug_level = level;
	return old;
}

extern void freeTNODE(TNODE *n){
	if (n->free) n->free(getTNODEvalue(n));
	free(n);
}

