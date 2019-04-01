#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"
#include "fileio.h"


struct player{
	char *name;
	DA *match_record;
	int wins;
	int losses;
	char *location;
};

void read_log(PLAYR *p);

extern PLAYR *newPLAYR(char *n, char *log){
	PLAYR *p = (PLAYR*) malloc(sizeof(PLAYR));
	p->location = log;
	p->match_record = newDA();	
	p->name = n;
	p->wins = 0;
	p->losses = 0;
	read_log(p);
	return p;
}

extern char *getPLAYRname(PLAYR *p){
	return p->name;
}

extern int getPLAYRwins(PLAYR *p){
	return p->wins;
}

extern int getPLAYRlosses(PLAYR *p){
	return p->losses;
}

extern void setPLAYRwins(PLAYR *p, int w){
	p->wins = w;
}

extern void incrementPLAYRwins(PLAYR *p){
	p->wins++;
}

extern void incrementPLAYRlosses(PLAYR *p){
	p->losses++;
}

extern void setPLAYRlosses(PLAYR *p, int l){
	p->losses = l;
}

extern void freePLAYR(void *p){
	PLAYR *a = (PLAYR*)p;
	if(a->name) free(a->name);
	free(a);
}

extern void printPLAYR(void *player, FILE *where){
	PLAYR *p = (PLAYR*)player;
	fprintf(where, "Name: %s | Wins: %d | Losses: %d\n", p->name, p->wins, p->losses);
	for(int i = 0; i < sizeDA(p->match_record); i++){
		printPLAYR(getDA(p->match_record, i), where);
	}
}

extern void updatePLAYRs(PLAYR *p1, PLAYR *p2, int p1score, int p2score){
	if(p1score > p2score){
		incrementPLAYRwins(p1);
		incrementPLAYRlosses(p2);
		int found = 0;
		PLAYR *p = NULL;
		for(int i = 0; i < sizeDA(p1->match_record); i++){
			p = (PLAYR*)getDA(p1->match_record, i);
			if(strcmp(getPLAYRname(p), getPLAYRname(p2)) == 0){
				found = 1;		
				break;
			}
		}
		if(found == 0){
			p = newPLAYR(getPLAYRname(p2), NULL);
			insertDAback(p1->match_record, p);
		}
		incrementPLAYRwins(p);
		found = 0;
		for(int i = 0; i < sizeDA(p2->match_record); i++){
			p = (PLAYR*)getDA(p2->match_record, i);
			if(strcmp(getPLAYRname(p), getPLAYRname(p1)) == 0){
				found = 1;		
				break;
			}
		}
		if(found == 0){
			p = newPLAYR(getPLAYRname(p1), NULL);
			insertDAback(p2->match_record, p);
		}
		incrementPLAYRlosses(p);
	}
	else{
		incrementPLAYRwins(p2);
		incrementPLAYRlosses(p1);
		int found = 0;
		PLAYR *p = NULL;
		for(int i = 0; i < sizeDA(p1->match_record); i++){
			p = (PLAYR*)getDA(p1->match_record, i);
			if(strcmp(getPLAYRname(p), getPLAYRname(p2)) == 0){
				found = 1;		
				break;
			}
		}
		if(found == 0){
			p = newPLAYR(getPLAYRname(p2), NULL);
			insertDAback(p1->match_record, p);
		}
		incrementPLAYRlosses(p);
		found = 0;
		for(int i = 0; i < sizeDA(p2->match_record); i++){
			p = (PLAYR*)getDA(p2->match_record, i);
			if(strcmp(getPLAYRname(p), getPLAYRname(p1)) == 0){
				found = 1;		
				break;
			}
		}
		if(found == 0){
			p = newPLAYR(getPLAYRname(p2), NULL);
			insertDAback(p2->match_record, p);
		}
		incrementPLAYRwins(p);
	}
	
}

extern void writePLAYRlog(PLAYR *p){
	FILE *fp = fopen(p->location, "w");	
	if(fp){
		fprintf(fp,"%d\n", p->wins);
		fprintf(fp,"%d\n", p->losses);
		PLAYR *p2 = NULL;
		for(int i = 0; i < sizeDA(p->match_record); i++){
			p2 = (PLAYR*)getDA(p->match_record, i);
			fprintf(fp,"%s\n", p2->name);
			fprintf(fp,"%d\n", p2->wins);
			if((i+1) == sizeDA(p->match_record))
				fprintf(fp, "%d", p2->losses);
			else
				fprintf(fp,"%d\n", p2->losses);
		}
		fclose(fp);
	}
}

void read_log(PLAYR *p){
	FILE *fp = fopen(p->location, "r");
	char *c;
	if(fp){
		p->wins = atoi(getRawToken(fp));
		p->losses = atoi(getRawToken(fp));
			
		while((c = getRawToken(fp))){
			PLAYR *e = newPLAYR(c, NULL);
			setPLAYRwins(e, atoi(getRawToken(fp)));
			setPLAYRlosses(e, atoi(getRawToken(fp)));
			insertDAback(p->match_record, e);
		}
		fclose(fp);
	}
	else
	       return;	
}

extern DA *getPLAYRlog(PLAYR *p){
	return p->match_record;
}
