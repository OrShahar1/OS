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

typedef enum {
	SYNC_OBJECTS_START,
	NO_THREAD_IN_FILE_SEMAPHORE = SYNC_OBJECTS_START,  // semaphore(1)
	READERS_MUTEX,									// mutex 
	TURNSTILE_MUTEX,								// mutex 
	QUEUE_MUTEX,								    // mutex 
	SYNC_OBJECTS_NUM
}sync_object;	


// structs --------------------------------------------------------------------

typedef struct _lock {
	HANDLE sync_objects[SYNC_OBJECTS_NUM];
	int readers;
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