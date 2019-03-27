

typedef struct dictionary DICT;

extern DICT *newDictionary();
extern void *getDICTword(DICT *, void *);
extern int insertDICTword(DICT *, void *);
extern int deleteDICTword(DICT *, void *);
extern void loadDICT(DICT *, FILE *);
extern void loadDICTprefixes(DICT *, FILE *);
extern int sizeDICT(DICT *);
