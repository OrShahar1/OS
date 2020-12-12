//
//// include headers ------------------------------------------------------------
//
//#include <stdio.h>
//#include <windows.h>
//#include "init_cipher_thread_input.h"
//#include "error_mgr.h"
//#include "cipher_thread.h"
//
//
//// function declarations ------------------------------------------------------
//
//void initialize_thread_input_fields(cipher_thread_input* thread_input, HANDLE* p_thread_start_semaphore, bool to_decrypt, int key, const char* input_path, const char* output_path);
//error_code_t initialize_threads_block_limits(cipher_thread_input* thread_inputs, const char* input_path, int threads_num); 
//error_code_t get_lines_number_in_file(HANDLE input_file, int* lines_amount);
//void calc_block_sizes_and_amounts(int* max_block_amount,int* min_block_amount,int* max_block_size, int* min_block_size,int threads_num, int  lines_num);
//error_code_t advance_file_and_get_bytes_block_size(HANDLE* p_thread_input_file, int lines_block_size, int* p_bytes_block_size);
//error_code_t init_block_limits(cipher_thread_input* thread_inputs, HANDLE thread_input_file, int threads_num, int lines_num);

// function implementations ------------------------------------------------------ 
//
///// initialize_thread_inputs
///// inputs:  thread_inputs[] ,  p_thread_start_semaphore , threads_num , to_decrypt , key , input_path ,output_path
///// outputs: error code 
///// summary: initialize thread_inputs structs & fields of each thread
//error_code_t initialize_thread_inputs(cipher_thread_input* p_thread_inputs[], HANDLE* p_thread_start_semaphore, int  threads_num,
//    bool to_decrypt, int key, const char* input_path, const char* output_path)
//{
//    int thread_index;
//    error_code_t status = SUCCESS_CODE;
//    cipher_thread_input* thread_inputs = NULL;
//
//    // create thread_inputs array that holds all threads' input structs 
//    thread_inputs = (cipher_thread_input*)malloc(threads_num * sizeof(cipher_thread_input));
//
//    // if malloc failed return with error
//    status = check_mem_alloc(thread_inputs, __FILE__, __LINE__, __func__);
//
//    if (status != SUCCESS_CODE)
//        return status;
//
//    // initialize the thread_inputs fields 
//    for (thread_index = 0; thread_index < threads_num; thread_index++)
//    {
//        initialize_thread_input_fields(thread_inputs + thread_index, p_thread_start_semaphore,
//            to_decrypt, key, input_path, output_path);
//    }
//
//    // initialize the threads' block limits  
//    status = initialize_threads_block_limits(thread_inputs, input_path, threads_num);
//
//    *p_thread_inputs = thread_inputs;
//
//    return status;
//}
//
///// initialize_thread_input_fields
///// inputs:  thread_input ,  p_thread_start_semaphore , to_decrypt , key , input_path , output_path
///// outputs: -
///// summary: initialize thread_inputs fields, without the block limits 
//void initialize_thread_input_fields(cipher_thread_input* thread_input, HANDLE* p_thread_start_semaphore,
//    bool to_decrypt, int key, const char* input_path, const char* output_path)
//{
//    thread_input->key = key;
//    thread_input->input_path = input_path;
//    thread_input->output_path = output_path;
//    thread_input->to_decrypt = to_decrypt;
//    thread_input->p_thread_start_semaphore = p_thread_start_semaphore;
//}
//
///// initialize_threads_block_limits
///// inputs:  thread_inputs[] ,  input_path , threads_num  
///// outputs: error code 
///// summary: initialize thread_inputs.line_block_limits struct of each thread by
/////          calculating max and min block sizes and amounts and then 
/////          reading the file and counting number of bytes in each block                              
//error_code_t initialize_threads_block_limits(cipher_thread_input* thread_inputs, const char* input_path, int threads_num)
//{
//    error_code_t status = SUCCESS_CODE;
//    int lines_num = 0;
//
//    HANDLE thread_input_file;
//
//    // open file for creating threads line blocks
//    thread_input_file = CreateFileA(input_path, GENERIC_READ, FILE_SHARE_READ, NULL,
//        OPEN_EXISTING, FILE_ATTRIBUTE_TEMPORARY, NULL);
//
//    // if file opening failed return with error
//    if (thread_input_file == INVALID_HANDLE_VALUE)
//    {
//        print_error(MSG_ERR_CANNOT_OPEN_FILE, __FILE__, __LINE__, __func__);
//        return FILE_OPEN_FAILED;
//    }
//
//    status = get_lines_number_in_file(thread_input_file, &lines_num);
//
//    if (status != SUCCESS_CODE)
//        goto init_threads_block_limits_exit;
//
//    status = init_block_limits(thread_inputs, thread_input_file, threads_num, lines_num);
//
//init_threads_block_limits_exit:
//
//    if (thread_input_file != INVALID_HANDLE_VALUE)
//        CloseHandle(thread_input_file);
//
//    return status;
//}
//
///// init_block_limits
///// inputs:  thread_inputs ,  thread_input_file , threads_num , lines_num 
///// outputs: error code 
///// summary: initialize thread_inputs.line_block_limits struct of all threads 
//error_code_t init_block_limits(cipher_thread_input* thread_inputs, HANDLE thread_input_file, int threads_num, int lines_num)
//{
//    error_code_t status = SUCCESS_CODE;
//    int max_block_size, min_block_size, max_block_amount, min_block_amount;
//
//    calc_block_sizes_and_amounts(&max_block_amount, &min_block_amount, &max_block_size, &min_block_size, threads_num, lines_num);
//  
//    int bytes_counter = 0, lines_block_size = 0, bytes_block_size = 0, thread_index = 0;
//
//    // first fill min_block_amount thread inputs with min_block_size line blocks, 
//    //  then fill max_block_amount thread inputs with max_block_size line blocks
//    while (min_block_amount > 0 || max_block_amount > 0)
//    {
//        if (min_block_amount > 0)
//        {
//            lines_block_size = min_block_size;
//            min_block_amount--;
//        }
//        else if (max_block_amount > 0)
//        {
//            lines_block_size = max_block_size;
//            max_block_amount--;
//        }
//
//        status = advance_file_and_get_bytes_block_size(&thread_input_file, lines_block_size, &bytes_block_size);
//        
//        if (status != SUCCESS_CODE)
//            return status;
//
//        thread_inputs[thread_index].line_block_limits.start = bytes_counter;
//
//        bytes_counter += bytes_block_size;
//
//        thread_inputs[thread_index].line_block_limits.end = bytes_counter;
//       
//        if (min_block_amount == 0 && max_block_amount == 0) 
//            thread_inputs[thread_index].line_block_limits.end -= 1;
//        
//        thread_index++;
//    }
//    return status;
//}
//
///// calc_block_sizes_and_amounts
///// inputs:  max_block_amount ,  min_block_amount , max_block_size , min_block_size ,threads_num ,lines_num 
///// outputs: error code 
///// summary:   calculate min block size (min bs) and max block size (max bs) 
/////            min bs = int(#L/#T), max bs = int(#L/#T) + 1                 
/////            amount of max bs = #L % #T, amount of mix bs = #T - #L % #T
//void calc_block_sizes_and_amounts(int* max_block_amount,int* min_block_amount,int* max_block_size,
//                                    int* min_block_size, int threads_num, int  lines_num)
//{
//    *max_block_amount = lines_num % threads_num;
//    *min_block_amount = threads_num - lines_num % threads_num;
//
//    *max_block_size = lines_num / threads_num + 1;
//    *min_block_size = lines_num / threads_num;
//}
//

