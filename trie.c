#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "trie.h"
#include "bit.h"
#include "list.h"


// Trie to store the key of dictionary

typedef struct trieNode trieNode_t;

struct trieNode {
    int prefixBits;
    trieNode_t *branchA;
    trieNode_t *branchB;

    list_t *records;
    char* prefix;
};

trieNode_t *patriciaTreeInsert(trieNode_t *root, void *data, char *keyNode, int keyBits) {
    char *keyBitStr = malloc(sizeof(char) * strlen(keyNode) + 1);
    strcpy(keyBitStr, keyNode);
    int bitCompsCount;

    // Create a leaf, insert to empty tree
    if (root == NULL) {
        trieNode_t *node = malloc(sizeof(trieNode_t));
        assert(node);
        node->branchA = NULL;
        node->branchB = NULL;
        node->records = listCreate();
        listAppend(node->records, data);
        
        node->prefix = keyBitStr;
        node->prefixBits = keyBits;
        return node;
    }

    // Root is leaf
    if (root->branchA == NULL && root->branchB == NULL) {
        int mismatch = firstMismatchBit(root->prefix, root->prefixBits, keyBitStr, keyBits, &bitCompsCount);

        // if match perfectly
        if (mismatch == -1 && keyBits == root->prefixBits) {
            listAppend(root->records, data);
            free(keyBitStr);
            return root;
        }

        int commonPrefixLength;
        if (mismatch == -1) {
            // One string is prefix of another
            commonPrefixLength = (root->prefixBits < keyBits) ? root->prefixBits : keyBits;
        } else {
            // Strings differ at mismatch position
            commonPrefixLength = mismatch;
        }

        // Split node and create new branches
        // Create new root node
        trieNode_t *newRoot = malloc (sizeof(trieNode_t));
        assert(newRoot);
        newRoot->records = NULL;
        
        if (commonPrefixLength > 0) {
            newRoot->prefix = createStem(root->prefix, 0, commonPrefixLength);
            newRoot->prefixBits = commonPrefixLength;
        } else {
            // Special case: no common prefix at all
            newRoot->prefix = malloc(1);
            newRoot->prefix[0] = '\0';
            newRoot->prefixBits = 0;
        }

        trieNode_t *oldStem = malloc(sizeof(trieNode_t)); // Stem for old root mismatch
        trieNode_t *mismatchStem = malloc(sizeof(trieNode_t)); // Stem for new key mismatch
        assert(oldStem && mismatchStem);

        if (commonPrefixLength < keyBits) {
            makeLeafNode(mismatchStem, keyBitStr, commonPrefixLength, keyBits, data);
        } else {
            // New key is prefix of old key
            mismatchStem->branchA = mismatchStem->branchB = NULL;
            mismatchStem->prefix = malloc(1);
            mismatchStem->prefix[0] = '\0';
            mismatchStem->prefixBits = 0;
            mismatchStem->records = listCreate();
            listAppend(mismatchStem->records, data);
        }

        if (commonPrefixLength < root->prefixBits) {
            makeLeafNode(oldStem, root->prefix, commonPrefixLength, root->prefixBits, NULL);
            oldStem->records = root->records;
        } else {
            // Old key is exact prefix of new key
            oldStem->branchA = oldStem->branchB = NULL;
            oldStem->prefix = malloc(1); // Empty suffix
            oldStem->prefix[0] = '\0';
            oldStem->prefixBits = 0;
            oldStem->records = root->records;
        }

        free(root->prefix);
        
        // Split node at mismatch
        if (getBit(keyBitStr, commonPrefixLength) == 0) {
            newRoot->branchA = mismatchStem;
            newRoot->branchB = oldStem;
        } else {
            newRoot->branchA = oldStem;
            newRoot->branchB = mismatchStem;
        }

        free(keyBitStr);
        free(root);
        return newRoot;
    }

    // Root is not leaf -> traverse
    int bit = getBit(keyBitStr, root->prefixBits);
    // Change keyNode inserted to exclude prefix-matched part
    char *nextKey = createStem(keyNode,root->prefixBits, keyBits - root->prefixBits);
    if (bit == 0) {
        root->branchA = patriciaTreeInsert(root->branchA,data,nextKey, keyBits - root->prefixBits);
    } else {
        root->branchB = patriciaTreeInsert(root->branchB,data,nextKey, keyBits - root->prefixBits);
    }
    free(nextKey);
    free(keyBitStr);
    return root;
}

