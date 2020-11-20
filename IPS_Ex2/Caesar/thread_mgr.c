#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_mgr.h"

//  Structs -----------------------------------------------------------------

typedef struct
{
    long int start;
    long int end;
} block_limits;

typedef struct {
    const char* input_path;
    const char* output_path;
    block_limits line_block_limits;
    HANDLE mutex_handle;

    bool to_decrypt;
    int key;
} cipher_thread_input;


//  Constants -----------------------------------------------------------------

static const int WAIT_FOR_THREADS_TIME = 5000;


// Function Declarations ------------------------------------------------------


HANDLE create_thread(LPTHREAD_START_ROUTINE StartAddress, LPVOID ParameterPtr, LPDWORD ThreadIdPtr );
error_code_t WINAPI cipher_thread(LPVOID Argument); 
BOOL get_thread_block_limits( block_limits* FirstRangePtr,  block_limits* SecondRangePtr );
DWORD lock_update_and_unlock(  HANDLE MutexHandle );
BOOL initialize_thread_inputs(cipher_thread_input* thread_inputs[], int  threads_num, bool to_decrypt, int key,
                              const char* input_path, const char* output_path );
int initialize_thread_block_limits(cipher_thread_input* thread_inputs, HANDLE* thread_input_file);



error_code_t cipher_thread_manager(int threads_num, bool to_decrypt, int key, const char* input_path, const char* output_path)
{
    error_code_t status = SUCCESS_CODE;
    error_code_t return_code;
    cipher_thread_input* thread_inputs;
    HANDLE* thread_handles = (HANDLE*)malloc(threads_num * sizeof(HANDLE));

    if (thread_handles == NULL)
    {
        status = MEM_ALLOC_ERROR;
        goto threads_cleanup;
    }
    // TODO -
    // for now - works with 1 thread 
    return_code = initialize_thread_inputs(&thread_inputs, threads_num, to_decrypt, key, input_path, output_path);

    if (return_code != SUCCESS_CODE)
    {
        status = return_code;
        goto threads_cleanup;
    }

    DWORD  WaitRes;
    LPVOID thread_input;

    int thread_idx;

    for (thread_idx = 0; thread_idx < threads_num; thread_idx++ )
    {        
        //thread_inputs[thread_idx].mutex_handle = mutex_handle;
        thread_input = (LPVOID) & (thread_inputs[thread_idx]);

        
        thread_handles[thread_idx] = create_thread((LPTHREAD_START_ROUTINE)cipher_thread,
                                                    thread_input,
                                                    NULL);
        
        if ( thread_handles[thread_idx] == NULL)
        {
            print_error(MSG_ERR_CREATE_THREAD, __FILE__, __LINE__, __func__); 
            goto threads_cleanup;
        }
    }

    WaitRes = WaitForMultipleObjects( 
        threads_num, 
        thread_handles, 
        TRUE , // ( TRUE --> wait for all )
        WAIT_FOR_THREADS_TIME );

    if ( WaitRes == WAIT_FAILED )
    { 
        print_error(MSG_ERR_CREATE_THREAD, __FILE__, __LINE__, __func__); 
        goto threads_cleanup;
    }


     // TODO - close everything        
     // release resources


threads_cleanup:

    for ( thread_idx = 0 ; thread_idx < threads_num ; thread_idx++ )
        CloseHandle( thread_handles[thread_idx] );

    free(thread_handles);
    
    return status;
}


HANDLE create_thread(LPTHREAD_START_ROUTINE StartAddress,  LPVOID ParameterPtr, LPDWORD ThreadIdPtr)
{
    return CreateThread( NULL,             /*  default security attributes */
                         0,                /*  use default stack size */
                         StartAddress,     /*  thread function */
                         ParameterPtr,     /*  argument to thread function */
                         0,                /*  use default creation flags */
                         ThreadIdPtr );    /*  returns the thread identifier */
}


error_code_t WINAPI cipher_thread(LPVOID Argument)
{
    cipher_thread_input* thread_input = (cipher_thread_input*)Argument;

    error_code_t status = SUCCESS_CODE;              

    status = cipher_execute(thread_input);

    if (status != SUCCESS_CODE) 
        return status;
}


