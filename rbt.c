#include "gst.h"
#include "rbt.h"
#include <stdlib.h>

typedef struct rnode{
	void *value;
	int color;
	RBT *self;
}RNODE;

struct rbt{
	GST *tree;
	int debug_level;
	void (*display) (void *, FILE *);
	void (*swapper) (TNODE *, TNODE *);
	void (*free) (void *);
	int (*comparator)(void *, void *);
};

static RNODE *newRNODE(RBT *t, void *value);
static void setRNODEtree(RNODE *, RBT *);
static int compareRNODE(void *, void *);
static void setRNODEtree(RNODE *r, RBT *t);
static int compareRNODE(void *a, void *b);
static void freeRNODE(void *r);
static void displayRNODE(void *r, FILE *fp);
static int color(TNODE *);
static TNODE *uncle(TNODE *);
static int isRoot(RBT *t, TNODE *);
static int whichChild(TNODE *);
static TNODE *grandParent(TNODE *n);
static void insFixUp(RBT *t, TNODE *n);
static void colorBlack(TNODE *);
static void colorRed(TNODE *);
static int isLinear(TNODE *);
static void leftRotate(RBT *t, TNODE *);
static void rightRotate(RBT *t, TNODE *);
static void rotate(RBT *t, TNODE *);
static void swapperRBT(TNODE *, TNODE *);
static void deletionFixUp(RBT *t, TNODE *n);
static TNODE *nephew(TNODE *n);
static TNODE *niece(TNODE *n);
static TNODE *sibling(TNODE *n);
static void matchColor(TNODE *c, TNODE *p);

extern RBT *newRBT(int (*c)(void *, void *)){
	RBT *newtree = malloc(sizeof(RBT));
	newtree->tree = newGST(compareRNODE);
	setGSTdisplay(newtree->tree, displayRNODE);
	setGSTfree(newtree->tree, freeRNODE);
	setGSTswapper(newtree->tree, swapperRBT);
	newtree->comparator = c;
	newtree->swapper = NULL;
	newtree->free = NULL;
	newtree->display = NULL;
	newtree->debug_level = 0;
	return newtree;
}

static RNODE *newRNODE(RBT *t, void *value){
	RNODE *r = malloc(sizeof(RNODE));
	r->value = value;
	r->color = 1;	//red	
	r->self = t;
	return r;
}

extern void setRBTdisplay(RBT *t, void (*d)(void *, FILE *)){
	t->display = d;
}

extern void setRBTswapper(RBT *t, void (*s)(TNODE *, TNODE*)){
	t->swapper = s;
}

extern void setRBTfree(RBT *t, void (*f)(void *)){
	t->free = f;
}

extern TNODE *getRBTroot(RBT *t){
	return getGSTroot(t->tree);
}

extern void setRBTroot(RBT *t, TNODE *replacement){
	setGSTroot(t->tree, replacement);	
}

extern void setRBTsize(RBT *t, int s){
	setGSTsize(t->tree, s);
}
extern int sizeRBT(RBT *t){
	return sizeGST(t->tree);
}
extern TNODE *insertRBT(RBT *t, void *value){
	RNODE *r = newRNODE(t, value);	
	TNODE *n = insertGST(t->tree, r);
	if(n){
		insFixUp(t, n);
	}
	return n;
}

extern void *findRBT(RBT *t, void *key){
	RNODE *p = newRNODE(t, key);
	RNODE *f = findGST(t->tree, p);
	setRNODEtree(p, NULL);
	freeRNODE(p);
	if(f) return f->value;
	else return NULL;
}

extern TNODE *locateRBT(RBT *t, void *key){
	RNODE *p = newRNODE(t, key);
	TNODE *l = locateGST(t->tree, p);
	setRNODEtree(p, NULL);
	freeRNODE(p);
	return l;
}

