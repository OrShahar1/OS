#ifndef COMMON_H
#define COMMON_H

typedef enum { 
    SUCCESS_CODE, 
    ARGS_NUM_ERROR,
    MEM_ALLOC_ERROR,
    FILE_OPEN_FAILED, 
    FILE_SEEK_FAILED,
    FILE_READING_FAILED,
    FTELL_FAILED,
    MUTEX_OPEN_FAILED,
    MUTEX_CREATE_FAILED,
    MUTEX_WAIT_FAILED,
    MUTEX_ABANDONED,
    MUTEX_RELEASE_FAILED,
    ILLEGAL_LETTER_WAS_READ,
    RETURN_CODE_ERROR,
} error_code_t;

extern const char* MSG_ERR_NUM_ARGS;
extern const char* MSG_ERR_CANNOT_OPEN_FILE;
extern const char* MSG_ERR_MEM_ALLOC;
extern const char* MSG_ERR_PROCESS_CREATION_FAILED;
extern const char* MSG_ERR_INVALID_EXITCODE;
extern const char* MSG_ERR_GET_EXIT_CODE;
extern const char* MSG_ERR_PROCESS_WAIT_FAILED;
extern const char* MSG_ERR_DIVISION_FAILED;
extern const char* MSG_ERR_CREATE_MUTEX;
extern const char* MSG_ERR_CREATE_THREAD;
extern const char* MSG_ERR_HANDLE_VALUE;
extern const char* MSG_ERR_SET_FILE_POINTER_FAILED;
extern const char* MSG_ERR_THREADS_INPUTS_INITALIZATION_FAILED;
void print_error(const char* msg, const char* file, int line, const char* func_name);

#endif