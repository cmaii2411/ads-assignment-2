#ifndef _TRIE_H_
#define _TRIE_H_
#include "bit.h"
#include "list.h"

#define MAX_DIST 99999

/* 1. nomatch 
1.1. theres a differing bit
1.2 the first one record is shorter
1.3 the second one is shorter
2. match */

typedef struct trieNode trieNode_t;

typedef struct comps comps_t;
struct comps {
    int bComps;
    int nComps;
    int strComps;
};

void patriciaTrieFree(trieNode_t *trie, void (*dataFree)(void *));
trieNode_t *patriciaTreeInsert(trieNode_t *root, void *data, char *keyNode, int keyBits);
int patriciaTreeSearchPrint(FILE *f, comps_t *comp, char *query, trieNode_t *root, void (*nodePrint)(void *, void *), char* (*dataGetKey)(void *));
//int patriciaTreeSearch(trieNode_t *root, char *query, comps_t *comp);
void collectDescendants(trieNode_t *node, list_t *candidates);
trieNode_t *selectClosestDist(char* query, list_t *candidates, trieNode_t* closest, char* (*dataGetKey)(void *), comps_t *comp);
trieNode_t *makeLeafNode(trieNode_t *newNode, char* prefix, int mismatch, int prefix_length, void *data);
int firstMismatchBit(char* str1, int str1_length, char *str2, int str2_length, int *bitComps);
//char *stringToBitString(char *str, int *outBits);
int editDistance(char *str1, char *str2, int n, int m);
int min(int a, int b, int c);
void freeCandidatesList(list_t *list);

#endif