//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "forest_mgr.h"
#include "file_parser.h"

int error_handler(const char* msg, const char* file, int line, const char* func_name ,FILE* f_input, FILE* f_output);

/// main
/// inputs:  argc - number of args 
///          argv - name of the .exe file & forest input file
/// outputs: will return in exit code the number of 'F' in forest 
/// summary: calls the functions that read the input file and writes
///			 to the output file the current forest and its number of burned trees
///          for each generation 
/// 
int main(int argc, char** argv) 
{
	if (argc != 2)
		return error_handler (MSG_ERR_NUM_ARGS, __FILE__, __LINE__, __func__, NULL, NULL);

	FILE* f_input = fopen(argv[1], "r");
	FILE* f_output = fopen("output.txt","w");

	if (f_input == NULL || f_output == NULL)
		return error_handler(MSG_ERR_CANNOT_OPEN_FILE, __FILE__, __LINE__, __func__, f_input , f_output);
	
	int side_len, gen_num;
	char* forest = NULL;
	
	// read file and return initial forest
	forest = parser(f_input, &side_len, &gen_num);

	if (forest != NULL_ERROR_CODE)
		// advance forest to final state
		forest = run_iterations(forest, side_len, gen_num, f_output);

	if (forest != NULL_ERROR_CODE)
		free(forest);

	fclose(f_input);
	fclose(f_output); 

	return 0;
}

int error_handler(const char* msg, const char* file, int line, const char* func_name, FILE* f_input, FILE* f_output)
{
	if (f_input != NULL)
		fclose(f_input);

	if (f_output != NULL)
		fclose(f_output);

	print_error(msg, file, line, func_name);

	return -1;
}









