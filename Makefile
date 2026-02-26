dict1: dict.o data.o list.o bit.o trie.o
	gcc -Wall -g -o dict1 dict.o data.o list.o bit.o trie.o

dict2: dict.o data.o list.o bit.o trie.o
	gcc -Wall -g -o dict2 dict.o data.o list.o bit.o trie.o

dict.o: dict.c 
	gcc -Wall -g -o dict.o -c dict.c 

trie.o: trie.c trie.h
	gcc -Wall -g -o trie.o -c trie.c

data.o: data.c data.h 
	gcc -Wall -g -o data.o -c data.c 

list.o: list.c list.h  
	gcc -Wall -g -o list.o -c list.c 

bit.o: bit.c bit.h
	gcc -Wall -g -o bit.o -c bit.c

clean:
	rm -f *.o dict1 

	