//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include "common.h"
#include "forest_mgr.h" 
#include "process_mgr.h"
#include "file_parser.h"

/// relative path to .exe file that counts burned trees in the forest 
static const char* BURNED_TREES_COUNTER_PATH = "..\\Debug\\Son.exe"; 

/// NON_DIAGONAL_NEIGHBORS_OFFSET - relatives direction from a specific cell without diagonal directions
/// len of NON_DIAGONAL_NEIGHBORS_NUM
static const int NON_DIAGONAL_NEIGHBORS_OFFSET[4][2] = { { 1,0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
static const int NON_DIAGONAL_NEIGHBORS_NUM = 4;

/// ALL_NEIGHBORS_OFFSET - all the possible relatives direction from a specific cell  
/// len of ALL_NEIGHBORS_OFFSET
static const int ALL_NEIGHBORS_OFFSET[8][2] = { {1,0}, { 0,1},  {-1,0},  {0,-1},  {1,1},  {-1,-1},  {-1,1}, { 1,-1} };
static const int ALL_NEIGHBORS_NUM = 8;

/// valid chars in forest 
static const char TREE_CHAR = 'T';
static const char FIRE_CHAR = 'F';
static const char GROUND_CHAR = 'G';

/// enum that contains all possible steps in a generation 
typedef enum generation_step { BURN, GROW, BURY } gen_step;

char* run_forest_iteration(char* forest, char* new_forest, int side_len);
char* forest_trees_step(char* forest, char* new_forest, int side_len, gen_step step);
void execute_step(char* forest, char* new_forest, int side_len, gen_step step, int row, int col, char* p_cell);
void burn_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell);
void grow_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell);
void bury_forest_trees(char* new_forest, int side_len, int row, int col, char* p_cell);
void burn_neighbors(char* forest, char* new_forest, int side_len, int row, int col);
int count_nearby_trees(char* forest, int side_len, int row, int col);
char* get_forest_char_pointer(char* forest, int side_len, int row, int col);
void check_and_set_forest_char(char* forest, char* new_forest, int side_len, int row, int col, char check_char, char set_char);

/// run_iterations (an iteration = one generation)
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///          side_len   - side length of the forest 
///			 gen_num    - number of generations we want 
///			 f_output - the file we write the output to
/// outputs: forest after all the generations    
/// summary: gets forest through all the generations,
///			 in each generation prints the forest and
///			 its amount of burned trees (as returned from the relevent process)
///			 to the output file 
/// 
char* run_iterations(char* forest, int side_len, int gen_num, FILE* f_output)
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

/// run_forest_iteration
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///          new_forest - next state of the forest
///          side_len   - side length of the forest 
/// outputs: new_forest (forest after one generation)
/// summary: calls forest_trees_step with the relevent steps in the generation
/// 
char* run_forest_iteration(char* forest, char* new_forest, int side_len)
{	
	new_forest = forest_trees_step(forest, new_forest, side_len, BURN);
	new_forest = forest_trees_step(forest, new_forest, side_len, GROW);
	new_forest = forest_trees_step(forest, new_forest, side_len, BURY);

	return new_forest;
}

/// forest_trees_step
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///          new_forest - next state of the forest
///          side_len   - side length of the forest 
///			 step       - the next step in a generation 
/// outputs: new_forest 
/// summary: iterates over the cells in the forest 
///			 and calls to execute_step to perform the next step in the generation 
///			(i.e. burn / grow / bury the relevent cells in new_forest)
/// 
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

/// execute_step
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///          new_forest - next state of the forest
///          side_len   - side length of the forest 
///			 step       - the next step in a generation 
///          row, col   - specifies a cell in the forest
///          p_cell     - pointer to cell in forest
/// outputs: - 
/// summary: calls the appropriate function according to step
/// 
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
		bury_forest_trees(new_forest, side_len, row, col, p_cell);
		break;
	}
}

/// burn_forest_trees
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///          new_forest - next state of the forest
///          side_len   - side length of the forest 
///          row, col   - specifies a cell in the forest
///          p_cell     - pointer to cell in forest
/// outputs: - 
/// summary: if there is a fire char in p_cell, 
///			 it sets the suitable cells in new_forest to fire char 
/// 
void burn_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell)
{
	if (*p_cell != FIRE_CHAR)
		return;

	burn_neighbors(forest, new_forest, side_len, row, col);
}

