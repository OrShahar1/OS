//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#include <stdio.h>
#include "..\\Father\\common.h"


void main(int argc, char** argv)
{
	if (argc != 2) 
		print_error_and_exit(MSG_ERR_NUM_ARGS, __FILE__, __LINE__, __func__);

	int burned_trees_counter = 0;

	char* forest = argv[1];

	while (*forest != '\0') 
	{
		if (*forest == 'F')
			burned_trees_counter++;

		forest++; 
	}

	exit(burned_trees_counter); 
}