//
///// advance_file_and_get_bytes_block_size
///// inputs:  thread_input_file ,  lines_block_size , p_bytes_block_size
///// outputs: error code 
///// summary:  read the file until EOF or lines_block_size lines is reached 
/////           updates *p_bytes_block_size with the number of bytes in the block,
/////           also advances file pointer for the next block
//error_code_t advance_file_and_get_bytes_block_size(HANDLE* p_thread_input_file, int lines_block_size, int* p_bytes_block_size)
//{
//    
//    if (lines_block_size == 0)
//    {
//        *p_bytes_block_size = 0;
//        return SUCCESS_CODE;
//    }
//    
//    BOOL return_code;
//    int bytes_block_size = 0;
//    int lines_counter = 0;
//    char char_buffer[CHAR_BUFFER_SIZE + 1];
//    DWORD bytes_read;
//    
//    // read file, count lines and update bytes_block_counter
//    return_code = ReadFile(*p_thread_input_file, char_buffer, CHAR_BUFFER_SIZE, &bytes_read, NULL);
//
//    while (return_code != FALSE && bytes_read != 0)
//    {
//        if (*char_buffer == '\n')
//            lines_counter++;
//
//        if (lines_counter >= lines_block_size)
//            break;
//
//        bytes_block_size++;
//        return_code = ReadFile(*p_thread_input_file, char_buffer, CHAR_BUFFER_SIZE, &bytes_read, NULL);
//    }
//    bytes_block_size++;
//
//    if (return_code == false)
//    {
//        print_error(MSG_ERR_FILE_READING_FAILED, __FILE__, __LINE__, __func__);
//        return FILE_READING_FAILED;
//    }
//
//    *p_bytes_block_size = bytes_block_size;
//    return SUCCESS_CODE;
//}