error_code_t initialize_thread_inputs(cipher_thread_input* thread_inputs[], int  threads_num,
    bool to_decrypt, int key, const char* input_path, const char* output_path)
{
    int thread_index;
    error_code_t status = SUCCESS_CODE;

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
        status = FILE_OPEN_FAILED;
        goto initialize_thread_inputs_exit;

    }

    // create thread_inputs array that holds all threads' input structs 
    (*thread_inputs) = (cipher_thread_input*)malloc(threads_num * sizeof(cipher_thread_input));

    // if malloc failed return with error
    if (*thread_inputs == NULL)
    {
        print_error(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__);
        status = MEM_ALLOC_ERROR;
        goto initialize_thread_inputs_exit;
    }


    // ---------------------------------------------------------------
    // count lines in file
    // calculate min block size (min bs) and max block size (max bs) 
    // min bs = int(#L/#T), max bs = int(#L/#T) + 1
    // amount of max bs = #L % #T, amount of mix bs = #T - #L % #T
    // ----------------------------------------------------------------

    // initialize the thread_inputs fields 
    for (thread_index = 0; thread_index < threads_num; thread_index++) 
    {
        status = initialize_thread_input_fields(thread_inputs[thread_index],
            to_decrypt, key, input_path, output_path, thread_input_file);

        if (status != SUCCESS_CODE)
            break;
    }


initialize_thread_inputs_exit:

    if (thread_input_file != INVALID_HANDLE_VALUE)
        CloseHandle(thread_input_file);

    return status;  
}



error_code_t initialize_thread_input_fields(cipher_thread_input* thread_input,
    bool to_decrypt, int key, const char* input_path, const char* output_path,
    HANDLE thread_input_file)
{
    error_code_t status;
    thread_input->key = key;
    thread_input->input_path = input_path;
    thread_input->output_path = output_path;
    thread_input->to_decrypt = to_decrypt;

    status = initialize_thread_block_limits(thread_input, thread_input_file);

    return status;
}



error_code_t initialize_thread_block_limits(cipher_thread_input* thread_input, HANDLE *input_file_handle)
{
    int thread_block_limit = 0;
    char char_input[1];
    DWORD dwBytesRead;
    DWORD dwPtr = SetFilePointer( input_file_handle, 
                                0, 
                                NULL, 
                                FILE_BEGIN ); 

    if (dwPtr == INVALID_SET_FILE_POINTER)
    {
        print_error(MSG_ERR_SET_FILE_POINTER_FAILED, __FILE__, __LINE__, __func__);
        return RETURN_CODE_ERROR; 
    } 

    
    do
    {
        if  (ReadFile(input_file_handle, char_input, 1 , &dwBytesRead, NULL) == FALSE)
            break;

       if (dwBytesRead == 0)
            break;
        
      thread_block_limit++;

    }while(true) ; // inifinte loop 
    
    thread_input->line_block_limits.start = 0;
    thread_input->line_block_limits.end = thread_block_limit;
    
    return SUCCESS_CODE;
}


error_code_t cipher_execute(cipher_thread_input* thread_input )
{

    printf("%d , %d\n ", thread_input->line_block_limits.start, thread_input->line_block_limits.end);
    printf("%s\n %s \n", thread_input->input_path, thread_input->output_path);
    printf("%d , %d \n", thread_input->key, thread_input->to_decrypt);


    HANDLE  thread_input_file = CreateFileA(thread_input->input_path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_TEMPORARY,
        NULL);

    HANDLE  output_file = CreateFileA(thread_input->output_path, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (output_file == INVALID_HANDLE_VALUE) {
        exit(EXIT_FAILURE);
    }

    error_code_t status = SUCCESS_CODE;
    char input_char, new_char; 
    int str_len = (thread_input->line_block_limits.end - thread_input->line_block_limits.start);
    char* line_buffer = (char*)malloc(str_len+1);
    int index = 0;

    DWORD dwBytesRead; 

    if (ReadFile(thread_input_file, line_buffer, str_len, &dwBytesRead, NULL) == FALSE)
        status = FILE_READING_FAILED;
    line_buffer[str_len] = '\0'; 
    printf("%s", line_cipher_execute(line_buffer, thread_input->to_decrypt, thread_input->key)); 
    

    DWORD dwBytesWritten;
    BOOL ret_val =  WriteFile(output_file, line_buffer, str_len, &dwBytesWritten, NULL);
     

    if (ret_val){
        printf("%d bytes written.\n", dwBytesWritten);
    }
 
    return status;
}

