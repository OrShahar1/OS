
// include headers --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include "error_mgr.h"
#include "factorization_thread.h"
#include "win_api_wrappers.h"

//  constants -----------------------------------------------------------------

static const int BRUTAL_TERMINATION_CODE = -1; 
static const int WAIT_FOR_THREADS_TIME = 5000;

// function declarations ------------------------------------------------------

error_code_t create_threads_and_wait(HANDLE* thread_handles, factorization_thread_input* thread_inputs, int threads_num); 
error_code_t check_threads_exit_code(HANDLE* thread_handles, int threads_num);
error_code_t free_threads_resources(HANDLE* thread_handles, factorization_thread_input* thread_inputs, int threads_num, error_code_t current_status);


// function implementations ------------------------------------------------------

/// 
/// inputs:  
/// outputs: 
/// summary: 
error_code_t factorization_threads_manager(int threads_num, const char* tasks_path, queue* priorities_queue, lock* resources_lock) 
{
    error_code_t status = SUCCESS_CODE;

    HANDLE* thread_handles = NULL;

    factorization_thread_input* thread_inputs = NULL;

    thread_handles = (HANDLE*)calloc(threads_num, sizeof(HANDLE));

    status = check_mem_alloc(thread_handles, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        goto factorization_threads_manager_exit;

    status = initialize_thread_inputs(&thread_inputs, threads_num, tasks_path, priorities_queue, resources_lock); 

    if (status != SUCCESS_CODE)
        goto factorization_threads_manager_exit;
    
    status = create_threads_and_wait(thread_handles , thread_inputs, threads_num); 

    if (status != SUCCESS_CODE)
        goto factorization_threads_manager_exit;

factorization_threads_manager_exit:

    status = free_threads_resources(thread_handles, thread_inputs, threads_num, status);

    return status;
}



/// initialize_thread_inputs
/// inputs:   
/// outputs: error_code 
/// summary: initialize thread_inputs structs & fields of each thread
error_code_t initialize_thread_inputs(factorization_thread_input* p_thread_inputs[], int threads_num,
                                      const char* tasks_path, queue* priorities_queue, lock* resources_lock) 
{
    error_code_t status = SUCCESS_CODE;
    factorization_thread_input* thread_inputs = NULL;
    int thread_index;

    // create thread_inputs array that holds all threads' input structs 
    thread_inputs = (factorization_thread_input*)malloc(threads_num * sizeof(factorization_thread_input));

    // if malloc failed return with error
    status = check_mem_alloc(thread_inputs, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;

    // initialize the thread_inputs fields 
    for (thread_index = 0; thread_index < threads_num; thread_index++)
    {
        thread_inputs[thread_index].tasks_path = tasks_path;
        thread_inputs[thread_index].priorities_queue = priorities_queue;
        thread_inputs[thread_index].resources_lock = resources_lock;
    }

    *p_thread_inputs = thread_inputs;

    return status;
}



/// create_threads_and_wait
/// inputs:  thread_handles, thread_inputs, threads_num
/// outputs: error code  
/// summary: creates cipher threads and waits for them to return
error_code_t create_threads_and_wait(HANDLE* thread_handles, factorization_thread_input* thread_inputs, int threads_num)
{
    error_code_t status = SUCCESS_CODE;

    LPVOID thread_input;
    int thread_index;
    DWORD  wait_code;

    for (thread_index = 0; thread_index < threads_num; thread_index++){
        thread_input = (LPVOID) & (thread_inputs[thread_index]);

        status = create_thread((LPTHREAD_START_ROUTINE)factorization_thread, thread_input,
                                    NULL, &(thread_handles[thread_index]));
        if (status != SUCCESS_CODE)
            return status;

    }

    //wait for all threads to return 
    wait_code = WaitForMultipleObjects(threads_num, thread_handles, TRUE, WAIT_FOR_THREADS_TIME); 

    // make sure there was no wait timeout, if there was terminate threads and return error code
    status = check_wait_code_and_terminate_threads(wait_code, thread_handles, threads_num, BRUTAL_TERMINATION_CODE, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;

    // make sure the threads didn't return errors 
    status = check_threads_exit_code(thread_handles, threads_num);

    return status;
}


/// check_threads_exit_code
/// inputs:  thread_handles ,  threads_num
/// outputs: error code  
/// summary: checks if threads exit codes were SUCCESS_CODE, if not --> return error 
error_code_t check_threads_exit_code(HANDLE* thread_handles, int threads_num)
{
    error_code_t status = SUCCESS_CODE; 

    DWORD thread_exit_code;
    int thread_index;

    for (thread_index = 0; thread_index < threads_num; thread_index++)
    {
        status = get_exit_code_thread(thread_handles[thread_index], &thread_exit_code, __FILE__, __LINE__, __func__);

        if (status != SUCCESS_CODE)
             return status;
        
        if (thread_exit_code != SUCCESS_CODE)
            return thread_exit_code;
    }

    return SUCCESS_CODE;
}

/// free_threads_resources
/// inputs:  thread_inputs[] ,  p_thread_start_semaphore , thread_inputs , threads_num
/// outputs: - 
/// summary: free all threads' resources
error_code_t free_threads_resources(HANDLE* thread_handles, factorization_thread_input* thread_inputs, int threads_num, error_code_t current_status)
{
    error_code_t status = SUCCESS_CODE; 
    int thread_idx;

    if (thread_handles != NULL)
    {
        for (thread_idx = 0; thread_idx < threads_num; thread_idx++) 
        {
            status = close_handle(thread_handles[thread_idx], NULL, __FILE__, __LINE__, __func__);

            if (status != SUCCESS_CODE)
                current_status = status;
        }
        
        free(thread_handles);
    }

    if (thread_inputs != NULL)
        free(thread_inputs);

    return current_status;
}


////---------------------------------------------------------------------------------
////---------------------------------------------------------------------------------
//
//
/////// open_empty_output_file
/////// inputs:  output_path ,  file_size
/////// outputs: error code  
/////// summary: open new output file with the correct size 
////error_code_t open_empty_output_file(const char* output_path, int file_size)
////{
////    error_code_t status = SUCCESS_CODE;
////    HANDLE thread_output_file;
////    DWORD dwPtr;
////
////    thread_output_file = CreateFileA(output_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
////        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
////
////    dwPtr = SetFilePointer(thread_output_file, file_size, NULL, FILE_BEGIN);
////
////
////    SetEndOfFile(thread_output_file);
////
////open_empty_file_exit:
////
////        CloseHandle(thread_output_file);
////
////    return status;
////
////}
//
//
