//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#include <stdio.h>

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
	char** init_forest = NULL;
	int burned_trees_counter;
	
	init_forest = parser(argv[1], &side_len, &gen_num);

	
	for (int i = 0; i < gen_num; i++)
	{
		// run Son process   --->  burned_trees_counter
		// fputs(f_output, "%s - %d\n", forest_str, burned_trees_counter)
		run_forest_iteration(init_forest);
	}

	fclose(f_output); 
	return 0;
}


char** parser(const char* forest_file, int *p_side_len, int *p_gen_num )
{
	FILE* fp = fopen(forest_file, "r");

	if (fp == NULL)
		print_error(msg_err_cannot_open_file, __FILE__, __LINE__, __func__);

	fp = read_num_from_file(fp, p_side_len);

	fp = read_num_from_file(fp, p_gen_num);

	int side_len = *p_side_len;

	char** init_forest = (char**)malloc(side_len * (side_len + 1));

	if (init_forest == NULL)
		print_error(msg_err_mem_alloc, __FILE__, __LINE__, __func__);

	read_forest_from_file(fp, init_forest);

	close(fp);
}

FILE* read_num_from_file(FILE* fp, int *num) 
{
	char c;
	
	c = fgetc(fp);

	while (c != '\n' && c != EOF) {
		*num += atoi(c) + (*num) * 10;
	}

	return fp; 
}

void read_forest_from_file(FILE* fp, char** forest) 
{
	char buffer_char;

	do {
		buffer_char = fgetc(fp);

		if (buffer_char == EOF) {
			**forest = '\0';
			break;
		}

		if (buffer_char == '\n')
			**forest = '\0';
		else
			**forest = buffer_char; 
		
		forest++;
	}
	while (buffer_char != EOF);  
}

void print_error(const char* msg, const * file, int line, const char* func) {

	printf("ERROR: %s\n", msg);
	printf("File: %s, Line: %d, Function: %s\n", file, line, func);
	exit(-1); 
}















