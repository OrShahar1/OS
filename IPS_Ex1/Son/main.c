// authors:
//		or shahar ( orshahar1@mail.tau.ac.il )
//		michaelz  ( zhitomirsky1@mail.tau.ac.il )

#include <stdio.h>

/// main
/// inputs:  argc - number of args 
///          argv - name of the .exe file & forest string
/// outputs: returns 0 
/// summary: return amount of 'F' in forest 
void main(int argc, char** argv)
{
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