// Matt McFarland
// KPCB project test file

#include <stdio.h>			// for I/O functions
#include <limits.h>			// for INT_MAX
#include <string.h>
#include <stdlib.h> 		// for malloc and free
#include "../src/hashmap.h"

#define SIZE 		10			// make a HashMap 10 elements long
#define MAX_LEN	 	20			// for testing strings

int main(int argc, char ** argv) {

	int verbose = 0; 				// if 1, will print out detailed testing
	FILE * new_stderr = -1;

	if (argc > 2) {

		fprintf(stderr,"invalid arguments for TestHashMap.\n");
		return FAILURE;

	} else if (argc == 1) {			// quiet mode

		new_stderr = fopen("/dev/null","w");
		stderr = new_stderr;

	} else if (argc == 2) {

		if (strcmp(argv[1],"verbose") == 0) {
			verbose = 1;			// detailed testing report desired

		} else {
			fprintf(stderr,"did not recognize option. exiting.\n");
			return FAILURE;
		}
	} 

	printf("This is the test program for Matt McFarland's HashMap.\n");
	int ret_val = 0;
	int status_var;

	int arr[SIZE];
	for (int i = 0; i < SIZE; i++){
		arr[i] = i;
	}

	/* --- testing hashmap creation --- */
	if (verbose)
		printf("testing map creation\n");

	// regular size
	HashMap * map0 = CreateMap(SIZE);			// should work
	if (!map0)
		ret_val++;

	// size 0
	HashMap * map1 = CreateMap(0);				// should fail, cannot have 0 size
	if (map1 != NULL)
		ret_val++;

	// size negative
	HashMap * map2 = CreateMap(-6);				// should fail, cannot have negative size
	if (map2 != NULL)
		ret_val++;
	
	/* --- testing addition of values --- */
	if (verbose)
		printf("testing value setting\n");

	// testing null map
	if (FAILURE != Set(NULL, "foo", (void *)arr))		// cannot set value for null map
		ret_val++;

	// testing null key
	if (FAILURE != Set(map0, NULL, (void *)arr))		// cannot set null key
		ret_val++;

	// add empty string
	if (SUCCESS != Set(map0, "", (void *)&arr[0]))		// should be able to set null string
		ret_val++;

	// add key with null data
	if (SUCCESS != Set(map0, "null val", NULL))			// should be valid
		ret_val++;

	/* test get and delete for data with NULL data */

	if (verbose)
		printf("testing Get and Delete for null data\n");

	// test for key with null data element
	if (NULL != Get(map0, "null val", &status_var))		// should return NULL data element
		ret_val++;

	if (status_var != SUCCESS)							// saved status should be SUCCESS
		ret_val++;										

	// test removal of key with null data element
	if (NULL != Delete(map0, "null val", &status_var))	// should return NULL data element
		ret_val++;

	if (status_var != SUCCESS)							// saved status should be SUCCESS
		ret_val++;

	if (verbose)
		printf("testing Get and Delete for keys not in map\n");

	// test get with key that doesn't exist
	if (NULL != Get(map0, "not in map", &status_var)) 	// should return NULL for failure
		ret_val++;

	if (status_var != FAILURE)							// failure because key isn't in map
		ret_val++;

	// test Delete with key that doesn't exist
	if (NULL != Delete(map0, "not in map", &status_var)) 	// should return NULL for failure
		ret_val++;

	if (status_var != FAILURE)							// failure because key isn't in map
		ret_val++;

	/* test with full map and data collisions */
	if (verbose)
		printf("testing with regular strings\n");

	// add a bunch of regular strings
	char str[MAX_LEN];
	for (int i = 0; i < SIZE-1; i++){
		sprintf(str, "%d", i);	 						// use index strings for keys

		if (verbose)
			printf("adding str %s\n", str);

		if (SUCCESS != Set(map0, str, &arr[i]))
			ret_val++;
	}

	if (verbose)
		PrintMap(map0);

	if (verbose)
		printf("load is now %f\n", GetLoad(map0));		// should be 1.0 (full)

	// test setting value while map is full
	if (FAILURE != Set(map0, "FULLNOW", (void *)&main))		// should not be able to add to full maps
		ret_val++;		


	/* --- test getting values --- */
	if (verbose)
		printf("testing get values\n");

	for (int i = 0; i < SIZE-1; i++) {

		sprintf(str, "%d", i );	 						// use index strings for keys
		if (&arr[i] != Get(map0, str, NULL))			// not using NULL data so no need to save status
			ret_val++;
	}

	/* --- test removing values --- */
	if (verbose)
		printf("testing removal of values\n");

	for (int i = SIZE-2; i >= 0; i--) {					// remove all of added strings
		sprintf(str, "%d", i);	 						// use index strings for keys
	
		if (NULL != Get(map0,str,NULL)) {				// not using NULL data so no need to save status

			// if value is in map, delete it
			if (verbose)
				printf("removing str %s\n", str);

			if (&arr[i] != Delete(map0, str, NULL))		// not using NULL data so no need to save status
				ret_val++;		
		} else {
			ret_val++;									// each value should be in HashMap
		}
	}

	if (verbose)
		printf("load is now %f\n", GetLoad(map0));

	/* 
	 * These tests will examine how elements are bumped around when collisions occurs.
	 * Size should be 10 for these to illustrate how collisions are handled.
	 */
	if (SUCCESS != Set(map0, "0", (void *)&main))		// "0" collides with "" in slot 1
		ret_val++;

	if (verbose)
		PrintMap(map0);									// "0" in a different slot (2) than ""

	if (SUCCESS != Set(map0, "1", &ret_val))			// bump "0" out of slot 2
		ret_val++;

	if (verbose)
		PrintMap(map0);									// "1" should be in slot 1 and "0" elsewhere

	if ((void *)&arr[0] != Delete(map0, "", NULL))		// delete "", "0" should get moved to slot 1
		ret_val++;

	if ((void *)&main != Get(map0, "0", NULL))			// should be able to get "0" still
		ret_val++;

	if (verbose)
		PrintMap(map0);									// "0" should be is slot 1 now
	
	if (SUCCESS != DeleteMap(map0))
		ret_val++;
	map0 = NULL;

	/* end of testing */

	if (ret_val == 0)
		printf("All tests have passed!\n");
	else
		printf("Error occured in some tests (%d errors)\n", ret_val);

	/* close rerouted stderr */
	if (new_stderr != -1)
		fclose(new_stderr);

	/* if all tests succeeded, then return value will be zero */
	return 0;
}
