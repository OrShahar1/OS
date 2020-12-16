#ifndef LOCK_H 
#define LOCK_H

// include headers ------------------------------------------------------------

#include <windows.h>
#include "error_mgr.h"

// structs --------------------------------------------------------------------

typedef struct _lock {
	int readers;
	HANDLE readers_mutex;  // mutex 
	HANDLE thread_in_file; // semaphore(1)
	HANDLE turnstile_mutex;      // mutex
} lock;

// function declarations ------------------------------------------------------

/// 
/// inputs: 
/// outputs:  
/// summary: 
error_code_t InitializeLock(lock** p_my_lock); 


#endif // LOCK_H