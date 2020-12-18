
// include headers -----------------------------------------------------------

#include <stdio.h>
#include <windows.h>

#include "error_mgr.h"
#include "win_api_wrappers.h"


// functions declarations -----------------------------------------------------

error_code_t get_line_length(HANDLE file, int* p_line_length);

// functions implementations  -------------------------------------------------

// ----------------------------------------------------------------------------
//                              files api   
// ----------------------------------------------------------------------------

/// open_file
/// inputs:  p_file_handle, file_name, desired_access, share_mode, creation_disposition,
///          source_file, source_line, source_func_name
/// outputs: error_code
/// summary: opens files according to file_name, 
///          with desired_access, share_mode and creation_disposition parameters and sets the file handle.
///          in case of error --> prints appropriate message specifying the function's caller
///          (according to source_file, source_line, source_func_name)
/// 
error_code_t open_file(HANDLE* p_file_handle, char* file_name, DWORD desired_access, DWORD share_mode, DWORD creation_disposition,
                       const char* source_file, int source_line, const char* source_func_name)
{
	error_code_t status = SUCCESS_CODE;

    HANDLE file = CreateFileA(file_name, desired_access, share_mode, NULL,
        creation_disposition, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE)
    {
        print_error(MSG_ERR_CANNOT_OPEN_FILE, source_file, source_line, source_func_name);
        status = FILE_OPEN_FAILED;
    }

    *p_file_handle = file;

    return status;
}

/// set_file_pointer
/// inputs:  file_handle, offset, starting_position, 
///          source_file, source_line, source_func_name
/// outputs: error_code
/// summary: changes the (file_handle) to the desired location in file 
///          according to the (starting_position) and the (offset)
///          in case of error --> prints appropriate message specifying the function's caller
///          (according to source_file, source_line, source_func_name)
/// 
error_code_t set_file_pointer(HANDLE file_handle, int offset, int starting_position,
                              const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    DWORD dwPtr;

    dwPtr = SetFilePointer(file_handle, offset, NULL, starting_position);

    if (dwPtr == INVALID_SET_FILE_POINTER)
    {
        print_error(MSG_ERR_SET_FILE_POINTER_FAILED, source_file, source_line, source_func_name);
        status = SET_FILE_POINTER_FAILED;
    }

    return status;
}

/// set_end_of_file
/// inputs:  file_handle, source_file, source_line, source_func_name
/// outputs: error_code
/// summary: sets the end of file for the file pointed by file_handle
///          in case of error --> prints appropriate message specifying the function's caller
///          (according to source_file, source_line, source_func_name)
/// 
error_code_t set_end_of_file(HANDLE file_handle, 
                             const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = SetEndOfFile(file_handle);

    if (return_code == FALSE)
    {
        print_error(MSG_ERR_SET_END_OF_FILE_FAILED, source_file, source_line, source_func_name);
        status = SET_END_OF_FILE_FAILED;
    }

    return status;
}

/// read_file
/// inputs:  file_handle, line, bytes_to_read, p_bytes_read,
///          source_file, source_line, source_func_name
/// outputs: error_code
/// summary: try to read (bytes_to_read) bytes from the file into the (line) buffer, 
///          sets the actual amount of byed read with (p_bytes_read)
///          in case of error --> prints appropriate message specifying the function's caller
///          (according to source_file, source_line, source_func_name)
/// 
error_code_t read_file(HANDLE file_handle, char *line, int bytes_to_read, DWORD* p_bytes_read,
                       const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = ReadFile(file_handle, line, bytes_to_read, p_bytes_read, NULL);
  
    if (return_code == FALSE)
    {
        print_error(MSG_ERR_FILE_READING_FAILED, source_file, source_line, source_func_name);
        status = FILE_READING_FAILED;
    }
    return status;
}

