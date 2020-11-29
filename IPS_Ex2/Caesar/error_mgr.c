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
const char* MSG_ERR_GET_THREAD_EXIT_CODE_FAILED = "GetExitCodeThread failed.";
const char* MSG_ERR_INVALID_HANDLE_VALUE = "handle failed.";
const char* MSG_ERR_CREATE_THREAD = "Thread creation failed.";
const char* MSG_ERR_SET_FILE_POINTER_FAILED = "SetFilePointer failed";
const char* MSG_ERR_THREADS_INPUTS_INITALIZATION_FAILED = "Threads inputs initalization failed";
const char* MSG_ERR_THREADS_WAIT_TIMEOUT = " Timeout while waiting for threads to return"; 
const char* MSG_ERR_THREADS_TERMINATION_FAILED = "Threads Termination failed";
const char* MSG_ERR_FILE_READING_FAILED = "File reading failed."; 
const char* MSG_ERR_FILE_WRITING_FAILED = "File Writing failed.";
const char* MSG_ERR_SEMAPHORE_CREATION_FAILED = "Create Semaphore failed."; 
const char* MSG_ERR_SEMAPHORE_WAIT_TIMEOUT = " Timeout while waiting for semaphore to return"; 


/// print_error
/// inputs:  error msg, file, line and function name where the error happended
/// outputs: -
/// summary: prints an error message
void print_error(const char* msg, const char* file, int line, const char* func_name)
{
    printf("ERROR: %s\n", msg);
    printf("File: %s\n", file);
    printf("Line: %d, Function: %s\n", line, func_name);
}
