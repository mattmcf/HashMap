/*
 * Matt McFarland
 * KPCM Engineering Fellows Application - October 2015
 *
 * Hashmap.h contains functions for creating and storing
 * values in a fixed size hashmap where the key is a 
 * string of arbitrary length and the data object is a data reference
 * pointer. 
 *
 */

#ifndef HASHMAP_h
#define HASHMAP_h

/* ------ PUBLIC CONSTANTS ------- */
#define SUCCESS 		0
#define FAILURE 		1


/* ------ STRUCTURES ------ */
typedef struct HashNode {
	int occupied;
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


/* ------ PUBLIC FUNCTIONS ------ */

/*
 * CreateMap returns a preallocated HashMap that
 * can contain up to size entries. 
 *
 * ARGUMENTS:
 * 		size - must be an integer >= 1
 * 
 * RETURN VALUE:
 * 		Returns pointer to new HashMap if successful
 * 		Returns NULL on failure
 *
 * USAGE:
 * 		HashMap * new_map = CreateMap( size )
 * 
 */
HashMap * CreateMap(int size);

/*
 * Set will add a key-value pair to the HashMap
 * if it does not exist. If the key already exists
 * in the HashMap, then the existing data value will
 * be overwritten.
 *
 * Note - NULL keys cannot be set in HashMap. Null
 * strings can be a valid key however. (ie. key == NULL
 * will return FAILURE, but key == "" will try to insert
 * or update the data element.) NULL data values are
 * valid data elements to store for a key.
 *
 * ARGUMENTS:
 * 		map - pointer to valid HashMap to add key-value to
 *		key - string key to add or update value for (cannot be NULL)
 *		val - data reference to store with key
 * 
 * RETURN VALUE:
 * 		Returns SUCCESS if key was added/updated 
 * 		Returns FAILURE if error occurs
 *  
 */ 
int Set(HashMap * map, char * key, void * val);

/*
 * Get will retrieve the data element for a specified
 * key if that key exists in the given HashMap. If the
 * key does not exist in the HashMap, NULL is returned.
 * 
 * Note - In the case that the data element was NULL,
 * there is no way to discern between and error return
 * value and the data element.
 *
 * ARGUMENTS:
 * 		map - pointer to valid HashMap to search for key in
 *		key - string key to find value for
 * 
 * RETURN VALUE:
 * 		Returns data value if key-value exists in HashMap
 * 		Returns NULL if key could not be located
 *  
 */  
void * Get(HashMap * map, char * key);

/*
 * Delete will remove the key-value pair from the HashMap
 * for a given key if that key exists in the HashMap. If the
 * key exists the key-value pair is removed, the data element
 * for that key is returned.
 *
 * Note - In the case that the data element was NULL,
 * there is no way to discern between and error return
 * value and the data element.
 *
 * ARGUMENTS:
 * 		map - pointer to valid HashMap to delete entry from
 *		key - string key to delete
 * 
 * RETURN VALUE:
 * 		Returns data value if key-value existed in the
 * 			HashMap and was deleted
 * 		Returns NULL if key did not exist or data element was NULL
 *  
 */  
void * Delete(HashMap * map, char * key);

/*
 * GetLoad returns the load factor for the specified
 * HashMap (defined as entries currently map divided by
 * the size of the HashMap). Load factor is a float value
 * between 0 and 1.0 . 
 *
 * ARGUMENTS:
 * 		map - pointer to valid HashMap retrieve load from 
 * 
 * RETURN VALUE:
 * 		Returns load factor is map exists
 * 		Returns FAILURE if map value array does not exist
 *
 */ 
float GetLoad(HashMap * map);

/*
 * DeleteMap will free the space allocated for the map
 * if memory has been allocated to it. The size and count
 * of the map are also reset to 0.
 *
 * ARGUMENTS:
 * 		map - pointer to valid HashMap to delete
 * 
 * RETURN VALUE:
 * 		Returns SUCCESS if memory was freed and map was cleared
 * 		Returns FAILURE if map or values array was invalid
 *
 */ 
int DeleteMap(HashMap * map);

/*
 * PrintMap prints the HashMap to stdout 
 * 
 * ARGUMENTS:
 * 		map - pointer to valid HashMap to print
 * 
 * RETURN VALUE:
 * 		None
 *
 */
void PrintMap(HashMap * map);

#endif // HASHMAP_h
