#include "bst.h"
#include "tnode.h"
#include "queue.h"
#include<stdlib.h>

static TNODE *navBST(BST *t, TNODE *root, TNODE *value);
static TNODE *searchBST(BST *t, TNODE *root, void *value);
static TNODE *max(TNODE *root);
static TNODE *min(TNODE *root);
static int isLeaf(TNODE *node);
static int whichChild(TNODE *leaf);
static int minh(TNODE *root);
static int maxh(TNODE *root);
static void postOrderFree(BST *t, TNODE *root);
static void postOrderDisplay(TNODE *root, FILE *fp);
static void preOrderDisplay(TNODE *root, FILE *fp);
static void inOrderDisplay(TNODE *root, FILE *fp);
static int isRoot(BST *t, TNODE *node);
static void printRoot(TNODE *root, FILE *fp);
static void printChild(TNODE *node, FILE *fp);
static void swapperBST(TNODE *, TNODE *);

struct bst{
	TNODE *root;	
	int size;
	int debug_level;
	int (*comparator)(void*,void*);
	void (*display)(void*,FILE*);
	void (*swapper)(TNODE*,TNODE*);
	void (*free)(void*);
};


extern BST *newBST(int (*c)(void *, void *)){
	BST *newtree = malloc(sizeof(BST)); 
	newtree->debug_level = 0;
	newtree->root = NULL;
	newtree->size = 0;
	newtree->comparator = c;
	newtree->display = NULL;
	newtree->swapper = swapperBST;
	newtree->free = NULL;
	return newtree;
}

extern void setBSTdisplay(BST *t, void (*d)(void *, FILE *)){
	t->display = d;
}

extern void setBSTswapper(BST *t, void (*s)(TNODE *, TNODE *)){
	t->swapper = s;
}

extern void setBSTfree(BST *t, void (*f)(void *)){
	t->free = f;
}

extern TNODE *getBSTroot(BST *t){
	return t->root;
}

extern void setBSTroot(BST *t, TNODE *replacement){
	t->root = replacement;
}

extern void setBSTsize(BST *t, int s){
	t->size = s;
}

extern TNODE *insertBST(BST *t, void *value){
	TNODE *n = newTNODE(value, NULL, NULL, NULL);
	setTNODEdisplay(n, t->display);
	setTNODEfree(n, t->free);
	t->root = navBST(t, getBSTroot(t), n);
	setTNODEparent(t->root, t->root); //causes infinite loop
	return n;
}

static TNODE *navBST(BST *t, TNODE *root, TNODE *n){
	if(!root){
		t->size++;
	       	return n; 
	}
	if(t->comparator(getTNODEvalue(root), getTNODEvalue(n)) == 0){
		//dunno if i need this bit
		t->free(getTNODEvalue(n));
		setTNODEdisplay(n, t->display);
		setTNODEvalue(n, getTNODEvalue(root));
		setTNODEleft(n, getTNODEleft(root));
		setTNODEparent(n, getTNODEparent(root));
		setTNODEright(n, getTNODEright(root));
		if(getTNODEleft(n)) setTNODEparent(getTNODEleft(n), n);
		if(getTNODEright(n)) setTNODEparent(getTNODEleft(n), n);
		setTNODEvalue(root, NULL);
		setTNODEfree(root, NULL);
		freeTNODE(root);
		return n;
	}
	else if(t->comparator(getTNODEvalue(root), getTNODEvalue(n)) > 0){ // root greater 
		TNODE *lchild = navBST(t, getTNODEleft(root), n);	
		setTNODEleft(root, lchild);
		setTNODEparent(lchild, root);
	}
	else if(t->comparator(getTNODEvalue(root), getTNODEvalue(n)) < 0){ //root lesser 
		TNODE *rchild = navBST(t, getTNODEright(root), n);
		setTNODEright(root, rchild);
		setTNODEparent(rchild, root); 
	}
	return root;
}

