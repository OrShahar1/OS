
// include headers -----------------------------------------------------------

#include <stdio.h>
#include <windows.h>
#include "error_mgr.h"
#include "win_api_wrappers.h"
// constants ------------------------------------------------------------------

// functions declarations -----------------------------------------------------

error_code_t get_line_length(HANDLE file, int* p_line_length);

// functions implementations  -------------------------------------------------

error_code_t open_file(HANDLE* p_file_handle, char* file_name, DWORD desired_access, DWORD share_mode, DWORD creation_disposition,
                       const char* source_file, int source_line, const char* source_func_name)
{
	error_code_t status = SUCCESS_CODE;

    HANDLE file = CreateFileA(file_name, desired_access, share_mode, NULL,
        creation_disposition, FILE_ATTRIBUTE_NORMAL, NULL);

    // if file opening failed return with error
    if (file == INVALID_HANDLE_VALUE)
    {
        print_error(MSG_ERR_CANNOT_OPEN_FILE, source_file, source_line, source_func_name);
        status = FILE_OPEN_FAILED;
    }
    *p_file_handle = file;
    return status;
}

error_code_t set_end_of_file(HANDLE file, DWORD desired_access, DWORD share_mode, DWORD creation_disposition,
    const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = SetEndOfFile(file);

    if (return_code == FALSE)
    {
        print_error(MSG_ERR_SET_END_OF_FILE_FAILED, source_file, source_line, source_func_name);
        status = SET_END_OF_FILE_FAILED;
    }

    return status;
}

error_code_t read_file(HANDLE file, char *line, int bytes_to_read ,DWORD* p_bytes_read,
                       const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = ReadFile(file, line, bytes_to_read, p_bytes_read, NULL);
  
    if (return_code == FALSE)
    {
        print_error(MSG_ERR_FILE_READING_FAILED, source_file, source_line, source_func_name);
        status = FILE_READING_FAILED;
    }
    return status;
}

error_code_t write_file(HANDLE file, char* line, int bytes_to_read, DWORD* p_bytes_read,
                        const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = WriteFile(file, line, bytes_to_read, p_bytes_read, NULL);

    if (return_code == FALSE)
    {
        print_error(MSG_ERR_FILE_WRITING_FAILED, source_file, source_line, source_func_name);
        status = FILE_WRITING_FAILED;
    }

    return status;
}

error_code_t set_file_pointer(HANDLE file, int offset, int starting_position,
                              const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    DWORD dwPtr;

    dwPtr = SetFilePointer(file, offset, NULL, starting_position);

    if (dwPtr == INVALID_SET_FILE_POINTER)
    {
        print_error(MSG_ERR_SET_FILE_POINTER_FAILED, source_file, source_line, source_func_name);
        status = SET_FILE_POINTER_FAILED;
    }

    return status;
}

/// create_thread
/// inputs:  StartAddress , ParameterPtr , ThreadIdPtr , p_thread_handle
/// outputs: error code  
/// summary: creates thread and updates handle pointer 
error_code_t create_thread(LPTHREAD_START_ROUTINE StartAddress, LPVOID ParameterPtr, LPDWORD ThreadIdPtr, HANDLE* p_thread_handle)
{
    error_code_t status = SUCCESS_CODE;

    HANDLE thread_handle = CreateThread(NULL,             /*  default security attributes */
        0,                /*  use default stack size */
        StartAddress,     /*  thread function */
        ParameterPtr,     /*  argument to thread function */
        0,                /*  use default creation flags */
        ThreadIdPtr);     /*  returns the thread identifier */

    if (thread_handle == NULL)
    {
        print_error(MSG_ERR_CREATE_THREAD, __FILE__, __LINE__, __func__);
        status = THREAD_CREATION_FAILED;
    }

    *p_thread_handle = thread_handle;

    return status;
}

error_code_t get_exit_code_thread(HANDLE thread_handle, DWORD* p_thread_exit_code,
                                  const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = GetExitCodeThread(thread_handle, p_thread_exit_code);

    if (return_code == FALSE)
    {
        print_error(MSG_ERR_GET_THREAD_EXIT_CODE_FAILED, source_file, source_line, source_func_name);
        status = GET_THREAD_EXIT_CODE_FAILED;
    }
    return status;
}

error_code_t wait_for_single_object(HANDLE object_handle, DWORD wait_time,
                                    const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    DWORD wait_code;

    wait_code = WaitForSingleObject(object_handle, wait_time);

    if (wait_code != WAIT_OBJECT_0)
    {
        print_error(MSG_ERR_OBJECT_WAIT_TIMEOUT, source_file, source_line, source_func_name);
        status = OBJECT_WAIT_TIMEOUT; 
    }

    return status;
}


