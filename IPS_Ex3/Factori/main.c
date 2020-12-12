

/* 
-----------------------------------------------------

* check if we should continue thread termination if get_exit_code_thread fails







// -----------------------------------------------------
*/
// include headers --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "win_api_wrappers.h"
#include "error_mgr.h"
#include "Queue.h"

//#include "thread_mgr.h"

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

void parse_data_from_cmd(char* cmd_data[], char* tasks_path, char* priority_path, int* threads_number, int* tasks_number);
error_code_t set_file_paths(char** p_input_path, char** p_output_path, char* input_path_main_arg, bool to_decrypt); 
error_code_t check_args_num(int argc); 
void free_main_resources(queue* my_queue);
error_code_t check_if_valid_args(int number_of_threads, char* encrypt_or_decrypt_flag);


int main(int argc, char* argv[])
{
	error_code_t status = SUCCESS_CODE;

	char* tasks_path = NULL;
	char* priorities_path = NULL;
	int tasks_num, threads_num;

	queue* priority_queue;

	status = InitializeQueue(&priority_queue);

	if (status != SUCCESS_CODE)
		goto exit_main;

	// make sure that we got the anticipated number of arguments
	status = check_args_num(argc, ARGS_NUM);

	if (status != SUCCESS_CODE)
		goto exit_main;
	
	// parse command line argumets 
	parse_data_from_cmd(argv, &tasks_path, &priorities_path, &tasks_num, &threads_num);

	// check if input args are valid: thread num > 0  && tasks_num > 0
	status = check_if_valid_args(threads_num, tasks_num);

	if (status != SUCCESS_CODE)
		goto exit_main;

	status = fill_priority_queue(priority_queue, priorities_path);

	if (status != SUCCESS_CODE)
		goto exit_main;









	//---------------------------

	printf("%s\n%s\n%d %d\n\n", tasks_path, priorities_path, tasks_num, threads_num);
	print_queue(priority_queue); 

	//---------------------------
	//// call cipher_thread_manager to create threads and encrypt / decrypt file 
	//status = cipher_thread_manager(number_of_threads, to_decrypt, key, input_path, output_path);
	
exit_main:
	free_main_resources(priority_queue);
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

/// check_if_valid_args
/// inputs:  threads_num ,  tasks_num
/// outputs: error code 
/// summary: check validation of threads_num and tasks_num( greater than 0 )  		 
error_code_t check_if_valid_args(int threads_num, int tasks_num)
{
	if ((threads_num <= 0 || threads_num > MAXIMUM_THREAD_NUM) || tasks_num < 0)
	{
		print_error(MSG_ERR_NOT_VALID_ARGS, __FILE__, __LINE__, __func__);
		return NOT_VALID_ARGS;
	}
	return SUCCESS_CODE;
}


/// free_main_resources
/// inputs:  input_path ,  output_path
/// outputs: none, void function 
/// summary:  free all main resources ( input_path , output_path ) 
void free_main_resources(queue* my_queue)  //char* input_path, char* output_path)
{
	//if (input_path != NULL)
	//	free(input_path);

	//if (output_path != NULL)
	//	free(output_path);
	if (my_queue != NULL)
		DestroyQueue(&my_queue);
}


