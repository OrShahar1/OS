#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_mgr.h"
#include "cipher_thread.h"



//  constants -----------------------------------------------------------------

static const int WAIT_FOR_THREADS_TIME = 5000;


// function declarations ------------------------------------------------------

error_code_t create_thread(LPTHREAD_START_ROUTINE StartAddress, LPVOID ParameterPtr, LPDWORD ThreadIdPtr, HANDLE* p_thread_handle); 
BOOL get_thread_block_limits( block_limits* FirstRangePtr,  block_limits* SecondRangePtr );
BOOL initialize_thread_inputs(cipher_thread_input* thread_inputs[], int  threads_num, bool to_decrypt, int key, const char* input_path, const char* output_path );
void initialize_thread_input_fields(cipher_thread_input* thread_input, bool to_decrypt, int key, const char* input_path, const char* output_path);
error_code_t initialize_threads_block_limits(cipher_thread_input* thread_inputs, HANDLE* thread_input_file);
error_code_t check_wait_for_objects_result(DWORD wait_result); 
void free_threads_resources(HANDLE* thread_handles, cipher_thread_input* thread_inputs, int threads_num);

// function implementations ------------------------------------------------------


error_code_t cipher_thread_manager(int threads_num, bool to_decrypt, int key, const char* input_path, const char* output_path)
{
    error_code_t status = SUCCESS_CODE;
    cipher_thread_input* thread_inputs = NULL;
    HANDLE* thread_handles = NULL;

    DWORD  wait_result;
    LPVOID thread_input;
    int thread_idx;

    thread_handles = (HANDLE*)malloc(threads_num * sizeof(HANDLE));

    if (thread_handles == NULL)
    {
        status = MEM_ALLOC_ERROR;
        goto threads_cleanup;
    }
  
    status = initialize_thread_inputs(&thread_inputs, threads_num, to_decrypt, key, input_path, output_path);

    if (status != SUCCESS_CODE)
        goto threads_cleanup;
    
    for (thread_idx = 0; thread_idx < threads_num; thread_idx++ )
    {        
        //thread_inputs[thread_idx].mutex_handle = mutex_handle;
        thread_input = (LPVOID) & (thread_inputs[thread_idx]);

        status = create_thread((LPTHREAD_START_ROUTINE)cipher_thread, thread_input,
                                                    NULL, &(thread_handles[thread_idx]));
        if ( status != SUCCESS_CODE)
            goto threads_cleanup;

    }

    // release semaphore <-- threads_num

    wait_result = WaitForMultipleObjects(threads_num, thread_handles, TRUE, WAIT_FOR_THREADS_TIME); // ( TRUE --> wait for all 

    // add thread termination ability to the function 
    status = check_wait_for_objects_result(wait_result);

    if (status != SUCCESS_CODE)
        goto threads_cleanup;

threads_cleanup:
    // TODO - close everything    release resources   

    free_threads_resources(thread_handles, thread_inputs, threads_num);

    return status;
}


error_code_t create_thread(LPTHREAD_START_ROUTINE StartAddress, LPVOID ParameterPtr, LPDWORD ThreadIdPtr, HANDLE* p_thread_handle)
{
    HANDLE thread_handle = CreateThread(NULL,             /*  default security attributes */
                                        0,                /*  use default stack size */
                                        StartAddress,     /*  thread function */
                                        ParameterPtr,     /*  argument to thread function */
                                        0,                /*  use default creation flags */
                                        ThreadIdPtr);    /*  returns the thread identifier */

    if (thread_handle == NULL)
    {
        print_error(MSG_ERR_CREATE_THREAD, __FILE__, __LINE__, __func__);
        return THREAD_CREATION_FAILED;
    }

    *p_thread_handle = thread_handle; 
     
    return SUCCESS_CODE;
}


