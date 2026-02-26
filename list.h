#ifndef _LIST_H_
#define _LIST_H_

#define MAX_FIELD_LEN 127
#define MAX_LINE_LEN 511

// type definitions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct list list_t;

typedef struct node node_t;
struct node {
	void *data;              // points to the data element of the node
	node_t *next;            // points to the next node in the list
};

struct list {
	node_t *head;  // points to the first node of the list
	node_t *tail;  // points to the last node of the list 
	size_t n;               // number of element in the list
};

// creates & returns an empty linked list
list_t *listCreate();

// free the list, with the help of function "freeData" for freeind data component
void listFree(list_t *list, void (*dataFree)(void *));

// returns 1 if the list is empty, 0 otherwise
int listIsEmpty(list_t *list); 

// returns the number of elements in list
int listCount(list_t *list);

// inserts a new node with value "data" to the end of "list" 
void listAppend(list_t *list, void *data); 

// print to a "f" file details of a an address (a list node data)
void addressDetailPrint(void *f, void *query, list_t *dict, char* (*dataGetKey)(void *), void (*nodePrint)(void *, void *));

// searches for the number of query and its difference
int listSearch(list_t *list, char *key, int *bComps, int *nComps, int *strComps, char* (*dataGetKey)(void *));

void addressListPrint(void *file, list_t *dict,void (*nodePrint)(void *, void *));

int listLength(list_t *dict);

#endif
