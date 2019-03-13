#include "tnode.h"
#include "gst.h"
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct gnode{
	void *value;
	int frequency;
	GST *self;
}GNODE;

struct gst {
	BST *tree;
	int duplicates;
	int debug_level;
	void (*display)(void *, FILE *);
	void (*swapper)(TNODE *, TNODE *);
	void (*free)(void *);
	int (*comparator)(void *, void *);
};

static GNODE *newGNODE(GST *t, void *value);
static int compareGNODE(void *a, void *b);
static void freeGNODE(void *a);
static void displayGNODE(void *n, FILE *fp);
static void incFreqGNODE(GNODE *g);
static void decFreqGNODE(GNODE *g);
static void incDupesGST(GST *t, GNODE *g);
static void decDupesGST(GST *t, GNODE *g);
static void setGNODEtree(GNODE *, GST *);

extern GST *newGST(int (*c)(void *, void *)){
	GST *newtree = malloc(sizeof(GST));
	newtree->duplicates = 0;
	newtree->debug_level = 0;
	newtree->tree = newBST(compareGNODE);
	setBSTfree(newtree->tree, freeGNODE);
	setBSTdisplay(newtree->tree, displayGNODE);
	newtree->comparator = c;
	newtree->display = NULL;
	newtree->free = NULL;
	newtree->swapper = NULL;
	return newtree;
}

static GNODE *newGNODE(GST *t, void *value){
	GNODE *g = malloc(sizeof(GNODE));
	g->value = value;
	g->frequency = 0;
	g->self = t;
	return g;
}

extern void setGSTdisplay(GST *t, void (*d)(void *, FILE *)){
	t->display = d;
}

extern void setGSTswapper(GST *t, void (*s)(TNODE *, TNODE *)){
	t->swapper = s;
	setBSTswapper(t->tree, s);
}

extern void setGSTfree(GST *t, void (*f)(void *)){
	t->free = f;
}

extern TNODE *getGSTroot(GST *t){
	return getBSTroot(t->tree);	
}

extern void setGSTroot(GST *t, TNODE *replacement){
	setBSTroot(t->tree, replacement); 
}

extern void setGSTsize(GST *t, int s){
	setBSTsize(t->tree, s);
}

extern TNODE *insertGST(GST *t, void *value){
	TNODE *tNode;
	GNODE *gNode;
	if((tNode = locateGST(t, value))){
		gNode = getTNODEvalue(tNode);	
		t->free(value);
		tNode = NULL;
	}
	else{
		gNode = newGNODE(t, value);
		tNode = insertBST(t->tree, gNode);
		gNode = getTNODEvalue(tNode);
	}
	incFreqGNODE(gNode);	
	incDupesGST(t, gNode);
	return tNode;
}

extern void *findGST(GST *t, void *key){
	GNODE *p = newGNODE(t, key);
	GNODE *f = findBST(t->tree, p);
	setGNODEtree(p, NULL);
	freeGNODE(p);
	if(f) return f->value;
	else return NULL;
}

extern TNODE *locateGST(GST *t, void *key){
	GNODE *p = newGNODE(t, key);
	TNODE *l = locateBST(t->tree, p);
	setGNODEtree(p, NULL);
	freeGNODE(p);
	return l;
}

extern int deleteGST(GST *t, void *key){
	TNODE *d = locateGST(t, key);
	if(!d) return -1;
	GNODE *p = getTNODEvalue(d);
	if(p->frequency > 1){
	       	decFreqGNODE(p);
		decDupesGST(t, p);
		return p->frequency;
	}
	d = swapToLeafGST(t, d);
	if (p->frequency == 1 && sizeGST(t) > 1){
		pruneLeafGST(t, d);	
	}
	else setGSTroot(t, NULL);
	setGSTsize(t, sizeGST(t)-1);
	setTNODEfree(d, NULL);
	freeTNODE(d);
	return 0;
}

extern TNODE *swapToLeafGST(GST *t, TNODE *node){
	return swapToLeafBST(t->tree, node);
}

extern void pruneLeafGST(GST *t, TNODE *leaf){
	pruneLeafBST(t->tree, leaf);
	setGNODEtree(getTNODEvalue(leaf), NULL);
	freeGNODE(getTNODEvalue(leaf));
}

extern int sizeGST(GST *t){
	return sizeBST(t->tree);
}

extern void statisticsGST(GST *t, FILE *fp){
	fprintf(fp, "Duplicates: %d\n", duplicatesGST(t));
	statisticsBST(t->tree, fp);
}

extern void displayGST(GST *t, FILE *fp){
	displayBST(t->tree, fp);
}

extern int debugGST(GST *t, int level){
	t->debug_level = level;
	return debugBST(t->tree, level);
}

extern void freeGST(GST *t){
	freeBST(t->tree);
	free(t);
}

extern void *unwrapGST(TNODE *n){
	GNODE *a = getTNODEvalue(n);
	return a->value;
}

extern int freqGST(GST *t, void *key){
	TNODE *l = locateGST(t, key);
	if(!l) return 0;
	GNODE *g = getTNODEvalue(l);
	return g->frequency;
}

extern int duplicatesGST(GST *g){
	return g->duplicates;
}

static int compareGNODE(void *a, void *b){
	GNODE *A = a;
	GNODE *B = b;
	return A->self->comparator(A->value, B->value);
}

static void freeGNODE(void *a){
	GNODE *A = (GNODE *)a;	
	if(A->self){
		if(A->self->free)
			A->self->free(A->value);
	}
	free(A);
}

static void displayGNODE(void *n, FILE *fp){
	GNODE *a = n;
	a->self->display(a->value, fp);
	if (a->frequency > 1) fprintf(fp, "<%d>", a->frequency);
}



static void incFreqGNODE(GNODE *g){
	g->frequency++;
}
static void decFreqGNODE(GNODE *g){
	g->frequency--;
}

static void incDupesGST(GST *t, GNODE *g){
	if(g->frequency > 1) t->duplicates++;
	return;
}

static void decDupesGST(GST *t, GNODE *g){
	if(g->frequency >= 1) t->duplicates--;
	return;
}

static void setGNODEtree(GNODE *g, GST *t){
	g->self = t;
}