error_code_t initialize_thread_inputs(cipher_thread_input* thread_inputs[], int  threads_num,
    bool to_decrypt, int key, const char* input_path, const char* output_path)
{
    int thread_index;
    error_code_t status = SUCCESS_CODE;

    // create thread_inputs array that holds all threads' input structs 
    (*thread_inputs) = (cipher_thread_input*)malloc(threads_num * sizeof(cipher_thread_input));

    // if malloc failed return with error
    if (*thread_inputs == NULL)
    {
        print_error(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__);
        return MEM_ALLOC_ERROR;
    }

    // initialize the thread_inputs fields 
    for (thread_index = 0; thread_index < threads_num; thread_index++)
    {
        initialize_thread_input_fields(thread_inputs[thread_index],
            to_decrypt, key, input_path, output_path);
    }

    status = initialize_threads_block_limits(*thread_inputs, input_path, threads_num);


initialize_thread_inputs_exit:
    return status;  
}



void initialize_thread_input_fields(cipher_thread_input* thread_input,
    bool to_decrypt, int key, const char* input_path, const char* output_path)
{
    thread_input->key = key;
    thread_input->input_path = input_path;
    thread_input->output_path = output_path;
    thread_input->to_decrypt = to_decrypt;
}


error_code_t initialize_threads_block_limits(cipher_thread_input* thread_inputs, const char* input_path, int threads_num)
{
    error_code_t status = SUCCESS_CODE;
    int lines_num, max_block_size, min_block_size;

    int thread_block_limit = 0;
    char char_input[1];
    DWORD dwBytesRead;

    // open file for creating threads line blocks
    HANDLE  thread_input_file = CreateFileA(input_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_TEMPORARY,
        NULL);


    // if file opening failed return with error
    if (thread_input_file == INVALID_HANDLE_VALUE)
    {
        print_error(MSG_ERR_HANDLE_VALUE, __FILE__, __LINE__, __func__);
        return FILE_OPEN_FAILED;
    }

    // get lines num
    // calc max and min block sizes
    // read file and get start and end in each block and fill line block limits in each thread input

    // ---------------------------------------------------------------
    // count lines in file
    // calculate min block size (min bs) and max block size (max bs) 
    // min bs = int(#L/#T), max bs = int(#L/#T) + 1
    // amount of max bs = #L % #T, amount of mix bs = #T - #L % #T
    // ----------------------------------------------------------------

    DWORD dwPtr = SetFilePointer(thread_input_file,
        0,
        NULL,
        FILE_BEGIN);

    if (dwPtr == INVALID_SET_FILE_POINTER)
    {
        print_error(MSG_ERR_SET_FILE_POINTER_FAILED, __FILE__, __LINE__, __func__);
        CloseHandle(thread_input_file);
        return SET_FILE_POINTER_FAILED;
    }


    do
    {
        if (ReadFile(thread_input_file, char_input, 1, &dwBytesRead, NULL) == FALSE)
            break;

        if (dwBytesRead == 0)
            break;

        thread_block_limit++;

    } while (true); // inifinte loop 

    thread_inputs->line_block_limits.start = 0;
    thread_inputs->line_block_limits.end = thread_block_limit;

    return SUCCESS_CODE;
}

error_code_t check_wait_for_objects_result(DWORD wait_result) 
{
    if (wait_result != WAIT_OBJECT_0) //if ( WaitRes == WAIT_FAILED )
    {
        print_error(MSG_ERR_THREADS_WAIT_TIMEOUT, __FILE__, __LINE__, __func__);
        return THREADS_WAIT_TIMEOUT;
    }
    
    return SUCCESS_CODE;
}


void free_threads_resources( HANDLE* thread_handles, cipher_thread_input* thread_inputs, int threads_num)
{
    int thread_idx;
    for (thread_idx = 0; thread_idx < threads_num; thread_idx++)
        CloseHandle(thread_handles[thread_idx]);

    free(thread_inputs);
    free(thread_handles);


}