extern int deleteRBT(RBT *t, void *key){
	if(freqRBT(t, key) > 1){
		RNODE *p = newRNODE(t, key);
		int f = deleteGST(t->tree, p);
		setRNODEtree(p, NULL);
		freeRNODE(p);
	       	return f;
	}
	TNODE *l = locateRBT(t, key);
	if(!l) return -1;
	l = swapToLeafRBT(t, l);
	deletionFixUp(t, l);
	if(sizeRBT(t) > 1) pruneLeafRBT(t, l);	
	else setRBTroot(t, NULL);
	setRBTsize(t, sizeRBT(t)-1);
	setTNODEfree(l, NULL);
	freeTNODE(l);
	return 1;
}

extern TNODE *swapToLeafRBT(RBT *t, TNODE *node){
	return swapToLeafGST(t->tree, node);
}

extern void pruneLeafRBT(RBT *t, TNODE *leaf){
	setRNODEtree(unwrapGST(leaf), NULL);
	freeRNODE(unwrapGST(leaf));
	pruneLeafGST(t->tree, leaf);
}

extern void statisticsRBT(RBT *t, FILE *fp){
	statisticsGST(t->tree, fp);
}

extern int freqRBT(RBT *t, void *key){
	RNODE *p = newRNODE(t, key);
       	int f = freqGST(t->tree, p);
	setRNODEtree(p, NULL);
	freeRNODE(p);
	return f;
}

extern int duplicatesRBT(RBT *t){
	return duplicatesGST(t->tree);
}
static void deletionFixUp(RBT *t, TNODE *n){
	while(n){
		if(isRoot(t, n)) break;
		if(color(n) == 1) break;
		if(color(sibling(n)) == 1){
			colorRed(getTNODEparent(n));
			colorBlack(sibling(n));
			rotate(t, sibling(n));
		}
		else if(color(nephew(n)) == 1){
			matchColor(sibling(n), getTNODEparent(n));
			colorBlack(getTNODEparent(n));
			colorBlack(nephew(n));
			rotate(t, sibling(n));
			break;
		}
		else if(color(niece(n)) == 1){
			colorBlack(niece(n));
			colorRed(sibling(n));
			rotate(t, niece(n));
		}
		else{
			colorRed(sibling(n));
			n = getTNODEparent(n);
		}
	}
	colorBlack(n);
}

static void insFixUp(RBT *t, TNODE *n){
	while (color(getTNODEparent(n)) == 1){
		TNODE *u = uncle(n);
		TNODE *p = getTNODEparent(n);
		TNODE *g = grandParent(n);
		if(isRoot(t, n)) break;
		if(color(p) == 0) break;
		if(color(u) == 1){
			colorBlack(p);	
			colorBlack(u);
			colorRed(g);
			n = g; 
		}
		else{		//uncle must be black
			if(isLinear(n) == 0){	//make child and parent linear
			rotate(t, n);   //make parent child of child
			n = p;
			}
		colorBlack(getTNODEparent(n));
		colorRed(grandParent(n));
		rotate(t, getTNODEparent(n)); //make grandparent child of parent	
		break;
		}
	}
	colorBlack(getRBTroot(t));
}

static void rotate(RBT *t, TNODE *n){
	if(whichChild(n) > 0) leftRotate(t, getTNODEparent(n)); 
	else rightRotate(t, getTNODEparent(n));
}

static void leftRotate(RBT *t, TNODE *x){
	/** assume x->right != NULL
	 ** and the root's parent == NULL
	 **/
	TNODE *y = getTNODEright(x);
	setTNODEright(x, getTNODEleft(y));
	if(getTNODEleft(y)) setTNODEparent(getTNODEleft(y), x);
	setTNODEparent(y, getTNODEparent(x));
	if(getTNODEparent(x) == x){
	       	setRBTroot(t, y);
		setTNODEparent(y, y);
	}
	else if(whichChild(x) < 0) setTNODEleft(getTNODEparent(x), y); 
	else setTNODEright(getTNODEparent(x), y);
	setTNODEleft(y, x);
	setTNODEparent(x, y);
}
static void rightRotate(RBT *t, TNODE *x){
	TNODE *y = getTNODEleft(x);
	setTNODEleft(x, getTNODEright(y));
	if(getTNODEright(y)) setTNODEparent(getTNODEright(y), x);
	setTNODEparent(y, getTNODEparent(x));
	if(getTNODEparent(x) == x){
	       	setRBTroot(t, y);
		setTNODEparent(y, y);
	}
	else if(whichChild(x) < 0) setTNODEleft(getTNODEparent(x), y); 
	else setTNODEright(getTNODEparent(x), y);
	setTNODEright(y, x);
	setTNODEparent(x, y);
}

