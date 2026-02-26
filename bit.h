/* Number of bits in a single character. */
#define BITS_PER_BYTE 8

int getBit(char *s, unsigned int bitIndex);

/* Allocates new memory to hold the numBits specified and fills the allocated
    memory with the numBits specified starting from the startBit of the oldKey
    array of bytes. */
char *createStem(char *oldKey, unsigned int startBit, unsigned int numBits);