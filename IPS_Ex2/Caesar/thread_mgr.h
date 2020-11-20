
#ifndef THREAD_MGR_H
#define THREAD_MGR_H

error_code_t cipher_thread_manager(int threads_num, 
	bool to_decrypt, int key, const char* input_path, const char* output_path); 


#endif // THREAD_MGR_H
