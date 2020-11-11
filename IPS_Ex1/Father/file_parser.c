//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il ) 

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h> 
#include "common.h"

/// the base to be used for converting char to int 
static const int BASE = 10;

FILE* read_num_from_file(FILE* f_input, int* num);
char* read_forest_from_file(FILE* f_input, int side_len);

/// parser
/// inputs:  f_input - pointer to input file 
///          p_side_len - pointer to number side_len
///          p_generations_num - pointer to number generations_num
/// outputs: char* - the initial forest
///          will updated p_side_len & p_generations_num with the correct numbers
///          extract values from first two lines in input.txt
/// summary: parses all data from input file 
/// 
char* parser(FILE* f_input, int *p_side_len, int *p_generations_num)
{
	char* init_forest; 

	f_input = read_num_from_file(f_input, p_side_len);
	
	f_input = read_num_from_file(f_input, p_generations_num);

	init_forest = read_forest_from_file(f_input, *p_side_len);
	
	return init_forest;
}

/// read_num_from_file
/// inputs:  f_input - pointer to file input.txt
///          num - pointer to number 
/// outputs: returns f_input
/// summary: sets num with the number that located in line 
/// 
FILE* read_num_from_file(FILE* f_input, int* num)
{
	*num = 0;
	char c = fgetc(f_input);

	while (c != '\n' && c != EOF)
	{
		*num = atoi(&c) + (*num) * BASE;
		c = fgetc(f_input);
	}

	return f_input;
}

/// read_forest_from_file
/// inputs:  f_input - pointer to file input.txt
///			 side_len - side length of the forest    
/// outputs: char* - the initial forset  
/// summary: read the initial forest from input file and return
///          the initial forest 
/// 
char* read_forest_from_file(FILE* f_input, int side_len)
{
	char* forest = (char*)malloc(side_len * side_len + 1);
	char* init_forest = forest; 

	if (forest == NULL)
	{
		 print_error(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__);
		 return NULL_ERROR_CODE; 
	}
		
	char forest_char = fgetc(f_input);

	while (forest_char != EOF)
	{
		if (forest_char != ',' && forest_char != '\n')
		{
			*forest = forest_char;
			forest++;
		}

		forest_char = fgetc(f_input);
	} 
	*forest = '\0';
	
	return init_forest;
}
