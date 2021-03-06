
// include headers ------------------------------------------------------------

#include <stdio.h>
#include <windows.h>

#include "win_api_wrappers.h"
#include "Lock.h"


// constants ------------------------------------------------------------------

static const int TURNSTILE_WAIT_TIME= 5000;
static const int THREAD_IN_FILE_WAIT_TIME= 5000;
static const int READERS_WAIT_TIME = 5000;
static const int QUEUE_WAIT_TIME = 5000;

// function declarations ------------------------------------------------------


// function implementations ------------------------------------------------------ 

/// InitializeLock
/// inputs: lock** p_my_lock
/// outputs: error_code
/// summary: Creates and allocte memory for the lock object.
///          - Uses one arry of:
///               - One semaphore
///               - 3 mutexs
///          - readers
error_code_t InitializeLock(lock** p_my_lock)
{
	error_code_t status = SUCCESS_CODE;

	lock* my_lock = (lock*)malloc(sizeof(lock));

	status = check_mem_alloc(my_lock, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		return status;

	my_lock->readers = 0;

	status = create_semaphore(&(my_lock->sync_objects[NO_THREAD_IN_FILE_SEMAPHORE]), 1, 1, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		goto initialize_lock_exit;
	
	for (sync_object sync_object_index = (SYNC_OBJECTS_START + 1); sync_object_index < SYNC_OBJECTS_NUM; sync_object_index++)
	{
		status = create_mutex(&(my_lock->sync_objects[sync_object_index]), __FILE__, __LINE__, __func__);

		if (status != SUCCESS_CODE)
			goto initialize_lock_exit;
	}

initialize_lock_exit: 
	*p_my_lock = my_lock;

	return status;
}

/// read_lock
/// inputs:  resources_lock
/// outputs: error_code
/// summary: Receives a pointer to the struct of a lock and performs a lock for reading
///
error_code_t read_lock(lock* resources_lock)
{
	error_code_t current_status, status = SUCCESS_CODE;
	current_status = status;

	status = wait_for_single_object(resources_lock->sync_objects[TURNSTILE_MUTEX], TURNSTILE_WAIT_TIME, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	status = release_mutex(resources_lock->sync_objects[TURNSTILE_MUTEX], __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	status = wait_for_single_object(resources_lock->sync_objects[READERS_MUTEX], READERS_WAIT_TIME, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	resources_lock->readers += 1;
	if ((resources_lock->readers) == 1) 
	{
		status = wait_for_single_object(resources_lock->sync_objects[NO_THREAD_IN_FILE_SEMAPHORE], THREAD_IN_FILE_WAIT_TIME, __FILE__, __LINE__, __func__);
		if (status != SUCCESS_CODE)
			current_status = status;
	}

	status = release_mutex(resources_lock->sync_objects[READERS_MUTEX], __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	return current_status;
}

/// read_release
/// inputs:  resources_lock
/// outputs: error_code
/// summary: Gets a pointer to the struct of a lock and releases the lock from catch to read
///          that made by the same thread
///
error_code_t read_release(lock* resources_lock)
{
	error_code_t current_status, status = SUCCESS_CODE;
	current_status = status;

	status = wait_for_single_object(resources_lock->sync_objects[READERS_MUTEX], TURNSTILE_WAIT_TIME, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	resources_lock->readers -= 1;

	if ((resources_lock->readers) == 0)
	{
		status = release_semaphore(resources_lock->sync_objects[NO_THREAD_IN_FILE_SEMAPHORE], 1, __FILE__, __LINE__, __func__);

		if (status != SUCCESS_CODE)
			current_status = status;
	}
	status = release_mutex(resources_lock->sync_objects[READERS_MUTEX], __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	return current_status;
}

/// InitializeLock
/// inputs:  resources_lock, lock_mode
/// outputs: error_code
/// summary: Receives a pointer to the struct of a lock and locks the lock for writing.
///          Reason for writing is repeated if and only if the lock is not caught for writing and no one catches it
///          to read.
///
error_code_t write_lock(lock* resources_lock, lock_mode_t lock_mode)
{
	error_code_t status = SUCCESS_CODE;

	if (lock_mode == FILE_LOCK)
	{
		status = wait_for_single_object(resources_lock->sync_objects[TURNSTILE_MUTEX], TURNSTILE_WAIT_TIME, __FILE__, __LINE__, __func__);

		if (status != SUCCESS_CODE)
			return status;

		status = wait_for_single_object(resources_lock->sync_objects[NO_THREAD_IN_FILE_SEMAPHORE], THREAD_IN_FILE_WAIT_TIME, __FILE__, __LINE__, __func__);
	}
	else
	{
		// lock_mode == QUEUE_LOCK
		status = wait_for_single_object(resources_lock->sync_objects[QUEUE_MUTEX], QUEUE_WAIT_TIME, __FILE__, __LINE__, __func__);
	}
	return status;
}

/// write_release
/// inputs:  resources_lock , lock_mode
/// outputs: error_code
/// summary: Gets a pointer to the lock struct and releases the lock from the catch
///          for writing. 
///          Note that only the thread that grabbed the lock for writing can release it.
///
error_code_t write_release(lock* resources_lock, lock_mode_t lock_mode)
{
	error_code_t status = SUCCESS_CODE;

	if (lock_mode == FILE_LOCK)
	{
		status = release_mutex(resources_lock->sync_objects[TURNSTILE_MUTEX], __FILE__, __LINE__, __func__);

		if (status != SUCCESS_CODE)
			return status;

		status = release_semaphore(resources_lock->sync_objects[NO_THREAD_IN_FILE_SEMAPHORE], 1, __FILE__, __LINE__, __func__);
	}
	else
	{
		// case of lock_mode == QUEUE_LOCK
		status = release_mutex(resources_lock->sync_objects[QUEUE_MUTEX], __FILE__, __LINE__, __func__);
	}
	return status; 
}

/// DestroyLock
/// inputs:  p_my_lock , current_status
/// outputs: error_code
/// summary: Gets a pointer to the struct of a lock and releases all its resources.
///          At the end of the function NULL assigned to the value of the pointer.
///
error_code_t DestroyLock(lock** p_my_lock, error_code_t current_status)
{
	error_code_t status = SUCCESS_CODE;
	
	for (sync_object sync_object_index = SYNC_OBJECTS_START ; sync_object_index < SYNC_OBJECTS_NUM; sync_object_index++)
	{
		status = close_handle((*p_my_lock)->sync_objects[sync_object_index], NULL,
							  __FILE__, __LINE__, __func__);

		if (status != SUCCESS_CODE)
			current_status = status;
	}

	free(*p_my_lock);

	*p_my_lock = NULL; 

	return status;
}