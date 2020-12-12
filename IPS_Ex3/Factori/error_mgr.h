#ifndef ERROR_MGR_H
#define ERROR_MGR_H

// enums ----------------------------------------------------------------------

typedef enum { 
    SUCCESS_CODE, 
    ARGS_NUM_ERROR,
    NOT_VALID_ARGS,
    MEM_ALLOC_ERROR,
    FILE_OPEN_FAILED, 
    FILE_READING_FAILED,
    FILE_WRITING_FAILED,
    SET_FILE_POINTER_FAILED,
    THREAD_CREATION_FAILED,
    THREADS_WAIT_TIMEOUT,
    GET_THREAD_EXIT_CODE_FAILED,
    THREADS_TERMINATION_FAILED,
    SEMAPHORE_CREATION_FAILED,
    OBJECT_WAIT_TIMEOUT,
    SET_END_OF_FILE_FAILED,
    CLOSE_HANDLE_FAILED
} error_code_t;

// constants ----------------------------------------------------------------------

extern const char* MSG_ERR_NUM_ARGS;
extern const char* MSG_ERR_NOT_VALID_ARGS; 
extern const char* MSG_ERR_CANNOT_OPEN_FILE;
extern const char* MSG_ERR_MEM_ALLOC;
extern const char* MSG_ERR_PROCESS_CREATION_FAILED;
extern const char* MSG_ERR_GET_THREAD_EXIT_CODE_FAILED;
extern const char* MSG_ERR_DIVISION_FAILED;
extern const char* MSG_ERR_CREATE_MUTEX;
extern const char* MSG_ERR_CREATE_THREAD;
extern const char* MSG_ERR_INVALID_HANDLE_VALUE;
extern const char* MSG_ERR_SET_FILE_POINTER_FAILED;
extern const char* MSG_ERR_THREADS_INPUTS_INITALIZATION_FAILED; 
extern const char* MSG_ERR_THREADS_WAIT_TIMEOUT;
extern const char* MSG_ERR_THREADS_TERMINATION_FAILED;
extern const char* MSG_ERR_FILE_READING_FAILED;
extern const char* MSG_ERR_FILE_WRITING_FAILED;
extern const char* MSG_ERR_SEMAPHORE_CREATION_FAILED;
extern const char* MSG_ERR_OBJECT_WAIT_TIMEOUT;
extern const char* MSG_ERR_SET_END_OF_FILE_FAILED;
extern const char* MSG_ERR_CLOSE_HANDLE_FAILED;
// function declarations ------------------------------------------------------

/// print_error
/// inputs:  error msg, file, line and function name where the error happended
/// outputs: -
/// summary: prints an error message
void print_error(const char* msg, const char* file, int line, const char* func_name);

/// check_mem_alloc
/// inputs:  ptr , file, line and function name where the error happended
/// outputs: -
/// summary: check ptr alloction & prints an error message
error_code_t check_mem_alloc(void* ptr, const char* file, int line, const char* func_name); 

/// check_args_num
/// inputs:  argc 
/// outputs: error code 
/// summary: checks if argc == 5 ( correct number of inputs ) 
error_code_t check_args_num(int argc, int anticipated_args_num);

#endif 