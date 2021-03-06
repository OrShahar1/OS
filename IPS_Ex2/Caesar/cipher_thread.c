#pragma warning( disable:6011 )
#pragma warning( disable:4028 )
#pragma warning( disable:4024 )
#pragma warning( disable:4047 )

#include <windows.h>
#include <stdio.h>           
#include "cipher_thread.h"
#include "error_mgr.h"
#include "stdbool.h"
#include "caesar_encryption_decryption.h"

// consts ---------------------------------------------------------------------
static const int WAIT_FOR_SEMAPHORE_TIME = 5000;

// function declarations ------------------------------------------------------
error_code_t thread_cipher_execute(HANDLE thread_input_file, HANDLE thread_output_file,
                                bool to_decrypt, int key, block_limits* line_block_limits); 
void free_cipher_thread_resources(HANDLE* p_thread_input_file, HANDLE* p_thread_output_file); 

// function implementations ------------------------------------------------------

/// cipher_thread
/// inputs:  Argument (struct that holds the thread's inputs) 
/// outputs: int (status) 
/// summary: main thread function , read file --> execute thread_cipher_execute --> write to file 
int WINAPI cipher_thread(LPVOID Argument)
{
    cipher_thread_input* thread_input = (cipher_thread_input*)Argument;
    error_code_t status = SUCCESS_CODE;
 
    DWORD wait_result;

    // wait for main thread to release semaphore, until then wait
    wait_result = WaitForSingleObject(*(thread_input->p_thread_start_semaphore), WAIT_FOR_SEMAPHORE_TIME);

    if (wait_result != WAIT_OBJECT_0)
    {
        print_error(MSG_ERR_SEMAPHORE_WAIT_TIMEOUT, __FILE__, __LINE__, __func__);
        status = SEMAPHORE_WAIT_TIMEOUT; 
        goto  cipher_thread_exit;
    }
  
    HANDLE thread_input_file, thread_output_file;
    DWORD dwPtr_in, dwPtr_out;
    int block_length;
    
    block_length = (int)(thread_input->line_block_limits.end - thread_input->line_block_limits.start);
    
    if (block_length == 0)
        return (int)status;
    
    // open files for reading / writing 
    thread_input_file =
        CreateFileA(thread_input->input_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_TEMPORARY, NULL);

    thread_output_file =
        CreateFileA(thread_input->output_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (thread_input_file == INVALID_HANDLE_VALUE ||  thread_output_file == INVALID_HANDLE_VALUE)
    {
        print_error(MSG_ERR_INVALID_HANDLE_VALUE, __FILE__, __LINE__, __func__);
        status = FILE_OPEN_FAILED;
        goto cipher_thread_exit;
    }

    // move file pointers to correct place 
    dwPtr_in = SetFilePointer(thread_input_file, thread_input->line_block_limits.start, NULL, FILE_BEGIN);
    dwPtr_out = SetFilePointer(thread_output_file, thread_input->line_block_limits.start, NULL, FILE_BEGIN);

    if (dwPtr_in == INVALID_SET_FILE_POINTER || dwPtr_out == INVALID_SET_FILE_POINTER)
    {
        print_error(MSG_ERR_SET_FILE_POINTER_FAILED, __FILE__, __LINE__, __func__);
        status = SET_FILE_POINTER_FAILED;
        goto cipher_thread_exit;
    }

    // encrypt / decrypt block 
    status = thread_cipher_execute(thread_input_file, thread_output_file,
                    thread_input->to_decrypt, thread_input->key, block_length);

cipher_thread_exit:
    free_cipher_thread_resources(&thread_input_file, &thread_output_file); 

    return (int)status; 
}

/// thread_cipher_execute
/// inputs:  thread_input_file , thread_output_file, to_decrypt , key , block_length
/// outputs: error code   
/// summary: execute thread. read file --> decrypt / encrypt block --> write to file  
error_code_t thread_cipher_execute(HANDLE thread_input_file, HANDLE thread_output_file, bool to_decrypt, int key, int block_length)
{
    error_code_t status = SUCCESS_CODE;
    char* line_buffer = NULL;
    int  index = 0;
    DWORD dwBytesRead;
    DWORD dwBytesWritten;
    BOOL return_code;

    line_buffer = (char*)malloc(block_length + 1);

    status = check_mem_alloc(line_buffer, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		return status;

    // read file ( block length bytes ) 
    return_code = ReadFile(thread_input_file, line_buffer, block_length, &dwBytesRead, NULL);

    if (return_code == false)
    {
        print_error(MSG_ERR_FILE_READING_FAILED, __FILE__, __LINE__, __func__);
        status = FILE_READING_FAILED;
        goto thread_cipher_execute_exit;
    }

    line_buffer[block_length] = '\0';

    // decrypt / encrypt specific line
    line_buffer = line_cipher_execute(line_buffer, to_decrypt, key);
    
    // write file ( block length bytes ) 
    return_code = WriteFile(thread_output_file, line_buffer, block_length, &dwBytesWritten, NULL);

    if (return_code == false)
    {
        print_error(MSG_ERR_FILE_WRITING_FAILED, __FILE__, __LINE__, __func__);
        status = FILE_WRITING_FAILED;
    }

thread_cipher_execute_exit:

    if (line_buffer != NULL)
        free(line_buffer);

    return status;
}

/// free_cipher_thread_resources
/// inputs:  p_thread_input_file , p_thread_output_file
/// outputs: -   
/// summary: release files handles 
void free_cipher_thread_resources(HANDLE* p_thread_input_file, HANDLE* p_thread_output_file)
{
    if (*p_thread_input_file != INVALID_HANDLE_VALUE)
        CloseHandle(*p_thread_input_file);

    if (*p_thread_output_file != INVALID_HANDLE_VALUE)
        CloseHandle(*p_thread_output_file);
}