extern void *findBST(BST *t, void *key){
	TNODE *s = searchBST(t, getBSTroot(t), key);
	if(s) return getTNODEvalue(s);
	else return NULL;
}

extern TNODE *locateBST(BST *t, void *key){
	return searchBST(t, getBSTroot(t), key);
}

static TNODE *searchBST(BST *t, TNODE *root, void *value){
	int compare;
	if(!root || (compare = t->comparator(getTNODEvalue(root), value)) == 0) return root;
	if(compare > 0) return searchBST(t, getTNODEleft(root), value);
	else return searchBST(t, getTNODEright(root), value);
}

extern int deleteBST(BST *t, void *key){
	TNODE *del = locateBST(t, key);
	if(!del) return -1;
	pruneLeafBST(t, (del = swapToLeafBST(t, del)));
	t->size--;
	setTNODEfree(del, NULL);
	freeTNODE(del);
	return 1;
}

extern TNODE *swapToLeafBST(BST *t, TNODE *node){
	TNODE *temp = node;
	while(isLeaf(temp) == 0){
		if(getTNODEleft(temp)) temp = max(getTNODEleft(temp));	//predecessor
		else temp = min(getTNODEright(temp));			//successor
		t->swapper(node, temp);
		node = temp;
	}
	return temp;
}

static TNODE *max(TNODE *root){
	if(!getTNODEright(root)) return root; 
	return max(getTNODEright(root));
}
static TNODE *min(TNODE *root){
	if(!getTNODEleft(root)) return root;
	return min(getTNODEleft(root));
}
static int isLeaf(TNODE *node){
	if(getTNODEleft(node) || getTNODEright(node)) return 0;
	else return 1;
}
static int whichChild(TNODE *leaf){
	TNODE *p = getTNODEparent(leaf);
	if(getTNODEleft(p)){
		if(getTNODEleft(p) == leaf){
			return -1;		//left child
		}
	}
	return 1;				//right child
}

extern void pruneLeafBST(BST *t, TNODE *leaf){
	TNODE *p = getTNODEparent(leaf);
	if(getBSTroot(t) == leaf) setBSTroot(t, NULL);
	if(whichChild(leaf) < 0) setTNODEleft(p, NULL);
	else setTNODEright(p, NULL);
	setTNODEparent(leaf, NULL);
}

extern int sizeBST(BST *t){
	return t->size;
}

static int minh(TNODE *root){
	if (!root) return -1;
	//if (!getTNODEleft(root)) return minh(getTNODEright(root));
	//if (!getTNODEright(root)) return minh(getTNODEleft(root));
	int r = minh(getTNODEright(root)) + 1;
	int l = minh(getTNODEleft(root)) + 1;
	if (l <= r) return l;
	else return r; 
}

static int maxh(TNODE *root){
	if (!root) return -1;
	int left = maxh(getTNODEleft(root));
	int right = maxh(getTNODEright(root));
	if (left > right) return left + 1;
	else return right + 1; 
}

extern void statisticsBST(BST *t, FILE *fp){
	fprintf(fp, "Nodes: %d\n", sizeBST(t));
	fprintf(fp, "Minimum depth: %d\n", minh(getBSTroot(t)));
	fprintf(fp, "Maximum depth: %d\n", maxh(getBSTroot(t)));
}

