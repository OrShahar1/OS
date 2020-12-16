
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
const char* MSG_ERR_OBJECT_WAIT_TIMEOUT            = "Timeout while waiting for semaphore to be released"; 
const char* MSG_ERR_SET_END_OF_FILE_FAILED         = "Set End of File failed."; 
const char* MSG_ERR_CLOSE_HANDLE_FAILED            = "CloseHandle Failed"; 
const char* MSG_ERR_MUTEX_CREATION_FAILED          = "Creating mutex failed.";  

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
/// outputs: error_code
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

/// check_args_num
/// inputs:  argc 
/// outputs: error_code 
/// summary: checks if argc == 5 ( correct number of inputs ) 
error_code_t check_args_num(int argc, int anticipated_args_num)
{
    if (argc == anticipated_args_num)
        return SUCCESS_CODE;

    print_error(MSG_ERR_NUM_ARGS, __FILE__, __LINE__, __func__);
    return ARGS_NUM_ERROR;
}

/// check_if_valid_args
/// inputs:  threads_num ,  tasks_num, maximum_threads_num
/// outputs: error_code 
/// summary: check validation of threads_num and tasks_num( greater than 0 )  		 
error_code_t check_if_valid_args(int threads_num, int tasks_num, const int maximum_threads_num)
{
    if ((threads_num <= 0 || threads_num > maximum_threads_num) || tasks_num < 0)
    {
        print_error(MSG_ERR_NOT_VALID_ARGS, __FILE__, __LINE__, __func__);
        return NOT_VALID_ARGS;
    }
    return SUCCESS_CODE;
}
