//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 
//#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h> 


char* parser(const char* forest_file, int* p_side_len, int* p_gen_num);
FILE* read_num_from_file(FILE* fp, int* num);
FILE* read_forest_from_file(FILE* fp, char** forest);
void print_error(const char* msg, const* file, int line, const char* func);

const char* msg_err_num_args = "wrong amount of arguments (expected 2)";
const char* msg_err_cannot_open_file = " ";
const char* msg_err_mem_alloc = " ";




int main(int argc, char** argv) 
{
	if (argc != 2)
		print_error(msg_err_num_args, __FILE__, __LINE__, __func__);

	
	FILE* f_output = fopen("output.txt","w");
	if (f_output == NULL)
		print_error(msg_err_cannot_open_file, __FILE__, __LINE__, __func__);
	

	int side_len, gen_num;
	char* init_forest = NULL;
	int burned_trees_counter;
	
	 
	init_forest = parser(argv[1], &side_len, &gen_num);

	printf(" %d %d\n" ,side_len, gen_num);
	for (int i =0 ; i < side_len; i++, init_forest=init_forest + side_len+1)
		printf("%s\n", init_forest);
		
	/*
	for (int i = 0; i < gen_num; i++)
	{
		// run Son process   --->  burned_trees_counter
		// fputs(f_output, "%s - %d\n", forest_str, burned_trees_counter)
		run_forest_iteration(init_forest);
	}

	*/
	fclose(f_output); 
	return 0;
}



char* parser(const char* forest_file, int *p_side_len, int *p_gen_num)
{
	FILE* fp = fopen(forest_file, "r");

	if (fp == NULL)
		print_error(msg_err_cannot_open_file, __FILE__, __LINE__, __func__);

	fp = read_num_from_file(fp, p_side_len);
	
	fp = read_num_from_file(fp, p_gen_num);

	int side_len = *p_side_len;

	char* init_forest = (char*)malloc(side_len * (side_len + 1));

	if (init_forest == NULL)
		print_error(msg_err_mem_alloc, __FILE__, __LINE__, __func__);
	
	fp = read_forest_from_file(fp, init_forest);
	
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


FILE* read_forest_from_file(FILE* fp, char* forest)
{
	char forest_char;

	do {
		forest_char = fgetc(fp);

		if (forest_char == ',')
			continue;
	
	
		if (forest_char == EOF) 
		{
			*forest = '\0';
			break;
		}

		if (forest_char == '\n')
			*forest = '\0';
			
		else
			*forest = forest_char;
		

		forest += 1;
	

	} while (forest_char != EOF);

	return fp;
}


void print_error(const char* msg, const * file, int line, const char* func) {

	printf("ERROR: %s\n", msg);
	printf("File: %s, Line: %d, Function: %s\n", file, line, func);
	exit(-1); 
}















