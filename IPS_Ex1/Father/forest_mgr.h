#ifndef FOREST_MGR_H
#define FOREST_MGR_H


typedef struct _Forest {
	char* forest_cells;
	int side_len;

}Forest;

void run_iterations(char* forest, int side_len, int gen_num);

#endif
