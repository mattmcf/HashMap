// Matt McFarland
// KPCB project test file

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "../src/hashmap.h"

#define SIZE 50

int main(void) {

	printf("This is the test program for Matt McFarland's HashMap.\n");
	int ret_val = 0;
	//void * ret_ptr;

	int arr[SIZE];
	for (int i = 0; i < SIZE; i++){
		arr[i] = i;
	}

	/* --- create hashmap tests --- */

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
	if (map3 != NULL)
		ret_val++;
	
	
	// testing addition of values
	// add a null key
	// add a null string
	// add a regular string
	// change a previously set value
	// what happens when you try to add to full map?
	

	printf("ret_val = %d\n",ret_val);	

	return 0;
}
