#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_mgr.h"
#include "cipher_thread.h"
#include "init_cipher_thread_input.h"

//  constants -----------------------------------------------------------------

static const int WAIT_FOR_THREADS_TIME = 5000;
static const int BRUTAL_TERMINATION_CODE = -1; 

// function declarations ------------------------------------------------------

error_code_t create_thread(LPTHREAD_START_ROUTINE StartAddress, LPVOID ParameterPtr, LPDWORD ThreadIdPtr, HANDLE* p_thread_handle); 
error_code_t check_wait_for_objects_result(DWORD wait_result, HANDLE* thread_handles, int threads_num); 
void free_threads_resources(HANDLE* thread_handles, HANDLE *p_thread_start_semaphore, cipher_thread_input* thread_inputs, int threads_num);
error_code_t terminate_threads(HANDLE* thread_handles, int threads_num);

// function implementations ------------------------------------------------------

error_code_t cipher_thread_manager(int threads_num, bool to_decrypt, int key, const char* input_path, const char* output_path)
{
    error_code_t status = SUCCESS_CODE;

    cipher_thread_input* thread_inputs = NULL;
    LPVOID thread_input;
    HANDLE  thread_start_semaphore;
    HANDLE* thread_handles = NULL;
    DWORD  wait_code;
    int thread_idx;

    thread_start_semaphore = CreateSemaphore(NULL,  0,  threads_num, NULL); 

    if (thread_start_semaphore == NULL)
    {
        print_error(MSG_ERR_SEMAPHORE_CREATION_FAILED, __FILE__, __LINE__, __func__);
        return SEMAPHORE_CREATION_FAILED;
    }

    thread_handles = (HANDLE*)malloc(threads_num * sizeof(HANDLE));

    if (thread_handles == NULL)
    {
        print_error(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__);
        status = MEM_ALLOC_ERROR;
        goto cipher_thread_manager_exit;
    }

    status = initialize_thread_inputs(&thread_inputs, &thread_start_semaphore, threads_num, to_decrypt, key, input_path, output_path);
  
    if (status != SUCCESS_CODE)
        goto cipher_thread_manager_exit;

    status = open_empty_output_file(output_path, thread_inputs[threads_num - 1].line_block_limits.end);

    if (status != SUCCESS_CODE)
        goto cipher_thread_manager_exit;

    
    for (thread_idx = 0; thread_idx < threads_num; thread_idx++)
    {
        thread_input = (LPVOID) & (thread_inputs[thread_idx]);

        status = create_thread((LPTHREAD_START_ROUTINE)cipher_thread, thread_input,
                                    NULL, &(thread_handles[thread_idx]));
        if (status != SUCCESS_CODE)
            goto cipher_thread_manager_exit;

    }

    // release semaphore to start all the threads' execution 
    ReleaseSemaphore(thread_start_semaphore, threads_num, NULL);

    //wait for all threads to return 
    wait_code = WaitForMultipleObjects(threads_num, thread_handles, TRUE, WAIT_FOR_THREADS_TIME); 

    // add thread termination ability to the function 
    status = check_wait_code_and_terminate_threads(wait_code, thread_handles, threads_num);

    if (status != SUCCESS_CODE)
        goto cipher_thread_manager_exit;

    status = check_threads_exit_code(thread_handles, threads_num);

cipher_thread_manager_exit:
    // TODO - close everything  release resources   

    free_threads_resources(thread_handles, &thread_start_semaphore, thread_inputs, threads_num);

    return status;
}

error_code_t create_thread(LPTHREAD_START_ROUTINE StartAddress, LPVOID ParameterPtr, LPDWORD ThreadIdPtr, HANDLE* p_thread_handle)
{
    HANDLE thread_handle = CreateThread(NULL,             /*  default security attributes */
                                        0,                /*  use default stack size */
                                        StartAddress,     /*  thread function */
                                        ParameterPtr,     /*  argument to thread function */
                                        0,                /*  use default creation flags */
                                        ThreadIdPtr);     /*  returns the thread identifier */

    if (thread_handle == NULL)
    {
        print_error(MSG_ERR_CREATE_THREAD, __FILE__, __LINE__, __func__);
        return THREAD_CREATION_FAILED;
    }

    *p_thread_handle = thread_handle; 
     
    return SUCCESS_CODE;
}


