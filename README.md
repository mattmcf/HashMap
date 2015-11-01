# Matt McFarland HashMap README

This project was created for KPCB's Engineering Fellows application (2015). It includes two source files that implement the functionality of the fixed size hashmap and a test file to test the implementation of the hashmap.

This project lives at: https://github.com/MattRMcFarland/HashMap.git

## Project Description:

This hashmap is a fixed size hashmap implemented in C. A fixed size hashmap means that there is a limited number of entries that can be stored in the map. All of the elements are stored within the hashmap itself (as opposed to a hashmap where each collided entry is chained off of the head element in a given slot). So if the hashmap is full and a new element is added, a FAILURE occurs and the new element isn't added. Keys are only placed in the hashmap once, so if the Set function is invoked for a key that already exists in the HashMap, then that key's value will be updated with the new value as opposed to adding a new key-value pair.

In order to manage the complexity of tracking and finding collisions that can occur within a fixed size hash map, this hashmap uses a doubly-linked list of HashNodes for a single slot to track it's collided elements. For example, if keys A, B, and C all hash to slot 13 (and are added in that order), then A will be located in slot 13. When B is added, B will collide with A in slot 13. So a free slot is found and then B is placed in that free slot. Then A's next element is pointed at B and B's previous element is pointed at A. When C is added and collides with A in slot 13, C will traverse down A's next list of collided nodes until the end of the list is found. A free slot for C is found and C is inserted there. C is then linked to the end of A's collided list. In this example, that means B's next element will point at C's slot and C's previous element will point at B's slot. 

Continuing this example with A, B, and C, if a fourth element, D is added that hashes to the slot that B occupies, D will see that B is occupying D's intended slot. Since B was place there arbitrarily, and D belongs is B's slot, B is moved to a different, arbitrary free slot and D is placed in B's old location. B's adjacent neighbors in its linked list of next and previous elements are updated to relect B's new location. If element E is added that also maps to D's slot, then E will be placed in an arbitrary, free slot and attached to D's collision list.

If element A is removed, then A is deleted from its slot. However, because A is the head of a collision list for slot 13 and that collision list isn't empty, a new head element need to be placed in slot 13 to maintain the integrity of that collision list. So B, the next element in that collision list, is moved from it's arbitrary location and placed in slot 13. B updates it's predecessor with its new location information.

Other Information:

Hash Function: I used the djb2 hash for my hash function. This acts on a string of arbitrary length to return a hash value. More information is included here: http://www.cse.yorku.ca/~oz/hash.html

Since I implemented this project in C, I needed to include the following libraries:
- stdlib.h for its memory allocation functions
- string.h for its string operation functions (since a string class would be built into other languages)
- stdio.h for its I/O operations (for verbose debugging and printing map)

## Project Contents

/src/hashmap.h       - public functions and definitions for interacting with HashMap

/src/hashmap.c       - implementation of functions in hashmap.h as well as private functions

/test/TestHashMap.c  - test routine for the functionality of the HashMap

Makefile             - builds TestHashMap and runs it

README.md            - this description file

## Building Instructions

Run "make" - This will build TestHashMap executable and run it. If build and tests are completed successfully, user will be notified. Run "make clean" to clean up executables and auxillary files.

The executable TestHashMap can be run in two ways:
- "./TestHashMap" = Quiet mode. Only test result is reported.
- "./TestHashMap verbose" = Verbose mode. Detailed testing is reported to stdout.

## Function Descriptions

### CreateMap
#### HashMap * CreateMap(int size)

CreateMap returns a preallocated HashMap that
can contain up to size entries.

ARGUMENTS:

- size: must be an integer >= 1

RETURN VALUE:

- Returns pointer to new HashMap if successful
- Returns NULL on failure
 

### Set
#### int Set(HashMap * map, char * key, void * val)

Set will add a key-value pair to the HashMap if it does not exist. If the key already exists in the HashMap, then the existing data value will be overwritten.

Note - NULL keys cannot be set in HashMap. Null strings can be a valid key however. (ie. key == NULL will return FAILURE, but key == "" will try to insert or update the data element.) NULL data values are valid data elements to store for a key.

ARGUMENTS:

- map: pointer to valid HashMap to add key-value to\n
- key: string key to add or update value for (cannot be NULL)\n
- val: data reference to store with key\n
 
RETURN VALUE:

- Returns SUCCESS if key was added/updated 
- Returns FAILURE if error occurs

### Get
#### void * Get(HashMap * map, char * key, int * status_ptr)

Get will retrieve the data element for a specified key if that key exists in the given HashMap. If the key does not exist in the HashMap, NULL is returned.

Note - If a key has a NULL data value, that NULL will be returned by Get. The result of the status variable pointed at in the argument will state whether a value was successfully retrieved (SUCCESS) or if the key could not be found/error occurred. So if Get returns NULL and the status variable is SUCCESS, the key exists in the HashMap and is NULL. If Get returns NULL and status is FAILURE, then the key isn't in the HashMap.

ARGUMENTS:

- map: pointer to valid HashMap to search for key in
- key: string key to find value for

RETURN VALUE:
- Returns data value if key-value exists in HashMap
- Returns NULL if key could not be located

### Delete
#### void * Delete(HashMap * map, char * key, int * status_ptr)

Delete will remove the key-value pair from the HashMap for a given key if that key exists in the HashMap. If the key exists the key-value pair is removed, the data element for that key is returned.

Note - If a key has a NULL data value, that NULL will be returned by Delete. The result of the status variable  pointed at in the argument will state whether a value was successfully retrieved (SUCCESS) or if the key could not be found/error occurred. So if Delete returns NULL and the status variable is SUCCESS, the key existed in the HashMap and the data was NULL. If Delete returns NULL and status is FAILURE, then the key wasn't in the HashMap.

ARGUMENTS:

- map: pointer to valid HashMap to delete entry from
- key: string key to delete
- status_ptr - pointer to variables where SUCCESS or FAILURE will be stored on return (if NULL, status won't be saved)
  
RETURN VALUE:

- Returns data value if key-value existed in the HashMap and was deleted
- Returns NULL if key did not exist or data element was NULL

### GetLoad
#### float GetLoad(HashMap * map)

GetLoad returns the load factor for the specified HashMap (defined as entries currently map divided by the size of the HashMap). Load factor is a float valuebetween 0 and 1.00. 

ARGUMENTS:

- map: pointer to valid HashMap retrieve load from 

RETURN VALUE:

- Returns load factor is map exists
- Returns FAILURE if map value array does not exist

### DeleteMap
#### int DeleteMap(HashMap * map)

DeleteMap will free the space allocated for the map if memory has been allocated to it. The size and count of the map are also reset to 0.

ARGUMENTS:

- map - pointer to valid HashMap to delete

RETURN VALUE:

- Returns SUCCESS if memory was freed and map was cleared
- Returns FAILURE if map or values array was invalid

### PrintMap
#### void PrintMap(HashMap * map)

PrintMap prints the HashMap to stdout 

ARGUMENTS:

- map: pointer to valid HashMap to print

RETURN VALUE:

- NA
