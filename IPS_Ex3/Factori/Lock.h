#ifndef LOCK_H 
#define LOCK_H

// include headers ------------------------------------------------------------

#include <windows.h>
#include "error_mgr.h"

//enums -----------------------------------------------------------------------

typedef enum {
	QUEUE_LOCK,
	FILE_LOCK
}lock_mode_t;

// structs --------------------------------------------------------------------

typedef struct _lock {
	int readers;
	HANDLE readers_mutex;  // mutex 
	HANDLE thread_in_file; // semaphore(1)
	HANDLE turnstile_mutex;      // mutex
	HANDLE queue_mutex;      // mutex
} lock;

// function declarations ------------------------------------------------------

/// 
/// inputs: 
/// outputs:  
/// summary: 
error_code_t InitializeLock(lock** p_my_lock); 

error_code_t read_lock(lock* resources_lock);

error_code_t read_release(lock* resources_lock);

error_code_t write_lock(lock* resources_lock, lock_mode_t lock_mode);

error_code_t write_release(lock* resources_lock, lock_mode_t lock_mode);

error_code_t DestroyLock(lock** p_my_lock, error_code_t current_status);

#endif // LOCK_H