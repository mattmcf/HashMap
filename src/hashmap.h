/*
 * Matt McFarland
 * KPCM Engineering Fellows Application - October 2015
 *
 * Hashmap.h contains functions for creating and storing
 * values in a fixed size hashmap where the key is a 
 * string of arbitrary length and the data object is a 
 * 32 bit reference (pointer).
 */

#ifndef HASHMAP_h
#define HASHMAP_h

/* ------ CONSTANTS ------- */
#define UNOCCUPIED 		0
#define OCCUPIED 		1

#ifndef NULL
#define NULL		0x00
#endif

/* is this a better way to utilize space? */
typedef struct HashNode {
	int occupied;			/* FREE or OCCUPIED */
	char * key;
	void * data;
	unsigned long hash;

	struct HashNode * next;
	struct HashNode * prev;
} HashNode;

typedef struct HashMap {
	int size;
	int count;
	HashNode * values;	
} HashMap;

/*
 * CreateMap returns a preallocated HashMap that
 * can contain up to size references
 *
 * returns a pointer to the HashMap struct if successful
 * returns NULL on failure
 */
HashMap * CreateMap(int size);

/*
 * For the given key, set the val to that key's value
 * overwrites a previous entry at that key
 *
 * returns 1 on success
 * returns 0 on failure
 */
int Set(HashMap * map, char * key, void * val);

/*
 * Returns the reference object for the given key
 *
 * returns NULL if no value was set
 * Note: if the value set was NULL, then this will return a false failure
 */
void * Get(HashMap * map, char * key);

/*
 * Deletes a key value pair
 *
 * returns the value if successful, else returns NULL
 * Note: If the value stored was NULL, then this will return a false failure
 */
void * Delete(HashMap * map, char * key);

/*
 * Returns the load factor in the hashmap
 *
 * load defined as (items in hashmap)/(size of hashmap)
 */
float GetLoad(HashMap * map);

#endif // HASHMAP_h
