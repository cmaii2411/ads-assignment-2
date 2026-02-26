/* Skeleton inspired by COMP20003 Workshop 3.4 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "data.h"
#include "list.h"
#include "trie.h"

#define STAGE_ONE 1
#define STAGE_TWO 2

void stageOne(char *dataFileName, FILE *inFile, FILE *stdoutFile, FILE *outFile);
void stageTwo(char *dataFileName, FILE *inFile, FILE *stdoutFile, FILE *outFile);

int processArgs(int argc, char *argv[]);
list_t *getAddressList(char *filename);
trieNode_t *getAddressTree(char *filename);

int main(int argc, char *argv[]) {
	int stage = processArgs(argc, argv);
	FILE *outputFile = fopen(argv[3], "w");
	assert(outputFile);

	// runs respective stage/task specified
	switch (stage) {
		case STAGE_ONE:
			stageOne(argv[2], stdin, stdout, outputFile);
			break;
		case STAGE_TWO:
			stageTwo(argv[2], stdin, stdout, outputFile);
			break;
		default:
			fprintf(stderr, "Please specify stage\n");
			exit(EXIT_FAILURE);
	}
	fclose(outputFile);
	return 0;
} 

// returns the specified stage
int processArgs(int argc, char *argv[]) {
	if (argc != 4 ) {
		fprintf(stderr, "Invalid command line\n");
		exit(EXIT_FAILURE);
	}
	return atoi(argv[1]);
}

// (SRC: getAddressList's inspiration from workshop 3.4 driver.c getStudentsList) 
// reads addresses from "filename", 
// returns linked list of addresses
list_t *getAddressList(char *filename) {
	FILE *f = fopen(filename, "r");
	assert(f);
	list_t *addresses = listCreate();
	addressSkipHeaderLine(f);
	address_t *s;

	while (1) {
		s = addressRead(f);
		if (s == NULL) break;
		listAppend(addresses, s);
	}
	fclose(f);
	return addresses;
}

// querying with address id on linked list_t
void stageOne(char *dataFileName, FILE *inFile, FILE *stdoutFile, FILE *outFile) {

	// Builds linked list of addresses
	list_t *addresses = getAddressList(dataFileName);

	// Querying and print results to stdout
	char query[MAX_FIELD_LEN + 1];

	while (fgets(query, sizeof(query), inFile)) {
        int bComps = 0, nComps = 0, strComps = 0; 
		query[strcspn(query, "\n")] = 0; // triming the trailing new-line character                   
		int found = listSearch(addresses, query, &bComps, &nComps, &strComps, addressGetID);
		
		// print to stdout heading
		fprintf(stdoutFile, "%s --> ", query);

		// print dictionary to output file
		fprintf(outFile, "%s\n", query);
		addressDetailPrint(outFile, query, addresses, addressGetID, addressPrint);

		if (found) {
			fprintf(stdoutFile, "%d records found - comparisons: b%d n%d s%d\n", found, bComps, nComps, strComps);
		} else {
			fprintf(stdoutFile, "NOTFOUND\n");
		}
	}
	listFree(addresses, addressFree);
}

trieNode_t *getAddressTree(char *filename) {
	FILE *f = fopen(filename, "r");
	assert(f);
	trieNode_t *addressesTree = NULL;
	addressSkipHeaderLine(f);
	address_t *s;

	while (1) {
		s = addressRead(f);
		if (s == NULL) break;
		int bits = (strlen(addressGetID(s)) + 1) * BITS_PER_BYTE;
		addressesTree = patriciaTreeInsert(addressesTree, s, addressGetID(s), bits);
	}
	fclose(f);
	return addressesTree;
}

void stageTwo(char *dataFileName, FILE *inFile, FILE *stdoutFile, FILE *outFile) {
	trieNode_t *addresses = getAddressTree(dataFileName);
	char query[MAX_FIELD_LEN + 1];

	while (fgets(query, sizeof(query), inFile)) {
        comps_t comp = {0};
		query[strcspn(query, "\n")] = 0; // triming the trailing new-line character                   
		
		// print to stdout heading
		fprintf(stdoutFile, "%s --> ", query);

		// print dictionary to output file
		fprintf(outFile, "%s\n", query);
		int found = patriciaTreeSearchPrint(outFile, &comp, query, addresses, addressPrint, addressGetID);

		if (found) {
			fprintf(stdoutFile, "%d records found - comparisons: b%d n%d s%d\n", found, comp.bComps, comp.nComps, comp.strComps);
		} else {
			fprintf(stdoutFile, "NOTFOUND\n");
		}
	}
	patriciaTrieFree(addresses, addressFree);
}

