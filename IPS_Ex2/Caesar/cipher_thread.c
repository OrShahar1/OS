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
void free_cipher_thread_resources(HANDLE* thread_input_file, HANDLE* thread_output_file); 

// function implementations ------------------------------------------------------
int WINAPI cipher_thread(LPVOID Argument)
{
    cipher_thread_input* thread_input = (cipher_thread_input*)Argument;
    error_code_t status = SUCCESS_CODE;
 
    DWORD wait_result;
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
    
    dwPtr_in = SetFilePointer(thread_input_file, thread_input->line_block_limits.start, NULL, FILE_BEGIN);
    dwPtr_out = SetFilePointer(thread_output_file, thread_input->line_block_limits.start, NULL, FILE_BEGIN);

    if (dwPtr_in == INVALID_SET_FILE_POINTER || dwPtr_out == INVALID_SET_FILE_POINTER)
    {
        print_error(MSG_ERR_SET_FILE_POINTER_FAILED, __FILE__, __LINE__, __func__);
        status = SET_FILE_POINTER_FAILED;
        goto cipher_thread_exit;
    }

    block_length = (int)(thread_input->line_block_limits.end - thread_input->line_block_limits.start);

    status = thread_cipher_execute(thread_input_file, thread_output_file,
        thread_input->to_decrypt, thread_input->key, block_length);

cipher_thread_exit:
    free_cipher_thread_resources(&thread_input_file, &thread_output_file); 

    return (int)status; 
}

error_code_t thread_cipher_execute(HANDLE thread_input_file, HANDLE thread_output_file, bool to_decrypt, int key, int block_length)
{
    error_code_t status = SUCCESS_CODE;
    char* line_buffer = NULL;
    int  index = 0;
    DWORD dwBytesRead;
    DWORD dwBytesWritten;
    BOOL return_code;

    line_buffer = (char*)malloc(block_length + 1);

    if (line_buffer == NULL)
    {
        print_error(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__);
        return MEM_ALLOC_ERROR;
    }
    return_code = ReadFile(thread_input_file, line_buffer, block_length, &dwBytesRead, NULL);

    if (return_code == false)
    {
        print_error(MSG_ERR_FILE_READING_FAILED, __FILE__, __LINE__, __func__);
        status = FILE_READING_FAILED;
        goto thread_cipher_execute_exit;
    }

    line_buffer[block_length] = '\0';

    line_buffer = line_cipher_execute(line_buffer, to_decrypt, key);

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

void free_cipher_thread_resources(HANDLE* p_thread_input_file, HANDLE* p_thread_output_file)
{
    if (*p_thread_input_file != INVALID_HANDLE_VALUE)
        CloseHandle(*p_thread_input_file);

    if (*p_thread_output_file != INVALID_HANDLE_VALUE)
        CloseHandle(*p_thread_output_file);
}