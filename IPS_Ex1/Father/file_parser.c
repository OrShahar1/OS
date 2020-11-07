//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h> 
#include "common.h"


FILE* read_num_from_file(FILE* fp, int* num);
char* read_forest_from_file(FILE* fp, int side_len);

char* parser(const char* forest_file, int *p_side_len, int *p_gen_num)
{
	FILE* fp = fopen(forest_file, "r");
	char* init_forest; 
	if (fp == NULL)
		print_error(MSG_ERR_CANNOT_OPEN_FILE, __FILE__, __LINE__, __func__);

	fp = read_num_from_file(fp, p_side_len);
	
	fp = read_num_from_file(fp, p_gen_num);

	int side_len = *p_side_len;

	init_forest = read_forest_from_file(fp, *p_side_len);
	
	fclose(fp);

	return init_forest;
}

FILE* read_num_from_file(FILE* fp, int* num)
{
	char c;
	*num = 0;
	c = fgetc(fp);

	while (c != '\n' && c != EOF)
	{
		*num = atoi(&c) + (*num) * 10;
		c = fgetc(fp);
	}

	return fp;
}

char* read_forest_from_file(FILE* fp, int side_len)
{
	char* forest = (char*)malloc(side_len * side_len + 1);
	char* init_forest = forest; 

	if (forest == NULL)
		print_error(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__);

	char forest_char = fgetc(fp); 

	while (forest_char != EOF) {

		if (forest_char != ',' && forest_char != '\n')
		{
			*forest = forest_char;
			forest++;
		}
				
		forest_char = fgetc(fp);
	} 
	*forest = '\0';
	
	return init_forest;
}
