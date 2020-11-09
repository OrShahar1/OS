//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h> 
#include "common.h"
#include "forest_mgr.h" 
#include "process_mgr.h"
#include "file_parser.h"

typedef enum generation_step { BURN, GROW, BURY } gen_step;

const char* BURNED_TREES_COUNTER_PATH = "..\\Debug\\Son.exe"; 

const int NON_DIAGONAL_NEIGHBORS_OFFSET[4][2] = { { 1,0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
const int NON_DIAGONAL_NEIGHBORS_NUM = 4;

const int ALL_NEIGHBORS_OFFSET[8][2] = { {1,0}, { 0,1},  {-1,0},  {0,-1},  {1,1},  {-1,-1},  {-1,1}, { 1,-1} }; 
const int ALL_NEIGHBORS_NUM = 8;

void run_iterations(char* forest, int side_len, int gen_num, FILE * f_output);
char* run_forest_iteration(char* forest, char* new_forest, int side_len);
char* forest_trees_step(char* forest, char* new_forest, int side_len, gen_step step);
void execute_step(char* forest, char* new_forest, int side_len, gen_step step, int row, int col, char* p_cell);
void burn_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell);
void grow_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell);
void* bury_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell);
void burn_neighbors(char* forest, char* new_forest, int side_len, int row, int col);
int count_nearby_trees(char* forest, int side_len, int row, int col);
char* get_forest_char_pointer(char* forest, int side_len, int row, int col);
void check_and_set_forest_char(char* forest, char* new_forest, int side_len, int row, int col, char check_char, char set_char);


void run_iterations(char* forest, int side_len, int gen_num, FILE* f_output)
{
	char* new_forest = (char*)malloc(side_len * side_len + 1);
	int burned_trees_num, i;

	strcpy(new_forest, forest);

	for (i = 0; i < gen_num; i++)
	{
		burned_trees_num = process_handler(BURNED_TREES_COUNTER_PATH, forest);

		if (burned_trees_num < 0)
			print_error_and_exit(MSG_ERR_INVALID_EXITCODE, __FILE__, __LINE__, __func__);

		fprintf(f_output, "%s - %d", forest, burned_trees_num);

		// if last iteration do not run another generation 
		if (i == gen_num - 1)
			break;

		fprintf(f_output, "\n");
		run_forest_iteration(forest, new_forest, side_len);

		strcpy(forest, new_forest);
	}

	free(new_forest);
	return forest;
}

char* run_forest_iteration(char* forest, char* new_forest, int side_len)
{	
	new_forest = forest_trees_step(forest, new_forest, side_len, BURN);
	new_forest = forest_trees_step(forest, new_forest, side_len, GROW);
	new_forest = forest_trees_step(forest, new_forest, side_len, BURY);

	return new_forest;
}

char* forest_trees_step(char* forest, char* new_forest, int side_len, gen_step step)
{
	int row, col;
	char* p_cell;
	
	for (row = 0; row < side_len; row++)
	{
		for (col = 0; col < side_len; col++)
		{
			p_cell = get_forest_char_pointer(forest, side_len, row, col);

			execute_step(forest, new_forest, side_len, step, row, col, p_cell);
		}
	}
	return new_forest;
}

void execute_step(char* forest, char* new_forest, int side_len, gen_step step, int row, int col, char* p_cell)
{
	switch (step)
	{
	case BURN:
		burn_forest_trees(forest, new_forest, side_len, row, col, p_cell);
		break;

	case GROW:
		grow_forest_trees(forest, new_forest, side_len, row, col, p_cell);
		break;

	case BURY:
		bury_forest_trees(forest, new_forest, side_len, row, col, p_cell);
		break;
	}
}

void burn_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell)
{
	if (*p_cell != 'F')
		return;

	burn_neighbors(forest, new_forest, side_len, row, col);
}

void grow_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell)
{
	if (*p_cell != 'G')
		return;

	int nearby_trees_num = count_nearby_trees(forest, side_len, row, col);

	if (nearby_trees_num >= 2)
		*(get_forest_char_pointer(new_forest, side_len, row, col)) = 'T';

}

void* bury_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell)
{
	if (*p_cell != 'F')
		return;

	*(get_forest_char_pointer(new_forest, side_len, row, col)) = 'G';
}

void burn_neighbors(char* forest, char* new_forest, int side_len, int row, int col)
{
	int row_offset, col_offset, i;
	for (i = 0; i < NON_DIAGONAL_NEIGHBORS_NUM; i ++)
	{
		row_offset = NON_DIAGONAL_NEIGHBORS_OFFSET[i][0];
		col_offset = NON_DIAGONAL_NEIGHBORS_OFFSET[i][1];
		check_and_set_forest_char(forest, new_forest, side_len,
			row + row_offset, col + col_offset, 'T', 'F');
	}
}

int count_nearby_trees(char* forest, int side_len, int row, int col)
{
	int trees_counter = 0;
	int row_offset, col_offset, i;
	char* p_cell;
	for (i = 0; i < ALL_NEIGHBORS_NUM; i ++)
	{
		row_offset = ALL_NEIGHBORS_OFFSET[i][0];
		col_offset = ALL_NEIGHBORS_OFFSET[i][1];

		p_cell = get_forest_char_pointer(forest, side_len, row + row_offset , col + col_offset );

		if (p_cell == NULL)
			continue;

		if (*p_cell == 'T')
			trees_counter++;
	}
	
	return trees_counter;
}

char* get_forest_char_pointer(char* forest, int side_len, int row, int col)
{
	if (row >= side_len || col >= side_len || row < 0 || col < 0)
		return NULL;

	return forest + (side_len * row + col);
}

void check_and_set_forest_char(char* forest, char* new_forest, int side_len, int row, int col, char check_char, char set_char)
{
	char* p_cell = get_forest_char_pointer(forest, side_len, row, col);
	if (p_cell == NULL)
		return;

	if (*p_cell == check_char)
		*(get_forest_char_pointer(new_forest, side_len, row, col)) = set_char;

}

