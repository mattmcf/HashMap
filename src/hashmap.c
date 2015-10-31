/*
 * Matt McFarland
 * KPCM Engineering Fellows Application - October 2015
 *
 * HashMap.c implements the fixed size HashMap with string keys
 * described in HashMap.h
 */

/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>		// for malloc, mempcy
#include <string.h> 	// for strcmp, strlen, strcpy 
#include "hashmap.h"

/* DEFINES */

#define SUCCESS 		0
#define FAILURE 		1
#define SLOT_FAILURE 	-1

/* ----- PRIVATE PROTOTYPES ------ */
int AddValueHere(HashNode * node, char * key, void * val); 
int MoveValueTo( HashNode * to, HashNode * from);
int FindNewSlot(HashMap * map, int start_point);
int EraseNode(HashNode * target);
unsigned long djb2(unsigned char * src);

/*
 *
 * ------ PUBLIC FUNCTIONS ------
 * 
 */

/*
 * CreateMap creates a hash map that can contain up to size values. 
 * This requires dynamic allocation because the size of the hash map is unknown
 * until called at run time.
 * 
 * If the map is successfully created, a pointer to is it returned.
 * If an error occurs, NULL is returned
 */
HashMap * CreateMap(int size) {
	HashMap * map;
	
	if (size < 1) {
		fprintf(stderr,"size must be greater than zero\n");
		return NULL;
	}

	map = (HashMap *)calloc(1,sizeof(HashMap));
	if (!map) {
		fprintf(stderr,"malloc error\n");
		return NULL;
	}

	map->values = (HashNode *)malloc(sizeof(HashNode) * size);
	if (!map->values) {
		fprintf(stderr,"malloc error\n");
		return NULL;
	}

	return map;
}

/*
 * Set stores a key-value pair in the hashmap
 * 
 * The key cannot be null.
 * if the key is already present in the hashmap, the existing value is overwritten.
 * 
 * returns SUCCESS if set was successful
 * returns FAILURE on failure
 */
int Set(HashMap * map, char * new_key, void * new_val) {

	HashNode * probe;

	if (!map){
		fprintf(stderr,"cannot add to null map\n");
		return FAILURE;
	} else if (!new_key) {
		fprintf(stderr,"cannot add null keys\n");
		return FAILURE;
	}

	HashNode * arr 					= map->values;		// for easier access;

	unsigned long this_hash 		= djb2((unsigned char *)new_key);
	int this_slot 					= this_hash % map->size;
	//int this_len 					= strlen(new_key);


	/* check if slot is unoccupied */
	if (arr[this_slot].occupied == UNOCCUPIED){

		/* slot is unoccupied, add new key-value pair here */
		if (FAILURE == AddValueHere( &arr[this_slot], new_key, new_val))
			return FAILURE;

		map->count++;

	/* check if slot is occupied by entry that also maps to specified slot */
	} else if ( this_slot == (arr[this_slot].hash % (map->size)) )  {

		/* slot is occupied by something that also maps to this slot */
		/* traverse down list to see if key-value already exists */
		probe = &arr[this_slot];
		int contains = 0;
		while (!contains && probe->next != NULL){

			/* compare current entry in list to key to add/update */
			if (strcmp(new_key,probe->key)) {

				/* key-value already exists in list, update value */
				contains = 1;
				probe->data = new_val;

			} else {

				/* if a next key-value exists in list, move to it */
				if (probe->next)
					probe = probe->next;
			}
		}

		/* if the key-value to add isn't in the list at this slot, add it */
		if (!contains) {

			/* find an empty slot */
			int new_slot;
			if (SLOT_FAILURE == (new_slot = FindNewSlot(map, this_slot)) )					
				return FAILURE;

			/* add new entry to the new slot in a collision list */
			if (FAILURE == AddValueHere(&arr[this_slot], new_key, new_val))  
				return FAILURE;

			/* connect new entry to the list from the original slot */
			probe->next = &arr[new_slot];					
			probe->prev = &arr[this_slot];

			map->count++;
		}
		
	/* check if slot is unoccupied by collided element from separate list */
	} else if (this_slot != arr[this_slot].hash % map->size)  {

		/* Slot is occupied but not by something that needs to be here. Bump it out */
		int move_to;
		if (SLOT_FAILURE == (move_to = FindNewSlot(map, this_slot)) )  
			return FAILURE;

		/* Bump non-related entry out of this slot */
		if (FAILURE == MoveValueTo( &arr[move_to], &arr[this_slot]))
			return FAILURE;

		/* Add new entry to this slot */
		if (FAILURE == AddValueHere( &arr[this_slot], new_key, new_val))
			return FAILURE;

		map->count++;

	} else {
		fprintf(stderr,"how did we get here ????\n");
		return FAILURE; 
		/* how did we end up here? */
	}

	return 0;
}

/*
 * Returns the reference object for the given key
 *
 * returns NULL if no value was set
 * Note: if the value set was NULL, then this will return a false failure
 */
void * Get(HashMap * map, char * key) {

	if (!map || !key) {
		fprintf(stderr,"HashMap or key was null\n");
		return NULL;
	}

	unsigned long hash 			= djb2((unsigned char *)key);
	int slot 					= hash % map->size;
	HashNode * current_node 	= &map->values[slot];
	int found 					= 0;
	void * to_return;


	/* while key hasn't been found and there are entries at this slot */
	while (!found && (current_node != NULL) && (current_node->occupied == OCCUPIED)) {

		/* If we found the matching entry, save the data */
		if (strcmp(key, current_node->key) == 0){
			to_return = current_node->data;	

		/* else move to the next entry */			
		} else {
			current_node = current_node->next;
		}
	}

	if (!found)
		to_return = NULL;

	return to_return;
}

