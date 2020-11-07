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

void run_forest_iteration(char* forest, char* new_forest, int side_len);
char* burn_forest_trees(char* forest, char* new_forest, int side_len); 
char* get_forest_char(char* forest, int side_len, int row, int col);
void check_and_set_forest_char(char * forest, char* new_forest, int side_len, int row, int col , char check_char, char set_char);
void burn_neighbors (char* forest, char* new_forest, int side_len, int row, int col);
char* grow_forest_trees(char* forest, char* new_forest, int side_len); 
int count_nearby_trees(char* forest, int side_len, int row, int col);
char* bury_forest_trees(char* forest, char* new_forest, int side_len);  


const char* BURNED_TREES_COUNTER_PATH = "..\\Debug\\Son.exe"; 
const int NON_DIAGONAL_NEIGHBORS_OFFSET[8] = { 1,0,  0,1,	 -1,0,  0,-1 };
const int ALL_NEIGHBORS_OFFSET[16] = { 1,0,  0,1,  -1,0,  0,-1,  1,1,  -1,-1,  -1,1,  1,-1 };


char* get_forest_char(char * forest, int side_len, int row , int col)
{
	if (row >= side_len || col >= side_len || row < 0 || col < 0)
		return NULL;
	
	return forest + (side_len * row + col);
}

void check_and_set_forest_char(char * forest, char* new_forest, int side_len, int row, int col , char check_char, char set_char)
{
	char* p_cell = get_forest_char(forest, side_len, row, col);
	if (p_cell == NULL)
		return;

	if (*p_cell == check_char )
		*get_forest_char(new_forest, side_len, row , col) = set_char;

}

void print_forest(char* forest, int side_len)
{
	printf("~~~~~~~~~~~~~~\n");
	for (int row = 0; row < side_len; row++)
	{
		for (int col = 0; col < side_len; col++)
			printf("%c", *get_forest_char(forest, side_len, row, col));

		printf("\n");
	}

}


void run_iterations(char* forest, int side_len, int gen_num)
{
	char* new_forest = (char*)malloc(side_len * side_len + 1);
	strcpy(new_forest, forest);
	int burned_trees_num; 
	int i;
	//print_forest(forest, side_len);



	for (i = 0; i < gen_num; i++)
	{
		burned_trees_num = process_handler(BURNED_TREES_COUNTER_PATH, forest);

		if (burned_trees_num >= 0)
		{
			// valid Son exitcode 

			printf("%s - %d\n", forest, burned_trees_num);


			/*
			printf("main father (burned_trees_num) = %d\n", burned_trees_num);
			printf(" %d %d\n", side_len, gen_num);
			printf("%s\n", forest);
			*/
			// fputs(f_output, "%s - %d\n", forest_str, burned_trees_counter)
		}

		run_forest_iteration(forest, new_forest, side_len);

		strcpy(forest, new_forest); 

	}

	free(new_forest);
	return forest;
}


void run_forest_iteration(char* forest, char* new_forest, int side_len)
{
	burn_forest_trees(forest, new_forest, side_len);
	grow_forest_trees(forest, new_forest, side_len);
	bury_forest_trees(forest, new_forest, side_len);

	return new_forest;
}

void burn_neighbors (char* forest, char* new_forest, int side_len, int row, int col)
{
	int row_offset, col_offset, i; 
	for ( i = 0; i < 8; i += 2)
	{
		row_offset = NON_DIAGONAL_NEIGHBORS_OFFSET[i];
		col_offset = NON_DIAGONAL_NEIGHBORS_OFFSET[i+1];
		check_and_set_forest_char(forest, new_forest, side_len,
			row + row_offset, col + col_offset, 'T', 'F');
	}
}

char* burn_forest_trees(char* forest, char* new_forest, int side_len)
{
	int row, col;
	char* p_cell;

	for (row = 0; row < side_len; row++)
	{
		for (col = 0; col < side_len; col++)
		{
			p_cell = get_forest_char(forest, side_len, row, col);

			if (*p_cell != 'F')
				continue;

			burn_neighbors(forest, new_forest, side_len, row, col);

		}
	}
	return new_forest;
}


int count_nearby_trees(char* forest, int side_len, int row, int col)
{
	int trees_counter = 0;
	int row_offset, col_offset, i;
	char* p_cell;
	for (i = 0; i < 16; i += 2)
	{
		row_offset = ALL_NEIGHBORS_OFFSET[i];
		col_offset = ALL_NEIGHBORS_OFFSET[i + 1];

		p_cell = get_forest_char(forest, side_len, row + row_offset , col + col_offset );

		if (p_cell == NULL)
			continue;

		if (*p_cell == 'T')
			trees_counter++;

	}
	
	return trees_counter;
}


char* grow_forest_trees(char* forest, char* new_forest, int side_len)
{
	int row, col;
	char* p_cell;
	int nearby_trees_num;
	for (row = 0; row < side_len; row++)
	{
		for (col = 0; col < side_len; col++)
		{
			p_cell = get_forest_char(forest, side_len, row, col);

			if (*p_cell != 'G')
				continue;

			nearby_trees_num=count_nearby_trees(forest, side_len, row, col);

			if (nearby_trees_num >= 2) 
				*(get_forest_char(new_forest, side_len, row, col)) = 'T';
			
		}
	}
	return new_forest; 
}


char* bury_forest_trees(char* forest, char* new_forest, int side_len)
{
	int row, col;
	char* p_cell;
	int nearby_trees_num;
	for (row = 0; row < side_len; row++)
	{
		for (col = 0; col < side_len; col++)
		{
			p_cell = get_forest_char(forest, side_len, row, col);

			if (*p_cell != 'F')
				continue;

			*(get_forest_char(new_forest, side_len, row, col)) = 'G';


		}
	}
	return new_forest;
}


