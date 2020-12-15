
#ifndef FACTORIZATION_THREAD_H
#define FACTORIZATION_THREAD_H

// include headers ------------------------------------------------------------

#include <windows.h>
#include "error_mgr.h"
#include "Queue.h"

//  structs -------------------------------------------------------------------

typedef struct {
    const char* tasks_path;
    queue* priorities_queue;
    // lock
    // int tasks_num;

} factorization_thread_input;

// function declarations ------------------------------------------------------

/// factorization_thread
/// inputs:  Argument
/// outputs: error_code
/// summary: main thread function: 
///          take task from queue -> read task from file -> perform the factorization -> write to file 
error_code_t WINAPI factorization_thread(LPVOID Argument);

#endif // FACTORIZATION_THREAD_H