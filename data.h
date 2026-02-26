#ifndef _DATA_H_
#define _DATA_H_
#include <stdio.h>

// constant defnitions
#define MAX_FIELD_LEN 127
#define MAX_LINE_LEN 511

// data definitions
typedef struct address address_t;

/*------------ function definitions -----------------------------*/

// skip the header line of .csv file "f"
void addressSkipHeaderLine(FILE *f);

// reads all the data for one address from file "f" to build address_t data
// returns the pointer, or NULL if reading is unsuccessful
address_t *addressRead(FILE *f);

// prints a address record *addr to outputfile "f"
void addressPrint(void *, void *);

// read in each field of the file and store in a struct member
int readString(FILE *F, char *attribute, char **addr_attr);

// free the memory allocated of "address"
void addressFree(void *addr);

// returns ezi_add of a address
char* addressGetID(void *s);

#endif
