
typedef struct Trie Trie;

extern Trie *getNewTrieNode();
extern void insert(struct Trie * *, char *);
extern int haveChildren(struct Trie *);
extern int search(struct Trie *, char *str);
extern int searchPrefix(struct Trie *, char *str);
extern int deletion(struct Trie * *, char *str);