extern void displayBST(BST *t, FILE *fp){
	if(t->debug_level == 0){
		TNODE *current;
		QUEUE *q1 = newQUEUE(); QUEUE *q2 = newQUEUE();
		enqueue(q1, t->root);
		int level = 0;
		while (sizeQUEUE(q1) != 0 || sizeQUEUE(q2) != 0){
			fprintf(fp,"%d:", level);
			while(sizeQUEUE(q1) != 0){
				if((current = dequeue(q1))){	
					if(isRoot(t, current) == 1) printRoot(current, fp);
					else printChild(current, fp);
					if(getTNODEleft(current)) enqueue(q2, getTNODEleft(current));
					if(getTNODEright(current)) enqueue(q2, getTNODEright(current));
				}
			}
			level++;
			if(sizeQUEUE(q2) != 0){
				fprintf(fp, "\n");
				fprintf(fp,"%d:", level);
			}
			while(sizeQUEUE(q2) != 0){
				current = dequeue(q2);	
				printChild(current, fp);
				if(getTNODEleft(current)) enqueue(q1, getTNODEleft(current));
				if(getTNODEright(current)) enqueue(q1, getTNODEright(current));
			}
			fprintf(fp, "\n");
			level++;
		}
		freeQUEUE(q1);
		freeQUEUE(q2);
		return;
	}
	if (t->debug_level == 1){
		if(sizeBST(t) == 0) fprintf(fp, "[]");
		inOrderDisplay(getBSTroot(t), fp);
		return;
	}
	if (t->debug_level == 2){
		if(sizeBST(t) == 0) fprintf(fp, "[]");
		preOrderDisplay(getBSTroot(t), fp);
		return;
	}
	if (t->debug_level == 3){
		if(sizeBST(t) == 0) fprintf(fp, "[]");
		postOrderDisplay(getBSTroot(t), fp);
		return;
	}
}
static void preOrderDisplay(TNODE *root, FILE *fp){
	if (!root){
	        return;
	}	
	fprintf(fp, "[");
	displayTNODE(root, fp);
	if(getTNODEleft(root))fprintf(fp, " ");
	preOrderDisplay(getTNODEleft(root), fp);
	if(getTNODEright(root))fprintf(fp, " ");
	preOrderDisplay(getTNODEright(root), fp);
	fprintf(fp, "]");
}

static void inOrderDisplay(TNODE *root, FILE *fp){
	if (!root){
	        return;
	}	
	fprintf(fp, "[");
	inOrderDisplay(getTNODEleft(root), fp);
	if(getTNODEleft(root))fprintf(fp, " ");
	displayTNODE(root, fp);
	if(getTNODEright(root))fprintf(fp, " ");
	inOrderDisplay(getTNODEright(root), fp);
	fprintf(fp, "]");
}

static void postOrderDisplay(TNODE *root, FILE *fp){
	if (!root){
	        return;
	}	
	fprintf(fp, "[");
	postOrderDisplay(getTNODEleft(root), fp);
	if(getTNODEleft(root))fprintf(fp, " ");
	postOrderDisplay(getTNODEright(root), fp);
	if(getTNODEright(root))fprintf(fp, " ");
	displayTNODE(root, fp);
	fprintf(fp, "]");
}

extern int debugBST(BST *t, int level){
	int old = t->debug_level;
	t->debug_level = level;
	return old;
}
extern void freeBST(BST *t){
	postOrderFree(t, getBSTroot(t));
	free(t);
}

static void postOrderFree(BST *t, TNODE *root){
	if(!root) return;
	postOrderFree(t, getTNODEleft(root));
	postOrderFree(t, getTNODEright(root));
	freeTNODE(root);
}

static int isRoot(BST *t, TNODE *node){
	if(node == getBSTroot(t)) return 1;
	else return 0;
}

static void printRoot(TNODE *root, FILE *fp){
	fprintf(fp, " ");
	if(isLeaf(root) == 1) fprintf(fp, "=");
	displayTNODE(root, fp);
	fprintf(fp, "(");
	displayTNODE(getTNODEparent(root), fp);
	fprintf(fp, ")");
	fprintf(fp, "X");
}

static void printChild(TNODE *node, FILE *fp){
	fprintf(fp, " ");
	if(isLeaf(node) == 1) fprintf(fp, "=");
	displayTNODE(node, fp);
	fprintf(fp, "(");
	displayTNODE(getTNODEparent(node), fp);
	fprintf(fp, ")");
	if(whichChild(node) < 0) fprintf(fp, "L");
	else fprintf(fp, "R");
}

static void swapperBST(TNODE *a, TNODE *b){
	void *temp = getTNODEvalue(a);
	setTNODEvalue(a, getTNODEvalue(b));
	setTNODEvalue(b, temp);
}
