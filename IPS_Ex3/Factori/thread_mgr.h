
#ifndef THREAD_MGR_H
#define THREAD_MGR_H

// include headers ------------------------------------------------------------
#include "Queue.h"
#include "error_mgr.h"
/// 
/// inputs:  
/// outputs: error_code  
/// summary: cread thread HANDLEs and schedule them together using semaphore
///          if an error occures -> relase resources
error_code_t factorization_threads_manager(int threads_num, const char* tasks_path, queue* priorities_queue) // +lock


#endif // THREAD_MGR_H