/// grow_forest_trees
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///          new_forest - next state of the forest
///          side_len   - side length of the forest 
///          row, col   - specifies a cell in the forest
///          p_cell     - pointer to cell in forest
/// outputs:  -
/// summary:  if p_cell's nearby trees number bigger than 2, 
///			  it sets new_forest[row][col] with tree char  
///                   
void grow_forest_trees(char* forest, char* new_forest, int side_len, int row, int col, char* p_cell)
{
	if (*p_cell != GROUND_CHAR)
		return;

	int nearby_trees_num = count_nearby_trees(forest, side_len, row, col);

	if (nearby_trees_num >= 2)
		*(get_forest_char_pointer(new_forest, side_len, row, col)) = TREE_CHAR;

}

/// bury_forest_trees
/// inputs:  new_forest - next state of the forest
///          side_len   - side length of the forest 
///          row, col   - specifies a cell in the forest
///          p_cell     - pointer to cell in forest
/// outputs:  - 
/// summary: if there is a fire char in cell then it sets the suitable cell 
///			 in new_forest (new_forest[row][col]) with a ground char 
/// 
void bury_forest_trees(char* new_forest, int side_len, int row, int col, char* p_cell)
{
	if (*p_cell != FIRE_CHAR)
		return;

	*(get_forest_char_pointer(new_forest, side_len, row, col)) = GROUND_CHAR;
}

/// count_nearby_trees
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///			 new_forest - next state of the forest
///          side_len - side length of the forest 
///          row, col - specifies a cell in the forest
/// outputs: - 
/// summary: if there is a fire char in the cell then it sets all tree chars 
///		     around it to a fire char in the suitable cells in new_forest
///   
void burn_neighbors(char* forest, char* new_forest, int side_len, int row, int col)
{
	int row_offset, col_offset, i;
	for (i = 0; i < NON_DIAGONAL_NEIGHBORS_NUM; i ++)
	{
		row_offset = NON_DIAGONAL_NEIGHBORS_OFFSET[i][0];
		col_offset = NON_DIAGONAL_NEIGHBORS_OFFSET[i][1];
		check_and_set_forest_char(forest, new_forest, side_len,
			row + row_offset, col + col_offset, TREE_CHAR, FIRE_CHAR);
	}
}

/// count_nearby_trees
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///          side_len - side length of the forest 
///          row, col - specifies a cell in the forest
/// outputs: number of trees surrounding the cell 
/// summary: gets the forest and coordinates of specific cell 
///			 and returns the amount of trees that are neighbors 
///   
int count_nearby_trees(char* forest, int side_len, int row, int col)
{
	int trees_counter = 0;
	int row_offset, col_offset, i;
	char* p_cell;
	for (i = 0; i < ALL_NEIGHBORS_NUM; i ++)
	{
		row_offset = ALL_NEIGHBORS_OFFSET[i][0];
		col_offset = ALL_NEIGHBORS_OFFSET[i][1];

		p_cell = get_forest_char_pointer(forest, side_len, row + row_offset , col + col_offset);

		if (p_cell == NULL)
			continue;

		if (*p_cell == TREE_CHAR)
			trees_counter++;
	}
	
	return trees_counter;
}

/// get_forest_char_pointer
/// inputs:  forest   - original forest in the generation (before burn, grow, bury)
///          side_len - side length of the forest 
///          row, col - specifies a cell in the forest
/// outputs: a pointer to char 
/// summary: returns a pointer to a valid cell in the forest (forest[row][col]),
///			 if cell outside of forest matrix returns NULL 
///    
char* get_forest_char_pointer(char* forest, int side_len, int row, int col)
{
	if (row >= side_len || col >= side_len || row < 0 || col < 0)
		return NULL;

	return forest + (side_len * row + col);
}

/// check_and_set_forest_char
/// inputs:  forest     - original forest in the generation (before burn, grow, bury)
///          new_forest - next state of the forest
///          side_len   - side length of the forest 
///          row, col   - specifies a cell in the forest
///          check_char - a char to compare the char in the cell with 
///          set_char   - a char to set a cell in the new_forest to 
/// outputs:  -
/// summary: checks if a specific cell in forest equals to check_char, 
///			 if it does sets the suitable cell in new_forest to set_char
/// 
void check_and_set_forest_char(char* forest, char* new_forest, int side_len, int row, int col, char check_char, char set_char)
{
	char* p_cell = get_forest_char_pointer(forest, side_len, row, col);
	if (p_cell == NULL)
		return;

	if (*p_cell == check_char)
		*(get_forest_char_pointer(new_forest, side_len, row, col)) = set_char;

}

