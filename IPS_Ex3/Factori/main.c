
#pragma warning( disable:6001 )

// ------------------------------------------------------
// ------------------------------------------------------

// read lock before set file pointer?





//-------------------------------------------------------
// ------------------------------------------------------

// include headers --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "thread_mgr.h"
#include "error_mgr.h"
#include "Queue.h" 
#include "Lock.h" 


//#include "thread_mgr.h"
#include <math.h>
#include "helpers.h"

// enums  ----------------------------------------------

typedef enum _arg_index{
	TASK_FILE_INDEX = 1 ,
	PRIORITY_FILE_INDEX,
	TASKS_NUMBER_INDEX,
	THREADS_NUMBER_INDEX,
	ARGS_NUM
} arg_index; 

// consts  ----------------------------------------------

static const int MAXIMUM_THREAD_NUM = 64;

// functions declarations  ----------------------------------------------

error_code_t fill_priority_queue(queue* my_queue, char* priorities_path);
void parse_data_from_cmd(char* cmd_data[], char** tasks_path, char** priority_path, int* tasks_number, int* threads_number);
error_code_t free_main_resources(queue** p_my_queue, lock** p_my_lock, error_code_t current_status); 


int main(int argc, char* argv[])
{
	error_code_t status = SUCCESS_CODE;

	char* tasks_path = NULL;
	char* priorities_path = NULL;
	int tasks_num, threads_num;

	queue* priorities_queue = NULL;
	lock* resources_lock = NULL;

	// make sure that we got the anticipated number of arguments
	status = check_args_num(argc, ARGS_NUM);

	if (status != SUCCESS_CODE)
		goto exit_main;
	
	// parse command line argumets 
	parse_data_from_cmd(argv, &tasks_path, &priorities_path, &tasks_num, &threads_num);

	// check if input args are valid: thread num > 0  && tasks_num > 0
	status = check_if_valid_args(threads_num, tasks_num, MAXIMUM_THREAD_NUM);

	if (status != SUCCESS_CODE)
		goto exit_main;

	// initialize and fill priority queue
	status = InitializeQueue(&priorities_queue);

	if (status != SUCCESS_CODE)
		goto exit_main;

	status = fill_priority_queue(priorities_queue, priorities_path);

	if (status != SUCCESS_CODE)
		goto exit_main;

	// initialize and fill priority queue
	status = InitializeLock(&resources_lock);

	if (status != SUCCESS_CODE)
		goto exit_main;

	// call factorization_thread_manager to create threads and perform the tasks 
	status = factorization_threads_manager(threads_num, tasks_path, priorities_queue, resources_lock); 
	
exit_main:
	status = free_main_resources(&priorities_queue, &resources_lock, status);
	return (int)status;
}

error_code_t fill_priority_queue(queue* my_queue, char* priorities_path) 
{
	error_code_t status = SUCCESS_CODE; 

	HANDLE priorities_file = NULL;
	char* line = NULL;
	int line_length, current_priority;

	status = open_file(&priorities_file, priorities_path, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING,
														  __FILE__, __LINE__, __func__);
	if (status != SUCCESS_CODE)
		return status; 

	status = read_line(priorities_file, &line, &line_length);

	if (status != SUCCESS_CODE)
		goto fill_priority_queue_exit;

	while (line_length != 0)
	{
		current_priority = atoi(line);

		Push(my_queue, current_priority);

		status = read_line(priorities_file, &line, &line_length);

		if (status != SUCCESS_CODE)
			break;
	}

fill_priority_queue_exit:

	if(line != NULL)
		free(line);
	status = close_handle(priorities_file, INVALID_HANDLE_VALUE, __FILE__, __LINE__, __func__);

	return status; 
}

/// parse_data_from_cmd
/// inputs:  argv (command line input) and key, number_of_threads and to_decrypt pointers 
/// outputs: - 
/// summary: A function that fills the variables  key, number_of_threads and to_decrypt 
void parse_data_from_cmd(char* cmd_data[], char** tasks_path, char** priority_path, int* tasks_number, int* threads_number)
{
	*tasks_path      = cmd_data[TASK_FILE_INDEX];
	*priority_path   = cmd_data[PRIORITY_FILE_INDEX];

	*threads_number = atoi(cmd_data[THREADS_NUMBER_INDEX]);
	*tasks_number   = atoi(cmd_data[TASKS_NUMBER_INDEX]);
}

/// free_main_resources
/// inputs:  
/// outputs: 
/// summary:  free all main resources  
error_code_t free_main_resources(queue** p_my_queue, lock** p_my_lock, error_code_t current_status)
{
	error_code_t status = SUCCESS_CODE;

	if (*p_my_lock != NULL)
		status = DestroyLock(p_my_lock, status);

	if (*p_my_queue != NULL)
		DestroyQueue(p_my_queue);

	if (status != SUCCESS_CODE)
		current_status = status;

	return current_status; 
}







