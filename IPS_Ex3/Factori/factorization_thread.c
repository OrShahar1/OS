
#include <windows.h>
#include <stdio.h>  

#include "factorization_thread.h"
#include "primes_handler.h"
#include "error_mgr.h"
#include "Queue.h"
#include "win_api_wrappers.h"


// consts ---------------------------------------------------------------------

// function declarations ------------------------------------------------------

error_code_t free_factorization_thread_resources(HANDLE thread_tasks_file, error_code_t current_status);

// function implementations ------------------------------------------------------


/// factorization_thread
/// inputs:  Argument
/// outputs: error_code
/// summary: main thread function: 
///          take task from queue -> read task from file -> perform the factorization -> write to file 
int WINAPI factorization_thread(LPVOID Argument)
{
    factorization_thread_input* thread_input = (factorization_thread_input*)Argument;
    error_code_t status = SUCCESS_CODE;
  
    HANDLE tasks_file;

    printf("%s\n", thread_input->tasks_path);

    // open tasks file for reading / writing 
    status = open_file(&tasks_file, thread_input->tasks_path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, OPEN_EXISTING,
                       __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;

    status = thread_factorization_execute(tasks_file, thread_input->priorities_queue);
    
factorization_thread_exit:
    status = free_factorization_thread_resources(tasks_file, status);

    return (int)status; 
}

/// thread_factorization_execute
/// inputs:  
/// outputs: error code   
/// summary: execute the thread functionality:  
///          take task from queue -> read task from file -> perform the factorization -> write to file 
error_code_t thread_factorization_execute(HANDLE tasks_file, queue* priorities_queue)
{
    error_code_t status = SUCCESS_CODE;
    char* task_line_buffer = NULL;
    char* primes_string_buffer = NULL;
    int task, task_position, task_line_length;
    
    while (true)
    {
        // lock queue mutex 
        if (Empty(priorities_queue))
            break;

        task_position = Pop(priorities_queue);
        // release queue mutex  

        status = set_file_pointer(tasks_file, task_position, FILE_BEGIN,
                                  __FILE__, __LINE__, __func__);

        if (status != SUCCESS_CODE)
            goto thread_factorization_execute_exit;

    
        status = read_line(tasks_file, &task_line_buffer, &task_line_length);
        // release read lock 

        if (status != SUCCESS_CODE)
            goto thread_factorization_execute_exit;

        task = atoi(task_line_buffer);

        status = get_primes_string(task, &primes_string_buffer);

        if (status != SUCCESS_CODE)
            goto thread_factorization_execute_exit;

        // write lock 
        status = append_line_to_line(tasks_file, primes_string_buffer);
        // release write lock 

        if (status != SUCCESS_CODE)
            goto thread_factorization_execute_exit;
    }

thread_factorization_execute_exit:

    if (task_line_buffer != NULL)
        free(task_line_buffer);

    if (primes_string_buffer != NULL)
        free(primes_string_buffer);

    return status;
}

/// free_factorization_thread_resources
/// inputs:  
/// outputs: error_code   
/// summary: 
error_code_t free_factorization_thread_resources(HANDLE thread_tasks_file, error_code_t current_status)
{
    error_code_t status = SUCCESS_CODE; 
    status = close_handle(thread_tasks_file, INVALID_HANDLE_VALUE, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        current_status = status; 

    return current_status;
}