error_code_t check_wait_code_and_terminate_threads(DWORD wait_result, HANDLE* thread_handles, int threads_num)
{
    error_code_t status = SUCCESS_CODE;

    if (wait_result != WAIT_OBJECT_0) 
    {
        print_error(MSG_ERR_THREADS_WAIT_TIMEOUT, __FILE__, __LINE__, __func__);
        status = terminate_threads(thread_handles, threads_num);
    }

    return status;
}

error_code_t terminate_threads(HANDLE* thread_handles, int threads_num)
{
    int thread_idx;
    BOOL return_code;
    DWORD thread_exit_code;

    for (thread_idx = 0; thread_idx < threads_num; thread_idx++)
    {
        return_code = GetExitCodeThread(thread_handles[thread_idx], &thread_exit_code);

        if (return_code == false)
        {
            print_error(MSG_ERR_GET_THREAD_EXIT_CODE_FAILED, __FILE__, __LINE__, __func__);
            return GET_THREAD_EXIT_CODE_FAILED;
        }

        if (thread_exit_code != STILL_ACTIVE)
            continue;

        return_code = TerminateThread(thread_handles[thread_idx], BRUTAL_TERMINATION_CODE);

        if (return_code == false)
        {
            print_error(MSG_ERR_THREADS_TERMINATION_FAILED, __FILE__, __LINE__, __func__);
            return THREADS_TERMINATION_FAILED;
        }
    }

    return THREADS_WAIT_TIMEOUT;
}

check_threads_exit_code(HANDLE* thread_handles, int threads_num) 
{
    int thread_index;
    BOOL return_code;
    DWORD thread_exit_code;
    for (int thread_idx = 0; thread_idx < threads_num; thread_idx++)
    {
        return_code = GetExitCodeThread(thread_handles[thread_idx], &thread_exit_code);

        if (return_code == false)
        {
            print_error(MSG_ERR_GET_THREAD_EXIT_CODE_FAILED, __FILE__, __LINE__, __func__);
            return GET_THREAD_EXIT_CODE_FAILED;
        }

        if (thread_exit_code != SUCCESS_CODE)
            return thread_exit_code;
        
    }
    return SUCCESS_CODE; 
}
error_code_t open_empty_output_file(const char* output_path, int file_size)
{
    error_code_t status = SUCCESS_CODE;
    HANDLE thread_output_file;
    DWORD dwPtr;

    thread_output_file = CreateFileA(output_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // if file opening failed return with error
    if (thread_output_file == INVALID_HANDLE_VALUE)
    {
        print_error(MSG_ERR_CANNOT_OPEN_FILE, __FILE__, __LINE__, __func__);
        return FILE_OPEN_FAILED;
    }

    dwPtr = SetFilePointer(thread_output_file, file_size, NULL, FILE_BEGIN);

    if (dwPtr == INVALID_SET_FILE_POINTER)
    {
        print_error(MSG_ERR_SET_FILE_POINTER_FAILED, __FILE__, __LINE__, __func__);
        status = SET_FILE_POINTER_FAILED;
        goto open_empty_file_exit;
    }

    SetEndOfFile(thread_output_file);

open_empty_file_exit:

    if (thread_output_file != INVALID_HANDLE_VALUE)
        CloseHandle(thread_output_file);

    return status;

}

void free_threads_resources( HANDLE* thread_handles, HANDLE* p_thread_start_semaphore, cipher_thread_input* thread_inputs, int threads_num)
{
    int thread_idx;
    for (thread_idx = 0; thread_idx < threads_num; thread_idx++)
        CloseHandle(thread_handles[thread_idx]);

    free(thread_inputs); 
    free(thread_handles);
    CloseHandle(*p_thread_start_semaphore);
}


