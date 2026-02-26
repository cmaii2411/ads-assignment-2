#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "bit.h"

// (SOURCE: COMP20003 WORKSHOP 3.4) data definitions, internal within list.checks >>>>>>>>>>>>>>>

// functions >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// (SOURCE: COMP20003 WORKSHOP 3.4) creates & returns an empty linked list
list_t *listCreate() {
	list_t *list = malloc(sizeof(*list));
	assert(list);
	list->head = list->tail = NULL;
	list->n = 0;
	return list;
}

// returns 1 if the list is empty, 0 otherwise
int listIsEmpty(list_t *list) {
	assert(list);
	return list->head == NULL;
}

// (SOURCE: COMP20003 WORKSHOP 3.4) free the list
void listFree(list_t *list, void (*dataFree)(void *)){
	assert(list != NULL);
	node_t *curr = list->head; // curr points to the first node
	while (curr) {             // while curr not reaching the end of list
		node_t *tmp = curr;
		curr = curr->next;     // advance curr to the next next node 
		dataFree(tmp->data);       // frees the previous node including
		free(tmp);             //    freeing the space used by data
	}
	free(list);                // free the list itself
} 

// (SOURCE: COMP20003 WORKSHOP 3.4) returns the number of elements in list
int listCount(list_t *list){
	assert(list);
	return list->n;
}

// inserts a new node with value "data" to the end of "list" 
void listAppend(list_t *list, void *data) {
	assert(list);

	// creates a new node and set data 
	node_t *new = malloc(sizeof(*new));
	assert(new);
	new->data = data;
	new->next = NULL;

	if (list->tail == NULL){ // append first node
		list->tail = new;
        list->head = new;
	} else {
		list->tail->next = new;
		list->tail = new;
	}
    list->n += 1;
}

// query search for address, return number of addresses found
int listSearch(list_t *list, char *key, int *bComps, int *nComps, int *strComps, char* (*dataGetKey)(void *)){
    int nFound = 0;
	node_t *curr = list->head;
	char *nodeKey;

    if (!curr) {
        return nFound;
    }

    while (curr) {
		nodeKey = dataGetKey(curr->data);
		int totalBits = strlen(nodeKey) * BITS_PER_BYTE;
		int match = 1;

		//compares each bits of each addressID to each of query 
		for (int i = 0; i < totalBits ; i++) {
			int node_bit = getBit(nodeKey, i);
			int query_bit = getBit(key, i);

			if (node_bit != query_bit) {
				(*bComps)++;
				match = 0;
				break;
			}
			(*bComps)++;
		}
		(*nComps)++;
		(*strComps)++;

		if (match) {
			(*bComps) += BITS_PER_BYTE;
			nFound++;
		}
		curr = curr->next;
    }
	return nFound;
}

// prints a address record to file "f"
void addressDetailPrint(void *file, void *query, list_t *addr, char* (*dataGetKey)(void *), void (*nodePrint)(void *, void *)) {
    FILE *f = (FILE *)file;
    char* s = (char*) query;
	node_t *curr = addr->head;
	
	if (!curr) {
        return;
    }
	
    while (curr){
		char *nodeKey = dataGetKey(curr->data); 
		if (strcmp(s, nodeKey) == 0) {
			nodePrint(f, curr->data);
		}
		curr = curr->next;
    }
} 

void addressListPrint(void *file, list_t *dict, void (*nodePrint)(void *, void *)) {
	FILE *f = (FILE *)file;
	node_t *curr = dict->head;

	if (!curr) {
        return;
    }
	
    while (curr){
		nodePrint(f, curr->data);
		curr = curr->next;
	}
}

int listLength(list_t *dict) {
	node_t *curr = dict->head;
	int length = 0;

	if (!curr) { return length; }

	while (curr) {
		length++;
		curr = curr->next;
	}

	return length;
}


/* ===============================================================================
Line 10 - 50 are code taken from workshop 3.4 list.c for linked list implementation 
================================================================================== */

