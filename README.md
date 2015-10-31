# Matt McFarland HashMap README

This project was created for KPCB's Engineering Fellows application (2015). It includes two source files that implement the functionality of the fixed size hashmap and a test file to test the implementation of the hashmap.

## Project Description:

This hashmap is a fixed size hashmap implemented in C. A fixed size hashmap means that there is a limited number of entries that can be stored in the map. All of the elements are stored within the hashmap itself (as opposed to a hashmap where each collided entry is chained off of the first element in a given slot)


- describe HASH used, 
- detail why I need stdlib.h, string.h

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
#### void * Get(HashMap * map, char * key)

Get will retrieve the data element for a specified key if that key exists in the given HashMap. If the key does not exist in the HashMap, NULL is returned.

Note - In the case that the data element was NULL, there is no way to discern between and error return value and the data element.

ARGUMENTS:

- map: pointer to valid HashMap to search for key in
- key: string key to find value for

RETURN VALUE:
- Returns data value if key-value exists in HashMap
- Returns NULL if key could not be located

### Delete
#### void * Delete(HashMap * map, char * key)

Delete will remove the key-value pair from the HashMap for a given key if that key exists in the HashMap. If the key exists the key-value pair is removed, the data element for that key is returned.

Note - In the case that the data element was NULL, there is no way to discern between and error return value and the data element.

ARGUMENTS:

- map: pointer to valid HashMap to delete entry from
- key: string key to delete
  
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
