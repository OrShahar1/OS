#ifndef COMMON_H
#define COMMON_H

extern const char* MSG_ERR_NUM_ARGS;
extern const char* MSG_ERR_CANNOT_OPEN_FILE;
extern const char* MSG_ERR_MEM_ALLOC;
extern const char* MSG_ERR_PROCESS_CREATION_FAILED;
extern const char* MSG_ERR_INVALID_EXITCODE;
extern const char* MSG_ERR_GET_EXIT_CODE;
extern const char* MSG_ERR_PROCESS_WAIT_FAILED;

extern const void* NULL_ERROR_CODE;
extern const int PROCESS_ERROR_CODE;

void print_error(const char* msg, const char* file, int line, const char* func_name);

#endif