int patriciaTreeSearchPrint(FILE *f, comps_t *comp, char *query, trieNode_t *root, void (*nodePrint)(void *, void *), char* (*dataGetKey)(void *)){
    if (root == NULL) {
        return 0;
    }

    // If node is a leaf
    trieNode_t *curr = root;
    int queryBitLength = (strlen(query) + 1) * BITS_PER_BYTE;
    int currentQueryBit = 0;
    comp->strComps +=1;

    while (curr){
        comp->nComps ++;

        // Create query substring from current position
        char *querySubstring = createStem(query, currentQueryBit, queryBitLength - currentQueryBit);

        int mismatch = firstMismatchBit(querySubstring, queryBitLength - currentQueryBit, curr->prefix, curr->prefixBits, &comp->bComps);
        
        // Prefix matchs
        if (mismatch == -1) {    
            // Case 1: 
            // Reach the leaf node
            if (curr->branchA == NULL && curr->branchB == NULL) {
                addressListPrint(f,curr->records,nodePrint);
                free(querySubstring);

                if (curr->records == NULL) {
                    return 0;
                }           
            
                return listLength(curr->records);
            }

            // Case 2: Query exhausted -> collect descendants -> choose Closest Distance
            if (queryBitLength <= curr->prefixBits) {                
                list_t *candidates = listCreate();
                collectDescendants(curr, candidates);

                trieNode_t *closestLeaf = NULL;
                closestLeaf = selectClosestDist(query, candidates, closestLeaf, dataGetKey, comp);
                
                freeCandidatesList(candidates);
                addressListPrint(f, closestLeaf->records, nodePrint);
                free(querySubstring);

                if (curr->records == NULL) {
                    return 0;
                }
                return listLength(closestLeaf->records);
            }

            // Traverse to next node
            currentQueryBit += curr->prefixBits;
            int nextBit = getBit(query, currentQueryBit);
            curr = (nextBit == 0) ? (curr->branchA) : (curr->branchB);
            free(querySubstring);
            continue;
        } 

        // Mismatch: check fallback
        list_t *candidates = listCreate();
        collectDescendants(curr, candidates);

        trieNode_t *closestLeaf = NULL;
        closestLeaf = selectClosestDist(query, candidates, closestLeaf, dataGetKey, comp);
        freeCandidatesList(candidates);
        
        addressListPrint(f, closestLeaf->records, nodePrint);
        free(querySubstring);
        
        return listLength(closestLeaf->records);
    }
    return 0;
}

trieNode_t *selectClosestDist(char* query, list_t *candidates, trieNode_t* closest, char* (*dataGetKey)(void *), comps_t *comp) {
    node_t *node = candidates->head;
    int bestDist = MAX_DIST;

    while (node) {
        trieNode_t *leaf = (trieNode_t *) node->data;
        
        if (leaf && leaf->records) {
            if (leaf->records->head) {
                char *candidateKey = dataGetKey(leaf->records->head->data);
                int dist = editDistance(query, candidateKey, strlen(query), strlen(candidateKey));

                if (dist < bestDist) {
                    bestDist = dist;
                    closest = leaf;
                } 

                else if (dist == bestDist && closest != NULL) {
                    char *currentBest = dataGetKey(closest->records->head->data);
                    if (strcmp(candidateKey, currentBest) < 0) {
                        closest = leaf;
                    }
                }
            } 
        }
        node = node->next;
    }
    return closest;
}

void patriciaTrieFree(trieNode_t *trie, void (*dataFree)(void *)){
    if (!trie) return;

    if (trie->records != NULL) {
        listFree(trie->records, dataFree);
    }

    patriciaTrieFree(trie->branchA, dataFree);  
    patriciaTrieFree(trie->branchB, dataFree); 
    
    if (trie->prefix) {
        free(trie->prefix);
    }
    
    free(trie);
}

void freeCandidatesList(list_t *list) {
    if (!list) return;

    node_t *curr = list->head;
    while (curr) {
        node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

trieNode_t *makeLeafNode(trieNode_t *newNode, char* prefix, int startBit, int endBit, void *data) {
    newNode->branchA = newNode->branchB = NULL;
    newNode->prefix = createStem(prefix, startBit, endBit - startBit);

    newNode->prefixBits = endBit - startBit;
    // Create new linked list to store new key's data
    if (data) {
        newNode->records = listCreate();
        listAppend(newNode->records, data);
    }
    return newNode;
}

int firstMismatchBit(char* str1, int str1_length, char *str2, int str2_length, int *count) {
        int min_length = (str1_length <= str2_length) ? str1_length : str2_length;

        for (int i = 0; i < min_length; i++) {
            int bit1 = getBit(str1, i);
            int bit2 = getBit(str2, i);

            (*count)++;
            if (bit1 != bit2) {
                return i;
            }
        }
        return -1;
}

void collectDescendants(trieNode_t *node, list_t *candidates) {
    if (!node) return;

    if (node->branchA == NULL && node->branchB == NULL) {
        // Leaf: copy all leaf nodes
        listAppend(candidates, node);
    } else {
        // Internal node → keep going
        collectDescendants(node->branchA, candidates);
        collectDescendants(node->branchB, candidates);
    }
}

/* Returns min of 3 integers 
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int min(int a, int b, int c) {
    if (a < b) {
        if(a < c) {
            return a;
        } else {
            return c;
        }
    } else {
        if(b < c) {
            return b;
        } else {
            return c;
        }
    }
}

/* Returns the edit distance of two strings
    reference: https://www.geeksforgeeks.org/edit-distance-in-c/ */
int editDistance(char *str1, char *str2, int n, int m){
    assert(m >= 0 && n >= 0 && (str1 || m == 0) && (str2 || n == 0));
    // Declare a 2D array to store the dynamic programming
    // table
    int dp[n + 1][m + 1];

    // Initialize the dp table
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            // If the first string is empty, the only option
            // is to insert all characters of the second
            // string
            if (i == 0) {
                dp[i][j] = j;
            }
            // If the second string is empty, the only
            // option is to remove all characters of the
            // first string
            else if (j == 0) {
                dp[i][j] = i;
            }
            // If the last characters are the same, no
            // modification is necessary to the string.
            else if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = min(1 + dp[i - 1][j], 1 + dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
            // If the last characters are different,
            // consider all three operations and find the
            // minimum
            else {
                dp[i][j] = 1 + min(dp[i - 1][j], dp[i][j - 1],
                    dp[i - 1][j - 1]);
            }
        }
    }

    // Return the result from the dynamic programming table
    return dp[n][m];
}