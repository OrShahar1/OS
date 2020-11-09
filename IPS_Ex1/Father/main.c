//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "forest_mgr.h"
#include "file_parser.h"

int main(int argc, char** argv) 
{
	if (argc != 2)
		print_error_and_exit(MSG_ERR_NUM_ARGS, __FILE__, __LINE__, __func__);

	FILE* f_output = fopen("output.txt","w");
	if (f_output == NULL)
		print_error_and_exit(MSG_ERR_CANNOT_OPEN_FILE, __FILE__, __LINE__, __func__);
	
	int side_len, gen_num;
	char* forest = NULL;
	
	forest = parser(argv[1], &side_len, &gen_num);
	run_iterations(forest, side_len, gen_num, f_output);

	free(forest);
	fclose(f_output); 

	return 0;
}

//-------------------------------------------------//
/*
* organize code - declarations, duplicate code (burn, grow, bury) etc.()
* add mode enum -> switch case for (burn, grow, bury)
* write to output (printf -> fprintf)
* add functions comments 
*/
//--------------------------------------------------//



















