
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
/// outputs:
/// summary: 
error_code_t InitializeLock(lock** p_my_lock)
{
	error_code_t status = SUCCESS_CODE;

	lock* my_lock = (lock*)malloc(sizeof(lock));

	status = check_mem_alloc(my_lock, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		return status;

	my_lock->readers = 0;

	status = create_semaphore(&(my_lock->thread_in_file), 1, 1, __FILE__, __LINE__, __func__);
	if (status != SUCCESS_CODE)
		goto initialize_lock_exit;

	status = create_mutex(&(my_lock->readers_mutex),  __FILE__, __LINE__, __func__);
	if (status != SUCCESS_CODE)
		goto initialize_lock_exit;

	status = create_mutex(&(my_lock->turnstile_mutex),  __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		goto initialize_lock_exit;

	status = create_mutex(&(my_lock->queue_mutex), __FILE__, __LINE__, __func__);

initialize_lock_exit: 
	*p_my_lock = my_lock;

	return status;
}

error_code_t read_lock(lock* resources_lock)
{
	error_code_t current_status, status = SUCCESS_CODE;
	current_status = status;

	status = wait_for_single_object(resources_lock->turnstile_mutex, TURNSTILE_WAIT_TIME, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	status = release_mutex(resources_lock->turnstile_mutex, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	status = wait_for_single_object(resources_lock->readers_mutex, READERS_WAIT_TIME, __FILE__, __LINE__, __func__);
	if (status != SUCCESS_CODE)
		current_status = status;

	resources_lock->readers += 1;
	if ((resources_lock->readers) == 1) 
	{
		status = wait_for_single_object(resources_lock->thread_in_file, THREAD_IN_FILE_WAIT_TIME, __FILE__, __LINE__, __func__);
		if (status != SUCCESS_CODE)
			current_status = status;
	}

	status = release_mutex(resources_lock->readers_mutex, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	return current_status;
}

error_code_t read_release(lock* resources_lock)
{
	error_code_t current_status, status = SUCCESS_CODE;
	current_status = status;

	status = wait_for_single_object(resources_lock->readers_mutex, TURNSTILE_WAIT_TIME, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	resources_lock->readers -= 1;

	if ((resources_lock->readers) == 0)
	{
		status = release_semaphore(resources_lock->thread_in_file, 1, __FILE__, __LINE__, __func__);

		if (status != SUCCESS_CODE)
			current_status = status;
	}
	status = release_mutex(resources_lock->readers_mutex, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	return current_status;
}

error_code_t write_lock(lock* resources_lock, lock_mode_t lock_mode)
{
	error_code_t status = SUCCESS_CODE;

	switch(lock_mode)
	{
	case FILE_LOCK:
		status = wait_for_single_object(resources_lock->turnstile_mutex, TURNSTILE_WAIT_TIME, __FILE__, __LINE__, __func__);

		if (status != SUCCESS_CODE)
			break;

		status = wait_for_single_object(resources_lock->thread_in_file, THREAD_IN_FILE_WAIT_TIME, __FILE__, __LINE__, __func__);
		break;

	case QUEUE_LOCK:
		status = wait_for_single_object(resources_lock->queue_mutex, QUEUE_WAIT_TIME, __FILE__, __LINE__, __func__);
		break;
	}
	return status;
}

error_code_t write_release(lock* resources_lock, lock_mode_t lock_mode)
{
	error_code_t status = SUCCESS_CODE;

	switch (lock_mode)
	{
	case FILE_LOCK:
		status = release_mutex(resources_lock->turnstile_mutex, __FILE__, __LINE__, __func__);

		if (status != SUCCESS_CODE)
			break;

		status = release_semaphore(resources_lock->thread_in_file, 1, __FILE__, __LINE__, __func__);
		break;

	case QUEUE_LOCK:
		status = release_mutex(resources_lock->queue_mutex, __FILE__, __LINE__, __func__);

		break;
	}
	return status;
}

error_code_t DestroyLock(lock** p_my_lock, error_code_t current_status)
{
	error_code_t status = SUCCESS_CODE;
	
	status = close_handle((*p_my_lock)->readers_mutex, NULL, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	status = close_handle((*p_my_lock)->thread_in_file, NULL, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	status = close_handle((*p_my_lock)->turnstile_mutex, NULL, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	status = close_handle((*p_my_lock)->queue_mutex, NULL, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		current_status = status;

	free(*p_my_lock);

	*p_my_lock = NULL; 

	return status;
}