/// check_wait_code_and_terminate_threads
/// inputs:  wait_code , thread_handles , threads_num
/// outputs: error code  
/// summary: if there is wait timeout --> terminate threads 
error_code_t check_wait_code_and_terminate_threads(DWORD wait_code, HANDLE* thread_handles, int threads_num, DWORD brutal_termination_code,
                                                   const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;

    if (wait_code != WAIT_OBJECT_0)
    {
        print_error(MSG_ERR_THREADS_WAIT_TIMEOUT, __FILE__, __LINE__, __func__);
        status = terminate_threads(thread_handles, threads_num, brutal_termination_code,
                                   source_file,  source_line, source_func_name);
    }
    return status;
}

/// terminate_threads
/// inputs:  thread_handles , threads_num
/// outputs: error code  
/// summary: for each thread that is still active - terminate it, 
///          if there are no active threads, return wait timeout error  
error_code_t terminate_threads(HANDLE* thread_handles, int threads_num, DWORD brutal_termination_code,
                               const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    DWORD thread_exit_code;
    
    for (int thread_idx = 0; thread_idx < threads_num; thread_idx++)
    {
        status = get_exit_code_thread(thread_handles[thread_idx], &thread_exit_code, source_file, source_line, source_func_name);

        if (status != SUCCESS_CODE)
            return status;

        if (thread_exit_code != STILL_ACTIVE)
            continue;

        status = terminate_thread(thread_handles[thread_idx], brutal_termination_code, source_file, source_line, source_func_name);
      
        if (status != SUCCESS_CODE)
            return status;
    }
    return THREADS_WAIT_TIMEOUT;
}

error_code_t terminate_thread(HANDLE thread_handle, DWORD brutal_termination_code,
    const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = TerminateThread(thread_handle, brutal_termination_code);

    if (return_code == FALSE)
    {
        print_error(MSG_ERR_THREADS_TERMINATION_FAILED, source_file, source_line, source_func_name);
        status = THREADS_TERMINATION_FAILED;
    }
    return status;
}

error_code_t create_semaphore(HANDLE* p_semaphore_handle, int max_value, 
                              const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;

    *p_semaphore_handle = CreateSemaphore(NULL, 0, max_value, NULL);

    if (*p_semaphore_handle == NULL)
    {
        print_error(MSG_ERR_SEMAPHORE_CREATION_FAILED, source_file, source_line, source_func_name);
        status = SEMAPHORE_CREATION_FAILED;
    }

    return status;
}

error_code_t close_handle(HANDLE object_handle, int invalid_handle_value,
                          const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    if (object_handle != invalid_handle_value)
    {
        return_code = CloseHandle(object_handle);

        if (return_code == FALSE)
        {
            print_error(MSG_ERR_CLOSE_HANDLE_FAILED, source_file, source_line, source_func_name);
            status = CLOSE_HANDLE_FAILED;
        }
    }
    return status;
}


/// get_lines_number_in_file
/// inputs:  input_file ,  lines_amount 
/// outputs: error code 
/// summary: fills lines_amount with the number of lines in file
error_code_t read_line(HANDLE file, char** p_line_buffer, int *p_line_length)
{
    error_code_t status = SUCCESS_CODE;
    DWORD bytes_read;
    char* line_buffer = NULL; 
    int line_length;

    status = get_line_length(file, &line_length);

    if (status != SUCCESS_CODE)
        return status;

     line_buffer = (char*)realloc(*p_line_buffer, (line_length + 1) * sizeof(char));

    status = check_mem_alloc(line_buffer, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;

    status = read_file(file, line_buffer, line_length, &bytes_read, __FILE__, __LINE__, __func__);

    line_buffer[line_length] = '\0';

    *p_line_buffer = line_buffer; 
    *p_line_length = line_length; 

    return status;
}

error_code_t get_line_length(HANDLE file, int* p_line_length)
{
    error_code_t status = SUCCESS_CODE;

    char char_buffer;
    int char_counter = 0;

    DWORD bytes_read;

    // read char until end of line 
    status = read_file(file, &char_buffer, 1, &bytes_read, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;
    
    while (bytes_read != 0)
    {
        char_counter++;

        if (char_buffer == '\n')
            break;

        status = read_file(file, &char_buffer, 1, &bytes_read, __FILE__, __LINE__, __func__);

        if (status != SUCCESS_CODE)
            return status;
    }

    //go to the beginning of the line 
    status = set_file_pointer(file, -(char_counter), FILE_CURRENT, __FILE__, __LINE__, __func__);

    *p_line_length = char_counter;

    return status;
}