/// read_line
/// inputs:  file_handle, p_line_buffer, p_line_length
/// outputs: error_code
/// summary: reads one line from the file into the (*p_line_buffer) 
///          and sets the length of the line read (p_line_length)
///          sets the actual amount of byed read with (p_bytes_read)
///
error_code_t read_line(HANDLE file_handle, char** p_line_buffer, int* p_line_length)
{
    error_code_t status = SUCCESS_CODE;
    char* line_buffer = NULL;
    DWORD bytes_read = 0;
    int line_length = 0;

    status = get_line_length(file_handle, &line_length);

    if (status != SUCCESS_CODE)
        return status;

    line_buffer = (char*)realloc(*p_line_buffer, (line_length + 1) * sizeof(char));

    status = check_mem_alloc(line_buffer, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;

    status = read_file(file_handle, line_buffer, line_length, &bytes_read, __FILE__, __LINE__, __func__);

    line_buffer[line_length] = '\0';

    *p_line_buffer = line_buffer;
    *p_line_length = line_length;

    return status;
}

/// get_line_length
/// inputs:  file_handle, p_line_length
/// outputs: error_code
/// summary: counts the number of bytes in the current line in the file,
///          after counting, it retrieves the file_handle 
///          to the begginning of the line so we will be able to read it
///
error_code_t get_line_length(HANDLE file_handle, int* p_line_length)
{
    error_code_t status = SUCCESS_CODE;

    char char_buffer;
    int char_counter = 0;

    DWORD bytes_read;

    // read char until end of line 
    status = read_file(file_handle, &char_buffer, 1, &bytes_read, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;

    while (bytes_read != 0)
    {
        char_counter++;

        if (char_buffer == '\n')
            break;

        status = read_file(file_handle, &char_buffer, 1, &bytes_read, __FILE__, __LINE__, __func__);

        if (status != SUCCESS_CODE)
            return status;
    }

    //go to the beginning of the line 
    status = set_file_pointer(file_handle, -(char_counter), FILE_CURRENT, __FILE__, __LINE__, __func__);

    *p_line_length = char_counter;

    return status;
}

error_code_t write_file(HANDLE file, char* line, int bytes_to_write, DWORD* p_bytes_written,
                        const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = WriteFile(file, line, bytes_to_write, p_bytes_written, NULL);

    if (return_code == FALSE)
    {
        print_error(MSG_ERR_FILE_WRITING_FAILED, source_file, source_line, source_func_name);
        status = FILE_WRITING_FAILED;
    }

    return status;
}

/// append_line_to_line
/// inputs:  
/// outputs: error_code  
/// summary: 
error_code_t append_line_to_line(HANDLE file, char* line)
{
    error_code_t status = SUCCESS_CODE;
    DWORD bytes_written;

    status = set_file_pointer(file, 0, FILE_END, __FILE__, __LINE__, __func__);

    if (status != SUCCESS_CODE)
        return status;

    status = write_file(file, line, strlen(line), &bytes_written, __FILE__, __LINE__, __func__);

    return status;
}


// ----------------------------------------------------------------------------
//                                 threads api   
// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------
//                           thread synchronization api   
// ----------------------------------------------------------------------------

error_code_t create_semaphore(HANDLE* p_semaphore_handle, int initial_value, int max_value, 
                              const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;

    *p_semaphore_handle = CreateSemaphore(NULL, initial_value, max_value, NULL);

    if (*p_semaphore_handle == NULL)
    {
        print_error(MSG_ERR_SEMAPHORE_CREATION_FAILED, source_file, source_line, source_func_name);
        status = SEMAPHORE_CREATION_FAILED;
    }

    return status;
}

error_code_t create_mutex(HANDLE* p_mutex_handle,
                          const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;

    *p_mutex_handle = CreateMutex(NULL, FALSE, NULL);     
    
    if (*p_mutex_handle == NULL)
    {
        print_error(MSG_ERR_MUTEX_CREATION_FAILED, source_file, source_line, source_func_name);
        status = MUTEX_CREATION_FAILED;
    }

    return status;
}

error_code_t release_semaphore(HANDLE semaphore, int up_amount,
                              const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = ReleaseSemaphore(semaphore, up_amount, NULL);

    if (return_code == FALSE)
    {
        print_error(MSG_ERR_THREADS_TERMINATION_FAILED, source_file, source_line, source_func_name);
        status = THREADS_TERMINATION_FAILED;
    }

    return status; 
}

error_code_t release_mutex(HANDLE mutex,
                           const char* source_file, int source_line, const char* source_func_name)
{
    error_code_t status = SUCCESS_CODE;
    BOOL return_code;

    return_code = ReleaseMutex(mutex);

    if (return_code == FALSE)
    {
        print_error(MSG_ERR_THREADS_TERMINATION_FAILED, source_file, source_line, source_func_name);
        status = THREADS_TERMINATION_FAILED;
    }

    return status;
}

// ----------------------------------------------------------------------------
//                              general functions 
// ----------------------------------------------------------------------------

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


