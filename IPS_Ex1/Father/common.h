#ifndef COMMON_H
#define COMMON_H

extern const char* MSG_ERR_NUM_ARGS;
extern const char* MSG_ERR_CANNOT_OPEN_FILE;
extern const char* MSG_ERR_MEM_ALLOC;
extern const char* MSG_ERR_PROCESS_CREATION_FAILED;

void print_error(const char* msg,  int file, int line, const char* func);

#endif
