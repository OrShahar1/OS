// include headers --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_mgr.h"
#include "caesar_encryption_decryption.h"
#include "tests.h"
#include "thread_mgr.h"

// enums  ----------------------------------------------
typedef enum _arg_index{
	INPUT_PATH_INDEX = 1,
	KEY_INDEX,
	NUMBER_OF_THREADS_INDEX,
	ENCRYPT_DECRYPT_FLAG_INDEX,
	ARGS_NUM
} arg_index; 

// consts  ----------------------------------------------

# define ERROR_CODE -1
//# define OUTPUT_FILE_NAME "decrypted.txt"
static const char* OUTPUT_FILE_NAME = "decrypted.txt";
static const char* DECRYPTION_FLAG = "-d"; 
static const char* DIR_SEPERATOR_STR = "\\";

static const char DIR_SEPERATOR_CHAR = '\\';

// functions decleretions  ----------------------------------------------

int error_handler(const char* msg, const char* file, int line, const char* func_name, FILE* f_input);
void parse_data_from_cmd(char* cmd_data[], int* key, int* number_of_thread, bool* to_decrypt); 


error_code_t main(int argc, char* argv[])
{
	error_code_t status = SUCCESS_CODE;
	error_code_t return_code;

	char *input_path = NULL;
	char *output_path = NULL;

	int key, number_of_thread;
	bool to_decrypt;

	if (argc != ARGS_NUM)
	{
		print_error(MSG_ERR_NUM_ARGS, __FILE__, __LINE__, __func__);
		status = ARGS_NUM_ERROR;
		goto exit_main;
	}
	
	return_code = set_file_paths(&input_path, &output_path, argv[INPUT_PATH_INDEX]);

	if (return_code != SUCCESS_CODE)
	{
		status = return_code; 
		goto exit_main;
	}
	
	parse_data_from_cmd(argv, &key, &number_of_thread, &to_decrypt);

	return_code = cipher_thread_manager(number_of_thread, to_decrypt, key, input_path, output_path);
	/*
	printf("%s\n %s \n", input_path, output_path);
	printf("%d , %d, %d ", key, number_of_thread, to_decrypt);
	*/
	// read and decrypt/ / encrypt  with flag ( -d -e ) 

exit_main:
	if (input_path != NULL)
		free(input_path);

	if (output_path != NULL)
		free(output_path);

	return status;
}

void parse_data_from_cmd(char* cmd_data[], int* key, int* number_of_thread, bool* to_decrypt)
{
	*key = atoi(cmd_data[KEY_INDEX]);
	*number_of_thread = atoi(cmd_data[NUMBER_OF_THREADS_INDEX]);
	*to_decrypt = (strcmp(cmd_data[ENCRYPT_DECRYPT_FLAG_INDEX], DECRYPTION_FLAG) == 0) ? true : false;
}

error_code_t set_file_paths(char** input_path, char** output_path, char* input_path_main_arg)
{
	int input_path_len, output_path_len_max;

	input_path_len = strlen(input_path_main_arg) + 1;
	output_path_len_max = input_path_len + strlen(OUTPUT_FILE_NAME) + 1;

	// Note that input_path_len + strlen(OUTPUT_FILE_NAME) is bigger than the actual output path length
	// this is done for implemetation simplicity 

	(*input_path) = (char*)malloc(sizeof(char) * input_path_len);
	(*output_path) = (char*)malloc(sizeof(char) * output_path_len_max);

	if (*input_path == NULL || *output_path == NULL)
	{
		print_error(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__);
		return MEM_ALLOC_ERROR;
	}

	strcpy_s(*input_path, input_path_len, input_path_main_arg);
	strcpy_s(*output_path, output_path_len_max, input_path_main_arg);

	// get pointer to the last occurance of '\', this is where the directory name ends
	char* dir_path_end = strrchr(*output_path, DIR_SEPERATOR_CHAR);
	*dir_path_end = '\0';

	// concatenate the '\' and the output file name 
	strcat_s(*output_path, output_path_len_max, DIR_SEPERATOR_STR);
	strcat_s(*output_path, output_path_len_max, OUTPUT_FILE_NAME);

	return SUCCESS_CODE;
}

/*
int error_handler(const char* msg, const char* file, int line, const char* func_name, FILE* f_input)
{
	if (f_input != NULL)
		fclose(f_input);

	print_error(msg, file, line, func_name);

	return ERROR_CODE;
}
*/