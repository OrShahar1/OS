//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "common.h"

static const int TIMEOUT_IN_MILLISECONDS = 5000;

BOOL create_process(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
BOOL waitcode_handle(int waitcode, HANDLE hprocess, int process_error_code);
char* get_command_with_args(const char* process_path, const char* process_args);
void terminate_process(HANDLE hprocess, int process_error_code); 


/// process_handler
/// inputs:	 process_path - the path to the .exe for the process
///		     process_args - string which contains the arguments to the process
///			 process_error_code - the exitcode of the process in case of a failure
/// outputs: exitcode of the process
/// summary: creates process according to inputs, waits for its termination and returns
///			 the exitcode 
/// 
int process_handler(const char* process_path, const char* process_args, int process_error_code)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;

	TCHAR* command = get_command_with_args(process_path, process_args);
	int get_exit_code_val = 0;

	if (command == NULL_ERROR_CODE)
		return process_error_code;

	retVal = create_process(command, &procinfo);

	if (retVal == FALSE)
	{
		free(command);
		print_error(MSG_ERR_PROCESS_CREATION_FAILED, __FILE__, __LINE__, __func__);
		return process_error_code;
	}

	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS);

	retVal = waitcode_handle(waitcode, procinfo.hProcess, process_error_code);

	if (retVal == FALSE)
	{
		print_error(MSG_ERR_PROCESS_WAIT_FAILED, __FILE__, __LINE__, __func__);
		return process_error_code;
	}
	
	retVal = GetExitCodeProcess(procinfo.hProcess, &exitcode);

	CloseHandle(procinfo.hProcess); 
	CloseHandle(procinfo.hThread); 
	
	if (retVal == FALSE)
	{
		print_error(MSG_ERR_GET_EXIT_CODE, __FILE__, __LINE__, __func__);
		return process_error_code;
	}

	return exitcode; 
}

/// get_command_with_args
/// inputs:	 process_path - the path to the .exe for the process
///		     process_args - string which contains the arguments to the process
/// outputs: pointer to concatenation of process_path and process_args 
/// summary: concatenates the input strings and returns pointer
char* get_command_with_args(const char* process_path, const char* process_args)
{
	// command length should be "process_path" length + "process args" length +
	//							+ 1 for ' ' and 1 for '\0'
	TCHAR* command = (TCHAR*)malloc((strlen(process_path) + strlen(process_args) + 2) * sizeof(TCHAR));

	if (command == NULL)
		return NULL_ERROR_CODE; 

	strcpy(command, process_path);
	strcat(command, " ");
	strcat(command, process_args);

	return command; 
}

/// create_process
/// inputs:	 CommandLine - string containing path to .exe file and its args 
///		     ProcessInfoPtr - pointer to process information
/// outputs: return value of CreateProcess
/// summary: creates process and returns true if process created successfully, else false
/// 
BOOL create_process(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; 

	BOOL ret_val= CreateProcess(
		NULL,					/*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);

	return ret_val;
}

/// waitcode_handle
/// inputs:	 waitcode - the waitcode as returned from WaitForSingleObject 
///		     hprocess - the handle to the process
///			 process_error_code - the exitcode of the process in case of a failure 
/// outputs:  -
/// summary: terminates process if there was wait timeout
/// 
BOOL waitcode_handle(int waitcode, HANDLE hprocess, int process_error_code)
{
	BOOL is_normal_waitcode = TRUE;

	switch (waitcode) 
	{
	case WAIT_TIMEOUT:
		terminate_process(hprocess, process_error_code);
		is_normal_waitcode = FALSE;
		break; 

	case WAIT_FAILED: 
		
		is_normal_waitcode = FALSE;
		break;
	}

	return is_normal_waitcode;
}

void terminate_process(HANDLE hprocess, int process_error_code)
{
	printf("Process wait timeout - terminating process.\n");
	TerminateProcess(hprocess, process_error_code);
	Sleep(10);
}




