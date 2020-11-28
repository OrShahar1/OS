#ifndef INIT_CIPHER_THREAD_INPUT_H
#define INIT_CIPHER_THREAD_INPUT_H
#include "error_mgr.h"
#include "stdbool.h"
#include "cipher_thread.h"

error_code_t initialize_thread_inputs(cipher_thread_input* thread_inputs[], HANDLE* p_thread_start_semaphore, int  threads_num,
	bool to_decrypt, int key, const char* input_path, const char* output_path );

#endif