/*
 * Deletes a key value pair
 *
 * returns the value if successful, else returns NULL
 * Note: If the value stored was NULL, then this will return a false failure
 */
 void * Delete(HashMap * map, char * search_key){

 	if (!map){
 		fprintf(stderr,"cannot delete entry from null map\n");
 		return NULL;
 	} else if (!search_key) {
 		fprintf(stderr,"cannot delete null entry\n");
 		return NULL;
 	}
 	
 	HashNode * current_node;	// search probe
 	HashNode * saved_next;		// saved next entry for slot	
 	int found = 0;				// found flag

 	void * to_return 			= NULL;
 	unsigned long hash 			= djb2((unsigned char *)search_key);
	int slot 					= hash % map->size;
	current_node 				= &map->values[slot];

	/* While there are unexamined entries at the search key slot */
	while (!found && (current_node != NULL)) {					

		/* compare search key against entry */
		if (strcmp(search_key, current_node->key) == 0) {	

			/* found! update flag and save data payload */
			found = 1;										
			to_return = current_node->data;

		} else {

			/* else entry didn't match the search string, get next */
			current_node = current_node->next;	
		}
	}

	/* delete entry if it matched key */
	if (found) {

		saved_next = NULL;
		/* if the matching entry is at the start of a collision list, save next pointer*/
		if (current_node->next && !current_node->prev){
			saved_next = current_node->next;
		}

		EraseNode(current_node);
		map->count--;

		/* if the erased entry was at the head of a list, update head */
		if (saved_next != NULL)
			MoveValueTo(&map->values[slot], saved_next);	
	}

	return to_return;
 }

/*
 * Returns the load factor in the hashmap
 *
 * load defined as (items in hashmap)/(size of hashmap)
 * returns FAILURE if map is not defined 
 */
float GetLoad(HashMap * map) {

	if (!map) {
		fprintf(stderr,"cannot get load of null map\n");
		return FAILURE;
	}

	float load = (float)map->count / map->count;

	return load;
}

/*
 *
 * ------ PRIVATE FUNCTIONS ------
 * 
 */

/*
 * AddValueHere places a key and value in a specified slot
 * if addition is sucessful, returns SUCCESS
 * if addition fails, returns FAILURE
 */
int AddValueHere(HashNode * node, char * new_key, void * val) {

	if (!node || !new_key) {
		fprintf(stderr,"cannot add value. node (%p) or key (%p) is null\n", node, new_key);
		return FAILURE;
	}

	unsigned long hash 	= djb2((unsigned char *)new_key);	
	int len 			= strlen(new_key);
	
	node->key = (char *)malloc((len+1) * sizeof(char));
	if (!node->key){
		fprintf(stderr,"could not add key %s. malloc error.\n",new_key);
		return FAILURE;
	}
	strcpy(node->key,new_key);

	node->hash 			= hash;
	node->data 			= val;
	node->next 			= NULL;
	node->prev 			= NULL;
	node->occupied 		= OCCUPIED;

	return SUCCESS;
}

/*
 * Moves hashnode at location to specified to location
 * returns SUCCESS on sucess
 * returns FAILURE on failure
 */ 
int MoveValueTo( HashNode * to, HashNode * from) {

	if (!to || !from){
		fprintf(stderr,"cannot move hashnode from (%p) to (%p) because one is null\n",from, to);
		return FAILURE;
	} else if (from->occupied == UNOCCUPIED) {
		fprintf(stderr,"cannot move from entry because entry is unoccupied\n");
		return FAILURE;
	} else if (from->key == NULL) {
		fprintf(stderr,"front key is null. error.\n");
		return FAILURE;
	}

	/* move data elements to new location */
	to->occupied 	= OCCUPIED;
	to->key 		= from->key;
	to->data 		= from->data;
	to->hash 		= from->hash;
	to->next 		= from->next;
	to->prev 		= from->prev;

	/* if the previous location was in a list, update list pointers on neighbors */
	if (from->next)
		from->next->prev = to;

	if (from->prev)
		from->prev->next = to;

	/* label previous location as unoccupied */
	from->occupied 	= UNOCCUPIED;

	return SUCCESS;
}

/*
 * FindNewSlot locates an UNOCCUPIED slot and returns that index
 * if no other slots are open, returns SLOT_ERROR (-1)
 */
int FindNewSlot(HashMap * map, int start_point) {

	if (!map){
		fprintf(stderr,"map is null\n");
		return SLOT_FAILURE;
	}

	if (map->size == map->count){
		fprintf(stderr,"map is full\n");
		return SLOT_FAILURE;
	} else if (start_point < 0 || start_point >= map->size) {
		fprintf(stderr,"invalid start point\n");
		return SLOT_FAILURE;
	}

	/* Poll list linearly until free slot is found */
	int probe = start_point;
	while (map->values[probe].occupied == OCCUPIED){
		probe++;
		if (probe == map->size)				// in reached end of list, reset to beginning
			probe = 0;
	}

	return probe;
}

/* 
 * deletes node and removes from list
 *
 * returns SUCCESS or FAILURE
 */
int EraseNode(HashNode * target) {

	if (!target){
		fprintf(stderr,"cannot erase null node\n");
		return FAILURE;
	}

	/* fix linked lists */
	if (target->prev)
		target->prev->next = target->next;

	if (target->next)
		target->next->prev = target->prev;

	/* now delete node */
	target->occupied 		= UNOCCUPIED;
	free(target->key);

	target->key 			= NULL;
	target->data			= NULL;
	target->hash 			= 0;

	return SUCCESS;
}

/*
 * djb2 hash for a string
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long djb2(unsigned char * str) {
	unsigned long hash = 5381;
	int c;

	while ( (c = *str++) )
		hash = ((hash << 5) + hash) + c;

	return hash;
}


