

typedef struct dictionary DICT;

extern DICT *newDictionary();
extern int getDICTword(DICT *, void *);
extern int getDICTprefix(DICT *, void *);
extern void insertDICTword(DICT *, void *);
extern int deleteDICTword(DICT *, void *);
extern void loadDICT(DICT *, FILE *);
extern void loadDICTprefixes(DICT *, FILE *);
//extern int sizeDICT(DICT *);
