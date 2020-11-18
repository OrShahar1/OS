// include headers --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "caesar_encryption_decryption.h"
#include "tests.h"
#include "thread_mgr.h"


// consts  ----------------------------------------------

# define ERROR_CODE -1

// functions decleretions  ----------------------------------------------

int error_handler(const char* msg, const char* file, int line, const char* func_name, FILE* f_input);
void parser_data_from_cmd(int* key, int* number_of_thread, char* flag_encrypt_or_decrypt, char* cmd_data[]);


int main(int argc, char* argv[]) 
{
	
	if (argc != 5)
		return error_handler(MSG_ERR_NUM_ARGS, __FILE__, __LINE__, __func__, NULL);
	
	FILE* f_input ;
	FILE* f_output;

	fopen_s(&f_input , argv[1], "r");
	if (NULL == f_input)
		return error_handler(MSG_ERR_CANNOT_OPEN_FILE, __FILE__, __LINE__, __func__, f_input);

	fopen_s(&f_output, "decrypted.txt" , "w");
	if (NULL == f_output)
		return error_handler(MSG_ERR_CANNOT_OPEN_FILE, __FILE__, __LINE__, __func__, f_output);

	int key, number_of_thread;
	bool to_decrypt; 

	parser_data_from_cmd(&key, &number_of_thread, &to_decrypt, argv);

	cipher_exe(f_input, f_output, to_decrypt,key, key, number_of_thread);

	//printf("%s , %d , %d, %c ", argv[1], key, number_of_thread, flag_encrypt_or_decrypt);
	// read and decrypt/ / encrypt  with flag ( -d -e ) 
	fclose(f_output);
	fclose(f_input);
	return 0;
}

void parser_data_from_cmd(int* key, int* number_of_thread, bool* to_decrypt, char* cmd_data[])
{
	*key = atoi(cmd_data[2]);
	*number_of_thread = atoi(cmd_data[3]);
	*to_decrypt = (cmd_data[4][1] == 'd')? true : false;
}


int error_handler(const char* msg, const char* file, int line, const char* func_name, FILE* f_input)
{
	if (f_input != NULL)
		fclose(f_input);

	print_error(msg, file, line, func_name);

	return ERROR_CODE;
}