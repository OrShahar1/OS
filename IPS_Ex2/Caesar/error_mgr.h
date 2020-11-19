#ifndef COMMON_H
#define COMMON_H

typedef enum { 
    ISP_SUCCESS, 
    ISP_FILE_OPEN_FAILED, 
    ISP_FILE_SEEK_FAILED,
    ISP_FILE_READING_FAILED,
    ISP_FTELL_FAILED,
    ISP_MUTEX_OPEN_FAILED,
    ISP_MUTEX_CREATE_FAILED,
    ISP_MUTEX_WAIT_FAILED,
    ISP_MUTEX_ABANDONED,
    ISP_MUTEX_RELEASE_FAILED,
    ISP_ILLEGAL_LETTER_WAS_READ
} ErrorCode_t;

extern const char* MSG_ERR_NUM_ARGS;
extern const char* MSG_ERR_CANNOT_OPEN_FILE;
extern const char* MSG_ERR_MEM_ALLOC;
extern const char* MSG_ERR_PROCESS_CREATION_FAILED;
extern const char* MSG_ERR_INVALID_EXITCODE;
extern const char* MSG_ERR_GET_EXIT_CODE;
extern const char* MSG_ERR_PROCESS_WAIT_FAILED;
extern const char* MSG_ERR_DIVISION_FAILED;
extern const char* MSG_ERR_CREATE_MUTEX;
extern const char* MSG_ERR_CREATE_TREAD;

extern const void* NULL_ERROR_CODE;
extern const int RETURN_CODE_ERROR;
extern const int RETURN_CODE_NORMAL;

void print_error(const char* msg, const char* file, int line, const char* func_name);

#endif