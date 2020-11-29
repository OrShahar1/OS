#pragma warning( disable:6011 )
#pragma warning( disable:6387 )
#pragma warning( disable:4083 )
#pragma warning( disable:4090 )

// include headers --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_mgr.h"
#include "caesar_encryption_decryption.h"
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

static const char* DECRYPTION_OUTPUT_FILE_NAME = "decrypted.txt";
static const char* ENCRYPTION_OUTPUT_FILE_NAME = "encrypted.txt";
static const char DIR_SEPERATOR_CHAR = '\\';
static const char* DECRYPTION_FLAG = "-d";
static const char* ENCRYPTION_FLAG = "-e";

// functions declarations  ----------------------------------------------

void parse_data_from_cmd(char* cmd_data[], int* key, int* number_of_thread, bool* to_decrypt);
error_code_t set_file_paths(char** p_input_path, char** p_output_path, char* input_path_main_arg, bool to_decrypt); 
error_code_t check_args_num(int argc); 
void free_main_resources(char* input_path, char* output_path);
error_code_t check_if_valid_args(int number_of_threads, char* encrypt_or_decrypt_flag);

int main(int argc, char* argv[])
{
	error_code_t status = SUCCESS_CODE;

	char *input_path = NULL;
	char *output_path = NULL;

	int key, number_of_threads;
	bool to_decrypt;

	// make sure that we got the anticipated number of arguments
	status = check_args_num(argc);

	if (status != SUCCESS_CODE)
		goto exit_main;
	
	// parse command line argumets to get the encryption / decryption key,
	// number of threads and whether we sould encrypt or decrypt the input file 
	parse_data_from_cmd(argv, &key, &number_of_threads, &to_decrypt);

	// check if input args  are valid: thread num > 0 and e/d flag is "-d"/"-e"
	status = check_if_valid_args(number_of_threads, argv[ENCRYPT_DECRYPT_FLAG_INDEX]);

	if (status != SUCCESS_CODE)
		goto exit_main;

	// get the file input and output path from argv[INPUT_PATH_INDEX]
	status = set_file_paths(&input_path, &output_path, argv[INPUT_PATH_INDEX], to_decrypt);

	if (status != SUCCESS_CODE)
		goto exit_main;

	// call cipher_thread_manager to create threads and encrypt / decrypt file 
	status = cipher_thread_manager(number_of_threads, to_decrypt, key, input_path, output_path);

exit_main:
	free_main_resources(input_path, output_path);
	return (int)status;
}

/// parse_data_from_cmd
/// inputs:  argv (command line input) and key, number_of_threads and to_decrypt pointers 
/// outputs: - 
/// summary: A function that fills the variables  key, number_of_threads and to_decrypt 
void parse_data_from_cmd(char* cmd_data[], int* p_key, int* p_number_of_threads, bool* p_to_decrypt)
{
	*p_key = atoi(cmd_data[KEY_INDEX]);
	*p_number_of_threads = atoi(cmd_data[NUMBER_OF_THREADS_INDEX]);
	*p_to_decrypt = (strcmp(cmd_data[ENCRYPT_DECRYPT_FLAG_INDEX], DECRYPTION_FLAG) == 0);
}

/// set_file_paths
/// inputs:  p_input_path,  p_output_path , input_path_main_arg , to_decrypt
/// outputs: error code
/// summary: calculate input and output paths and store them in the addresses
///          which are held by p_input_path & p_output_path
error_code_t set_file_paths(char** p_input_path, char** p_output_path, char* input_path_main_arg, bool to_decrypt)
{
	error_code_t status = SUCCESS_CODE;
	int input_path_len, output_path_len_max;
	char* input_path, *output_path;
	char* output_file_name;

	// calculate input and output path lengths 
	// note that input_path_len + strlen(OUTPUT_FILE_NAME) is bigger than the actual output path length
	// this is done for implemetation simplicity 
	input_path_len = strlen(input_path_main_arg) + 1;
	output_path_len_max = input_path_len + strlen(DECRYPTION_OUTPUT_FILE_NAME) + 1;

	// allocate memory for paths strings
	input_path = (char*)malloc(sizeof(char) * input_path_len);

	status = check_mem_alloc(input_path, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		return status;

	output_path = (char*)malloc(sizeof(char) * output_path_len_max);

	status = check_mem_alloc(output_path, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		return status;

	// copy input_path_main_arg to both strings
	strcpy_s(input_path, input_path_len, input_path_main_arg);
	strcpy_s(output_path, output_path_len_max, input_path_main_arg);

	// get pointer to the last occurance of '\', this is where the directory name ends
	char* dir_path_end = strrchr(output_path, DIR_SEPERATOR_CHAR);

	// absolute path --> keep the directory name but not input file name
	// and concatenate the '\' and the output file name 
	if (dir_path_end != NULL)
		*(dir_path_end + 1) = '\0';

	// relative path --> just don't keep input file name 
	else *output_path = '\0';

	// decide what the output file name should be according to the flag
	// (-d / -e --> to_decrypt = true / false) 
	output_file_name = to_decrypt ? DECRYPTION_OUTPUT_FILE_NAME : ENCRYPTION_OUTPUT_FILE_NAME;
	strcat_s(output_path, output_path_len_max, output_file_name);

	// set path pointers 
	*p_input_path = input_path;
	*p_output_path = output_path;
	return status;
}

/// check_args_num
/// inputs:  argc 
/// outputs: error code 
/// summary: checks if argc == 5 ( correct number of inputs ) 
error_code_t check_args_num(int argc) 
{
	if (argc == ARGS_NUM)
		return SUCCESS_CODE;

	print_error(MSG_ERR_NUM_ARGS, __FILE__, __LINE__, __func__);
	return ARGS_NUM_ERROR;
}

/// check_if_valid_args
/// inputs:  number_of_threads ,  encrypt_or_decrypt_flag
/// outputs: error code 
/// summary: check validation of number_of_threads ( greater than 0 )  
///			 encrypt_or_decrypt_flag ( -e or -d )
error_code_t check_if_valid_args(int number_of_threads, char* encrypt_or_decrypt_flag)
{
	if ((strcmp(encrypt_or_decrypt_flag, DECRYPTION_FLAG) != 0 && 
		strcmp(encrypt_or_decrypt_flag, ENCRYPTION_FLAG) != 0) ||
		number_of_threads <= 0)
	{
		print_error(MSG_ERR_NUM_ARGS, __FILE__, __LINE__, __func__);
		return NOT_VALID_ARGS;
	}
	return SUCCESS_CODE;
}

/// free_main_resources
/// inputs:  input_path ,  output_path
/// outputs: none, void function 
/// summary:  free all main resources ( input_path , output_path ) 
void free_main_resources(char* input_path, char* output_path)
{
	if (input_path != NULL)
		free(input_path);

	if (output_path != NULL)
		free(output_path);
}


