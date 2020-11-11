//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#include <stdio.h>
#include <stdlib.h> 

/// error messages to be used for "print_error_and_exit"
const char* MSG_ERR_NUM_ARGS = "wrong amount of arguments (expected 2)";
const char* MSG_ERR_CANNOT_OPEN_FILE = "couldn't open file";
const char* MSG_ERR_MEM_ALLOC = "memory allocation error";
const char* MSG_ERR_PROCESS_CREATION_FAILED = "process creation failed."; 
const char* MSG_ERR_INVALID_EXITCODE = "invalid exit code number";
const char* MSG_ERR_GET_EXIT_CODE = "GetExitCodeProcess failed";

/// error code for generic pointer problem 
const void* ERROR_CODE_NULL = NULL;

/// error code for process errors
const int ERROR_CODE_PROCESS = -1;

/// print_error_and_exit
/// inputs:  error msg, file, line and function name where the error happended
/// outputs: -
/// summary: prints an error message and terminates program with -1 exitcode 
/// 
void* print_error_and_return_error_code(const char* msg, const char* file, int line, const char* func_name)
{
	printf("ERROR: %s\n", msg);
	printf("File: %s\n", file);
	printf("Line: %d, Function: %s\n", line, func_name);
	return (ERROR_CODE_NULL);
}