extern void displayRBT(RBT *t, FILE *fp){
	displayGST(t->tree, fp);
}

extern int debugRBT(RBT *t, int level){
	return debugGST(t->tree, level);
}

extern void freeRBT(RBT *t){
	freeGST(t->tree);
	free(t);
}

static int color(TNODE *n){
	if(!n) return 0;
	RNODE *r = unwrapGST(n);
	return r->color;
}

static void matchColor(TNODE *c, TNODE *p){
	if(color(p) == 1) colorRed(c);
	else colorBlack(c);
}

static TNODE *uncle(TNODE *n){
	return sibling(getTNODEparent(n));
}

static TNODE *nephew(TNODE *n){
	TNODE *s = sibling(n);
	if(whichChild(n) < 0) return getTNODEright(s);
	if(whichChild(n) > 0) return getTNODEleft(s);
	return NULL;
}

static TNODE *niece(TNODE *n){
	TNODE *s = sibling(n);
	if(whichChild(n) > 0) return getTNODEright(s);
	if(whichChild(n) < 0) return getTNODEleft(s);
	return NULL;
}

static TNODE *sibling(TNODE *n){
	if(whichChild(n) < 0){
	       	return getTNODEright(getTNODEparent(n));
	}
	if(whichChild(n) > 0) return getTNODEleft(getTNODEparent(n));
	return NULL;
}

static int isRoot(RBT *t, TNODE *n){
	if(getRBTroot(t) == n) return 1;
	else return 0;
}

static TNODE *grandParent(TNODE *n){
	return getTNODEparent(getTNODEparent(n));
}

static int whichChild(TNODE *leaf){
	TNODE *p = getTNODEparent(leaf);
	if(getTNODEleft(p)){
		if(getTNODEleft(p) == leaf){
			return -1;		//left child
		}
	}
	if(getTNODEright(p)){
		if(getTNODEright(p) == leaf)
			return 1;
	}
	return 0;
}
static int isLinear(TNODE *n){
	if(whichChild(getTNODEparent(n)) < 0 && whichChild(n) < 0)  
		return 1;
	else if (whichChild(getTNODEparent(n)) > 0 && whichChild(n) > 0)
		return 1;
	return 0;
}
static void colorBlack(TNODE *n){
	RNODE *r = unwrapGST(n);
	r->color = 0;
}
static void colorRed(TNODE *n){
	RNODE *r = unwrapGST(n);
	r->color = 1;
}
static void setRNODEtree(RNODE *r, RBT *t){
	r->self = t;
}

static int compareRNODE(void *a, void *b){
	RNODE *A = a;
	RNODE *B = b;
	return A->self->comparator(A->value, B->value);
}

static void freeRNODE(void *r){
	RNODE *A = r; 
	if(A->self){
		if(A->self->free)
			A->self->free(A->value);
	}
	free(A);
}

static void displayRNODE(void *r, FILE *fp){
	RNODE *a = r;
	a->self->display(a->value, fp);
	if(a->color == 1) fprintf(fp, "*");
}

static void swapperRBT(TNODE *a, TNODE *b){
	void *va = getTNODEvalue(a);   //get the GST value
	void *vb = getTNODEvalue(b);   //get the GST value
	// swap the GST values
	setTNODEvalue(a,vb);
	setTNODEvalue(b,va);
	// the above swap swapped the colors,
	// but the colors should stay with the nodes,
	// so swap the colors back to the original nodes
	RNODE *x = unwrapGST(a);
	RNODE *y = unwrapGST(b);
	int color = x->color;
	x->color = y->color;
	y->color = color;
}

extern void *unwrapRBT(TNODE *n){
	RNODE *r = unwrapGST(n);
	return r->value;
}
















