
#ifndef CIPHER_THREAD_H
#define CIPHER_THREAD_H

// include headers ------------------------------------------------------------

#include <windows.h>
#include "error_mgr.h"
#include "stdbool.h"

//  structs -------------------------------------------------------------------

typedef struct
{
    long int start;
    long int end;
} block_limits;

typedef struct {
    const char* input_path;
    const char* output_path;
    block_limits line_block_limits;
    HANDLE * p_thread_start_semaphore;

    bool to_decrypt;
    int key;
} cipher_thread_input;

// function declarations ------------------------------------------------------

/// cipher_thread
/// inputs:  Argument
/// outputs: int (status)   
/// summary: main thread function , read file -> execute thread_cipher_execute - > wirte to file 
error_code_t WINAPI cipher_thread(LPVOID Argument);

#endif