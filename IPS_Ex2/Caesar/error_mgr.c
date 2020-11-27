//authors:
//      or shahar ( orshahar1@mail.tau.ac.il )
//      michaelz  ( zhitomirsky1@mail.tau.ac.il )

#include <stdio.h>
#include <stdlib.h>
#include "error_mgr.h"

/// error messages to be used for "print_error"
const char* MSG_ERR_NUM_ARGS = "wrong amount of arguments (expected 4).";
const char* MSG_ERR_CANNOT_OPEN_FILE = "couldn't open file";
const char* MSG_ERR_MEM_ALLOC = "memory allocation error.";
const char* MSG_ERR_PROCESS_CREATION_FAILED = "process creation failed.";
const char* MSG_ERR_INVALID_EXITCODE = "invalid exit code number.";
const char* MSG_ERR_GET_EXIT_CODE = "GetExitCodeProcess failed.";
const char* MSG_ERR_PROCESS_WAIT_FAILED = "process wait failed.";
const char* MSG_ERR_HANDLE_VALUE = "handle failed.";
const char* MSG_ERR_CREATE_THREAD = "hadle failed.";
const char* MSG_ERR_SET_FILE_POINTER_FAILED = "SetFilePointer failed";
const char* MSG_ERR_THREADS_INPUTS_INITALIZATION_FAILED = "Threads inputs initalization failed";
const char* MSG_ERR_THREADS_WAIT_TIMEOUT = " Timeout while waiting for threads to return"; 


/// error code for generic pointer problems
const void* NULL_ERROR_CODE = NULL;


/// print_error
/// inputs:  error msg, file, line and function name where the error happended
/// outputs: -
/// summary: prints an error message and terminates program with -1 exitcode
void print_error(const char* msg, const char* file, int line, const char* func_name)
{
    printf("ERROR: %s\n", msg);
    printf("File: %s\n", file);
    printf("Line: %d, Function: %s\n", line, func_name);
}


// should we keep this? 
/*
error_code_t print_error_and_return_program_status(const char* msg, const char* file, int line, const char* func_name, error_code_t status)
{
    print_error(msg, file, line, func_name);
    return status; 
}
*/