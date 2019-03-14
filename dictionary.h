

typedef struct dictionary DICT;

extern DICT *newDictionary(FILE *);
extern void *getDICTword(DICT *, void *);
extern int insertDICTword(DICT *, void *);
extern int deleteDICTword(DICT *, void *);
extern int sizeDICT(DICT *);
