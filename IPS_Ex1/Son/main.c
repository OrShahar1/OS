//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#include <stdio.h>


void main(int argc, char** argv)
{
	int burned_trees_counter = 0;

	if (argc != 2){
		printf("ERROR: wrong amount of arguments (%d instead of 2)\n", argc);
		printf("File: %s, Line: %d, Function: %s\n", __FILE__, __LINE__, __func__);
		exit(-1);
	}

	char* forest = argv[1];

	while (*forest != '\0') {
		if (*forest == 'F')
			burned_trees_counter++;
		forest++;
	}

	exit(burned_trees_counter); 
}