// Matt McFarland
// KPCB project test file

#include <stdio.h>			// for I/O functions
#include <limits.h>			// for INT_MAX
#include <string.h>
#include <stdlib.h> 		// for malloc and free
#include "../src/hashmap.h"

#define SIZE 		10
#define MAX_LEN	 	20

int main(int argc, char ** argv) {

	int verbose = 0; 				// if 1, will print out detailed testing
	FILE * new_stderr;

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

	int arr[SIZE];
	for (int i = 0; i < SIZE; i++){
		arr[i] = i;
	}

	/* --- testing hashmap creation --- */
	if (verbose)
		printf("testing map creation\n");

	// regular size
	HashMap * map0 = CreateMap(SIZE);
	if (!map0)
		ret_val++;

	// size 0
	HashMap * map1 = CreateMap(0);
	if (map1 != NULL)
		ret_val++;

	// size negative
	HashMap * map2 = CreateMap(-6);
	if (map2 != NULL)
		ret_val++;

	// size max_int
	HashMap * map3 = CreateMap(INT_MAX);
	if (map3 == NULL)
		ret_val++;
	else {
		if (FAILURE == DeleteMap(map3))
			ret_val++;
	}
	
	
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
	if (FAILURE == Set(map0, "", (void *)&arr[0]))		// should be able to set null string
		ret_val++;

	// add a bunch of regular strings
	char str[MAX_LEN];
	for (int i = 0; i < SIZE-1; i++){
		sprintf(str, "%d", i );	 						// use index strings for keys

		if (verbose)
			printf("adding str %s\n", str);

		if (FAILURE == Set(map0, str, &arr[i]))
			ret_val++;
	}

	if (verbose)
		printf("load is now %f\n", GetLoad(map0));		// should be 1.0 (full)

	// test setting value while map is full
	if (FAILURE != Set(map0, "full now", (void *)&main))		// should not be able to add to full maps
		ret_val++;		


	/* --- test getting values --- */
	if (verbose)
		printf("testing get values\n");

	for (int i = 0; i < SIZE-1; i++) {

		sprintf(str, "%d", i );	 						// use index strings for keys
		if (&arr[i] != Get(map0, str))
			ret_val++;
	}

	/* --- test removing values --- */
	if (verbose)
		printf("testing removal of values\n");

	for (int i = SIZE-1; i >= 0; i--) {
		sprintf(str, "%d", i );	 						// use index strings for keys
	
		if (Get(map0,str)) {

			// if value is in map, delete it
			if (verbose)
				printf("removing str %s\n", str);

			if (&arr[i] != Delete(map0, str))
				ret_val++;		
		}
	}

	if (verbose)
		printf("load is now %f\n", GetLoad(map0));

	/* if we remove the key "" (set at line 87 in this file) after adding "0", can we still find "0"? */

	if (FAILURE == Set(map0, "0", (void *)&main))		// "0" collides with "" in slot 1
		ret_val++;

	if (verbose)
		PrintMap(map0);									// "0" should be in slot 2

	if (FAILURE == Set(map0, "1", &ret_val))			// bump "0" out of slot 2
		ret_val++;

	if (verbose)
		PrintMap(map0);									// "1" should be in slot 1 and "0" elsewhere

	if ((void *)&arr[0] != Delete(map0, ""))			// delete "", "0" should get moved to slot 1
		ret_val++;

	if ((void *)&main != Get(map0, "0"))				// should be able to get "0" still
		ret_val++;

	if (verbose)
		PrintMap(map0);									// "0" should be is slot 1 now
	
	if (FAILURE == DeleteMap(map0))
		ret_val++;

	/* end of testing */

	if (ret_val == 0)
		printf("All tests have passed!\n");
	else
		printf("Error occured in some tests\n");

	/* if all tests succeeded, then return value will be zero */
	return 0;
}
