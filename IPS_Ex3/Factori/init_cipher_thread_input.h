#ifndef INIT_CIPHER_THREAD_INPUT_H
#define INIT_CIPHER_THREAD_INPUT_H

// include headers ------------------------------------------------------------

#include "error_mgr.h"
#include "stdbool.h"
#include "cipher_thread.h"

// function declarations ------------------------------------------------------

/// initialize_thread_inputs
/// inputs:  thread_inputs[] ,  p_thread_start_semaphore , threads_num , to_decrypt , key , input_path ,output_path
/// outputs: error code 
/// summary: initialize thread_inputs structs & fields of each thread
error_code_t initialize_thread_inputs(factorization_thread_input* thread_inputs[], HANDLE* p_thread_start_semaphore, int  threads_num,
	bool to_decrypt, int key, const char* input_path, const char* output_path );

#endif