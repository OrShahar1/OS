// include headers ------------------------------------------------------------

#include <windows.h>
#include <stdio.h>  

#include "factorization_thread.h"
#include "win_api_wrappers.h"
#include "primes_handler.h"
#include "error_mgr.h"
#include "Queue.h"
#include "Lock.h"


// function declarations ------------------------------------------------------

void free_thread_factorization_execute_resources(char* task_line_buffer, char* primes_string_buffer);
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


    // open tasks file for reading / writing 
    DWORD desired_access       = GENERIC_READ | GENERIC_WRITE;
    DWORD share_mode           = FILE_SHARE_READ | FILE_SHARE_WRITE; 
    DWORD creation_disposition = OPEN_EXISTING;

    status = open_file(&tasks_file, thread_input->tasks_path, desired_access, share_mode, creation_disposition,
                       __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;

    status = thread_factorization_execute(tasks_file, thread_input->priorities_queue, thread_input->resources_lock);
    
    status = free_factorization_thread_resources(tasks_file, status);

    return (int)status; 
}

/// thread_factorization_execute
/// inputs:  tasks_file, priorities_queue, resources_lock
/// outputs: error code   
/// summary: execute the thread functionality:  
///          take task from queue -> read task from file -> perform the factorization -> write to file 
error_code_t thread_factorization_execute(HANDLE tasks_file, queue* priorities_queue, lock* resources_lock)
{
    error_code_t status = SUCCESS_CODE;
    char* task_line_buffer = NULL;
    char* primes_string_buffer = NULL;
    int task = 0, task_position = 0; 
    bool is_empty_queue = false;

    while (true)
    {
        status = get_task_position_from_queue(priorities_queue, resources_lock, &task_position, &is_empty_queue); 
      
        if (status != SUCCESS_CODE || is_empty_queue)
            break;

        status = read_task_from_thread(tasks_file, resources_lock, task_position, &task_line_buffer, &task);

        if (status != SUCCESS_CODE)
            break;

        status = get_primes_string(task, &primes_string_buffer);

        if (status != SUCCESS_CODE)
            break;

        status = append_primes_string_to_file(tasks_file, resources_lock, primes_string_buffer);

        if (status != SUCCESS_CODE)
            break;
    }

    free_thread_factorization_execute_resources(task_line_buffer, primes_string_buffer);

    return status;
}

/// get_task_position_from_queue
/// inputs:  priorities_queue, resources_lock, p_task_position, p_is_empty_queue
/// outputs: error code   
/// summary: Takes the task location from the queue.
///          Performs lock protection by accessing the shared resource.
///            
error_code_t get_task_position_from_queue(queue* priorities_queue, lock* resources_lock, int* p_task_position, bool* p_is_empty_queue)
{
    error_code_t status = SUCCESS_CODE;

    status = write_lock(resources_lock, QUEUE_LOCK);

    if (status != SUCCESS_CODE)
        return status;

    if (Empty(priorities_queue) == false)
        *p_task_position = Pop(priorities_queue);
    else  
        *p_is_empty_queue = true;

    status = write_release(resources_lock, QUEUE_LOCK);

    return status;
}

/// read_task_from_thread
/// inputs:  tasks_file, resources_lock, task_position, p_task_line_buffer, p_task
/// outputs: error code   
/// summary: Produces the appropriate format string for the current task.
///          Performs lock protection by accessing the shared resource.
/// 
error_code_t read_task_from_thread(HANDLE tasks_file, lock* resources_lock, int task_position, char** p_task_line_buffer, int* p_task)
{
    error_code_t  status = SUCCESS_CODE;

    int task_line_length;

    status = set_file_pointer(tasks_file, task_position, FILE_BEGIN,
                              __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status; 

    status = read_lock(resources_lock);

    if (status != SUCCESS_CODE)
        return status;

    else { status = read_line(tasks_file, p_task_line_buffer, &task_line_length); }
    
    if (status != SUCCESS_CODE)
        return status;

    status = read_release(resources_lock); 
    
    if (status != SUCCESS_CODE)
        return status;

    *p_task = atoi(*p_task_line_buffer);

    return status;
}

/// append_primes_string_to_file
/// inputs:  tasks_file, resources_lock, primes_string_buffer
/// outputs: error code   
/// summary: write the appropriate format string to tasks_file.
///          Performs lock protection by accessing the shared resource.
/// 
error_code_t append_primes_string_to_file(HANDLE tasks_file, lock* resources_lock, char* primes_string_buffer)
{
    error_code_t status = SUCCESS_CODE;

    status = write_lock(resources_lock, FILE_LOCK);

    if (status != SUCCESS_CODE)
        return status;

    status = append_line_to_file(tasks_file, primes_string_buffer);
    // release write lock 

    if (status != SUCCESS_CODE)
        return status;

    status = write_release(resources_lock, FILE_LOCK);

    return status;

}

/// free_factorization_thread_resources
/// inputs:  thread_tasks_file , current_status
/// outputs: error_code   
/// summary: free all factorization thread resources 
///
error_code_t free_factorization_thread_resources(HANDLE thread_tasks_file, error_code_t current_status)
{
    error_code_t status = SUCCESS_CODE; 
    status = close_handle(thread_tasks_file, INVALID_HANDLE_VALUE, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        current_status = status; 

    return current_status;
}

/// free_thread_factorization_execute_resources
/// inputs:  task_line_buffer, primes_string_buffer
/// outputs: -   
/// summary: free thread factorization resources
///
void free_thread_factorization_execute_resources(char* task_line_buffer, char* primes_string_buffer)
{
    if (task_line_buffer != NULL)
        free(task_line_buffer);

    if (primes_string_buffer != NULL)
        free(primes_string_buffer);
}
