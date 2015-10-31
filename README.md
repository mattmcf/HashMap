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

- ARGUMENTS:
size - must be an integer >= 1

- RETURN VALUE:
Returns pointer to new HashMap if successful
Returns NULL on failure
 

### Set
#### int Set(HashMap * map, char * key, void * val)

Set will add a key-value pair to the HashMap if it does not exist. If the key already exists in the HashMap, then the existing data value will be overwritten.

Note - NULL keys cannot be set in HashMap. Null strings can be a valid key however. (ie. key == NULL will return FAILURE, but key == "" will try to insert or update the data element.) NULL data values are valid data elements to store for a key.

-ARGUMENTS:\n
map - pointer to valid HashMap to add key-value to\n
key - string key to add or update value for (cannot be NULL)\n
val - data reference to store with key\n
 
- RETURN VALUE:
Returns SUCCESS if key was added/updated 
Returns FAILURE if error occurs
  
