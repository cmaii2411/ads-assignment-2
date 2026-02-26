/* Module contains partial codes inspired from Workshop 3.4 */

/* ------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"

//struct address filled with all data for addresses
struct address {
	char *pfi;
	char *ezi_add;     
    char *src_verif;
	char *propstatus;
	char *gcodefeat;
	char *loc_desc;
    char *blgunttyp;
    char *hsaunitid;
    char *bunit_pre1;
    char *bunit_id1;
    char *bunit_suf1;
    char *bunit_pre2;
    char *bunit_id2;
    char *bunit_suf2;
    char *floor_type;
    char *floor_no_1;
    char *floor_no_2;
    char *building;
    char *complex;
    char *hse_pref1;
    char *hse_num1;
    char *hse_suf1;
    char *hse_pref2;
    char *hse_num2;
    char *hse_suf2;
    char *disp_num1;
    char *road_name;
    char *road_type;
    char *rd_suf;
    char *locality;
    char *state;
    char *postcode;
    char *accestype;
    long double x;
    long double y;
};

// SRC: INSPIRATION from Workshop 3.4
// skip the header line of .csv file "f"
void addressSkipHeaderLine(FILE *f) {
	while (fgetc(f) != '\n');
}

// read in each field of the file and store in a struct member
int readString(FILE *F, char *attribute, char **addr_attr){
    if(fscanf(F, "%127[^,]", attribute) != 1){
        attribute[0] = '\0'; // if field is empty, ensures to put null
    }
    *addr_attr = strdup(attribute);
    assert(*addr_attr);
    
    fscanf(F, ","); // ensure "," is not scanned twice
    return 1; 
}

// reads a address from csv file "f" to build data. 
// returns the pointer, or NULL if reading is unsuccessful.
address_t *addressRead(FILE *f) {
	address_t *addr = NULL;
    
    //create temporary field to save data
	char pfi[MAX_FIELD_LEN + 1];
    char ezi_add[MAX_FIELD_LEN + 1];
    char src_verif[MAX_FIELD_LEN + 1];
    char propstatus[MAX_FIELD_LEN + 1];
    char gcodefeat[MAX_FIELD_LEN + 1];
    char loc_desc[MAX_FIELD_LEN + 1];
    char blgunttyp[MAX_FIELD_LEN + 1];
    char hsaunitid[MAX_FIELD_LEN + 1];
    char bunit_pre1[MAX_FIELD_LEN + 1];
    char bunit_id1[MAX_FIELD_LEN + 1]; 
    char bunit_suf1[MAX_FIELD_LEN + 1];
    char bunit_pre2[MAX_FIELD_LEN + 1];
    char bunit_id2[MAX_FIELD_LEN + 1]; 
    char bunit_suf2[MAX_FIELD_LEN + 1];
    char floor_type[MAX_FIELD_LEN + 1];
    char floor_no_1[MAX_FIELD_LEN + 1]; 
    char floor_no_2[MAX_FIELD_LEN + 1]; 
    char building[MAX_FIELD_LEN + 1];
    char complex[MAX_FIELD_LEN + 1];
    char hse_pref1[MAX_FIELD_LEN + 1];
    char hse_num1[MAX_FIELD_LEN + 1]; 
    char hse_suf1[MAX_FIELD_LEN + 1];
    char hse_pref2[MAX_FIELD_LEN + 1];
    char hse_num2[MAX_FIELD_LEN + 1]; 
    char hse_suf2[MAX_FIELD_LEN + 1];
    char disp_num1[MAX_FIELD_LEN + 1]; 
    char road_name[MAX_FIELD_LEN + 1];
    char road_type[MAX_FIELD_LEN + 1];
    char rd_suf[MAX_FIELD_LEN + 1];
    char locality[MAX_FIELD_LEN + 1];
    char state[MAX_FIELD_LEN + 1];
    char postcode[MAX_FIELD_LEN + 1];
    char accestype[MAX_FIELD_LEN + 1];

    addr = malloc(sizeof(*addr));     // allocates memory for addr
    assert(addr);

    //scans through one address line
    readString(f, pfi, &addr->pfi);
    readString(f, ezi_add, &addr->ezi_add);
    readString(f, src_verif, &addr->src_verif);
    readString(f, propstatus, &addr->propstatus);
    readString(f, gcodefeat, &addr->gcodefeat);
    readString(f, loc_desc, &addr->loc_desc);
    readString(f, blgunttyp, &addr->blgunttyp);
    readString(f, hsaunitid, &addr->hsaunitid);
    readString(f, bunit_pre1, &addr->bunit_pre1);
    readString(f, bunit_id1, &addr->bunit_id1);
    readString(f, bunit_suf1, &addr->bunit_suf1);
    readString(f, bunit_pre2, &addr->bunit_pre2);
    readString(f, bunit_id2, &addr->bunit_id2);
    readString(f, bunit_suf2, &addr->bunit_suf2);
    readString(f, floor_type, &addr->floor_type);
    readString(f, floor_no_1, &addr->floor_no_1);
    readString(f, floor_no_2, &addr-> floor_no_2);
    readString(f, building, &addr->building);
    readString(f, complex, &addr->complex);
    readString(f, hse_pref1, &addr->hse_pref1);
    readString(f, hse_num1, &addr->hse_num1);
    readString(f, hse_suf1, &addr->hse_suf1);
    readString(f, hse_pref2, &addr->hse_pref2);
    readString(f, hse_num2, &addr->hse_num2);
    readString(f, hse_suf2, &addr->hse_suf2);
    readString(f, disp_num1, &addr->disp_num1);
    readString(f, road_name, &addr->road_name);
    readString(f, road_type, &addr->road_type);
    readString(f, rd_suf, &addr->rd_suf);
    readString(f, locality, &addr->locality);
    readString(f, state, &addr->state);
    readString(f, postcode, &addr->postcode);
    readString(f, accestype, &addr->accestype);

    //scan long doubles and save to struct
    if (fscanf(f, "%Lf,%Lf\n", &addr->x, &addr->y)!=2) {
        addressFree(addr);
        return NULL;
    }
	return addr;
}

// prints an address record *addr to file "f"
void addressPrint(void *file, void *addr) {
    FILE *f = (FILE *)file;
    address_t *adr = (address_t*) addr;

	fprintf(f, "--> PFI: %s || EZI_ADD: %s || SRC_VERIF: %s || PROPSTATUS: %s || GCODEFEAT: %s || LOC_DESC: %s || BLGUNTTYP: %s || HSAUNITID: %s || BUNIT_PRE1: %s ||"
        "BUNIT_ID1: %s || BUNIT_SUF1: %s || BUNIT_PRE2: %s || BUNIT_ID2: %s || BUNIT_SUF2: %s || FLOOR_TYPE: %s || FLOOR_NO_1: %s || FLOOR_NO_2: %s || BUILDING: %s || COMPLEX: %s || HSE_PREF1: %s"
        "|| HSE_NUM1: %s || HSE_SUF1: %s || HSE_PREF2: %s || HSE_NUM2: %s || HSE_SUF2: %s || DISP_NUM1: %s || ROAD_NAME: %s || ROAD_TYPE: %s || RD_SUF: %s || LOCALITY: %s"
        "|| STATE: %s || POSTCODE: %s || ACCESSTYPE: %s || x: %Lf || y: %Lf || \n", adr->pfi, adr->ezi_add, (adr)->src_verif, (adr)->propstatus, (adr)->gcodefeat, 
        (adr)->loc_desc, (adr)->blgunttyp, (adr)->hsaunitid,(adr)->bunit_pre1, (adr)->bunit_id1, (adr)->bunit_suf1, (adr)->bunit_pre2, (adr)->bunit_id2, (adr)->bunit_suf2, 
        (adr)->floor_type, (adr)->floor_no_1, (adr)->floor_no_2, (adr)->building, (adr)->complex, (adr)->hse_pref1, (adr)->hse_num1, (adr)->hse_suf1, (adr)->hse_pref2, 
        (adr)->hse_num2, (adr)->hse_suf2, (adr)->disp_num1, (adr)->road_name, (adr)->road_type,(adr)->rd_suf, (adr)->locality, (adr)->state, (adr)->postcode, 
        (adr)->accestype, (adr)->x, (adr)->y);
} 

// SRC: INSPIRATION from Workshop 3.4
// free the memory allocated to a address 
// parameter "void *" for polymorphic use
void addressFree(void *addr) {
    address_t *a = (address_t *)addr;
    
	free(a->pfi);
    free(a->ezi_add);
    free(a->src_verif); 
    free(a->propstatus); 
    free(a->gcodefeat); 
    free(a->loc_desc); 
    free(a->blgunttyp); 
    free(a->hsaunitid); 
    free(a->bunit_pre1); 
    free(a-> bunit_id1);
    free(a->bunit_suf1); 
    free(a->bunit_pre2); 
    free(a -> bunit_id2);
    free(a->bunit_suf2); 
    free(a->floor_type);
    free(a->floor_no_1);
    free(a->floor_no_2); 
    free(a->building); 
    free(a->complex); 
    free(a->hse_pref1); 
    free(a->hse_num1);
    free(a->hse_suf1); 
    free(a->hse_pref2);
    free(a->hse_num2); 
    free(a->hse_suf2); 
    free(a->disp_num1);
    free(a->road_name); 
    free(a->road_type); 
    free(a->rd_suf); 
    free(a->locality); 
    free(a->state); 
    free(a->postcode); 
    free(a->accestype);

	free(a);
}

// SRC: inspiration from WORKSHOP 3.4
// returns ezi_add of a address
char *addressGetID(void *addr) {
	return ((address_t *)addr)->ezi_add;
}

/* ==================================================================
    Line 10-53 & Line 169-216: code inspired from workshop 3.4 
====================================================================== */
