//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#include <stdio.h>
#include <stdlib.h> 

const char* MSG_ERR_NUM_ARGS = "wrong amount of arguments (expected 2)";
const char* MSG_ERR_CANNOT_OPEN_FILE = "couldn't open file";
const char* MSG_ERR_MEM_ALLOC = "memory allocation error";
const char* MSG_ERR_PROCESS_CREATION_FAILED = "process creation failed."; 
const char* MSG_ERR_INVALID_EXITCODE = "invalid exit code number";


void print_error_and_exit(const char* msg, const char* file, int line, const char* func_name)
{
	printf("ERROR: %s\n", msg);
	printf("File: %s\n", file);
	printf("Line: %d, Function: %s\n", line, func_name);
	exit(-1); 
}

