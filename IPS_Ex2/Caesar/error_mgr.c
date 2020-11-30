
// include headers ------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "error_mgr.h"

// constants ------------------------------------------------------------------

/// error messages to be used for "print_error" 
const char* MSG_ERR_NUM_ARGS                       = "wrong amount of arguments.";
const char* MSG_ERR_NOT_VALID_ARGS                 = "program received invalid argument.";
const char* MSG_ERR_CANNOT_OPEN_FILE               = "couldn't open file";
const char* MSG_ERR_MEM_ALLOC                      = "memory allocation error.";
const char* MSG_ERR_GET_THREAD_EXIT_CODE_FAILED    = "GetExitCodeThread failed.";
const char* MSG_ERR_INVALID_HANDLE_VALUE           = "got invalid handle value.";
const char* MSG_ERR_CREATE_THREAD                  = "Thread creation failed.";
const char* MSG_ERR_SET_FILE_POINTER_FAILED        = "SetFilePointer failed";
const char* MSG_ERR_THREADS_WAIT_TIMEOUT           = " Timeout while waiting for threads to return"; 
const char* MSG_ERR_THREADS_TERMINATION_FAILED     = "Threads Termination failed";
const char* MSG_ERR_FILE_READING_FAILED            = "File reading failed."; 
const char* MSG_ERR_FILE_WRITING_FAILED            = "File writing failed.";
const char* MSG_ERR_SEMAPHORE_CREATION_FAILED      = "Creating semaphore failed."; 
const char* MSG_ERR_SEMAPHORE_WAIT_TIMEOUT         = " Timeout while waiting for semaphore to be released"; 

// functions implementations  ----------------------------------------------------

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

/// check_mem_alloc
/// inputs:  ptr , file, line and function name where the error happended
/// outputs: error code
/// summary: check ptr allocation & return to appropriate status 
error_code_t check_mem_alloc(void* ptr, const char* file, int line, const char* func_name)
{
    if (ptr == NULL) 
    {
        print_error(MSG_ERR_MEM_ALLOC, file, line, func_name);
        return MEM_ALLOC_ERROR;
    }
    return SUCCESS_CODE;
}