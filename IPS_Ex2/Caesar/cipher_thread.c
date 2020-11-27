
#include <windows.h>
#include <stdio.h>          // delete this 
#include "cipher_thread.h"
#include "error_mgr.h"
#include "stdbool.h"


// function declarations ------------------------------------------------------


error_code_t cipher_execute(HANDLE* thread_input_file, HANDLE* output_file, int key, bool to_decrypt, block_limits* line_block_limits);
void free_cipher_thread_resources(HANDLE* thread_input_file, HANDLE* thread_output_file); 

// function implementations ------------------------------------------------------


error_code_t WINAPI cipher_thread(LPVOID Argument)
{
    cipher_thread_input* thread_input = (cipher_thread_input*)Argument;

    // wait for semaphore

    error_code_t status = SUCCESS_CODE;
    HANDLE  thread_input_file, thread_output_file;
    DWORD dwPtr_in, dwPtr_out;

    thread_input_file =
        CreateFileA(thread_input->input_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_TEMPORARY, NULL);

    thread_output_file =
        CreateFileA(thread_input->output_path, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (thread_input_file == INVALID_HANDLE_VALUE ||  thread_output_file == INVALID_HANDLE_VALUE)
    {
        print_error(MSG_ERR_HANDLE_VALUE, __FILE__, __LINE__, __func__);
        status = FILE_OPEN_FAILED;
        goto cipher_thread_exit;
    }

    // SET CORRECT FILES POINTER 
    
    dwPtr_in = SetFilePointer(thread_input_file, 0, NULL, FILE_BEGIN);
    dwPtr_out = SetFilePointer(thread_output_file, 0, NULL, FILE_BEGIN);

    if (dwPtr_in == INVALID_SET_FILE_POINTER || dwPtr_out == INVALID_SET_FILE_POINTER)
    {
        print_error(MSG_ERR_SET_FILE_POINTER_FAILED, __FILE__, __LINE__, __func__);
        status = SET_FILE_POINTER_FAILED;
        goto cipher_thread_exit;
    }
    
    status = cipher_execute(thread_input_file, thread_output_file,
        thread_input->to_decrypt, thread_input->key, &(thread_input->line_block_limits));

cipher_thread_exit:
    free_cipher_thread_resources(thread_input_file, thread_output_file); 

    return status;
}


error_code_t cipher_execute(HANDLE* thread_input_file, HANDLE* output_file, bool to_decrypt, int key, block_limits* line_block_limits)
{
    /*
    printf("%d , %d\n ", thread_input->line_block_limits.start, thread_input->line_block_limits.end);
    printf("%s\n %s \n", thread_input->input_path, thread_input->output_path);
    printf("%d , %d \n", thread_input->key, thread_input->to_decrypt);
    */

    error_code_t status = SUCCESS_CODE;
    char input_char, new_char;
    char* line_buffer;
    int str_len, index = 0;
    DWORD dwBytesRead;

    str_len = line_block_limits->end - line_block_limits->start;
    line_buffer = (char*)malloc(str_len + 1);

    if (ReadFile(thread_input_file, line_buffer, str_len, &dwBytesRead, NULL) == FALSE)
        status = FILE_READING_FAILED;
    line_buffer[str_len] = '\0';
    printf("%s", line_cipher_execute(line_buffer, to_decrypt, key));

    DWORD dwBytesWritten;
    BOOL ret_val = WriteFile(output_file, line_buffer, str_len, &dwBytesWritten, NULL);

    if (ret_val) {
        printf("%d bytes written.\n", dwBytesWritten);
    }

    free(line_buffer); 

    return status;
}

void free_cipher_thread_resources(HANDLE* thread_input_file, HANDLE* thread_output_file)
{
    if (thread_input_file != INVALID_HANDLE_VALUE)
        CloseHandle(thread_input_file);

    if (thread_output_file != INVALID_HANDLE_VALUE)
        CloseHandle(thread_output_file);
}