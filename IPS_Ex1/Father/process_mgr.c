//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include "common.h"

static const int TIMEOUT_IN_MILLISECONDS = 5000;

bool create_process(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
void timeout_handle(int waitcode, HANDLE hprocess);
char* get_command_with_args(const char* process_path, const char* process_args);


/// process_handler
/// inputs:	 process_path - the path to the .exe for the process
///		     process_args - string which contains the arguments to the process
/// outputs: exitcode of the process
/// summary: creates process according to inputs, waits for its termination and returns
///			 the exitcode 
int process_handler(const char* process_path, const char* process_args)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;

	TCHAR* command = get_command_with_args(process_path, process_args);
	int get_exit_code_val = 0;

	if (command == ERROR_CODE_NULL)
		return ERROR_CODE_PROCESS;

	retVal = create_process(command, &procinfo);

	if (retVal == 0) 
	{
		free(command);
		print_error_and_return_error_code(MSG_ERR_PROCESS_CREATION_FAILED, __FILE__, __LINE__, __func__);
		return ERROR_CODE_PROCESS;
	}

	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS);

	timeout_handle(waitcode, procinfo.hProcess);

	get_exit_code_val = GetExitCodeProcess(procinfo.hProcess, &exitcode);

	CloseHandle(procinfo.hProcess); 
	CloseHandle(procinfo.hThread); 

	if (get_exit_code_val == 0)
	{
		print_error_and_return_error_code(MSG_ERR_GET_EXIT_CODE, __FILE__, __LINE__, __func__);
		return ERROR_CODE_PROCESS;
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
	TCHAR* command = (TCHAR*)malloc(strlen(process_path) + strlen(process_args) + 2);

	if (command == NULL) 
		return(print_error_and_return_error_code(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__));

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
bool create_process(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; 

	bool ret_val= CreateProcess(
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

/// timeout_handle
/// inputs:	 waitcode - the waitcode as returned from WaitForSingleObject 
///		     hprocess - the handle to the process
/// outputs:  -
/// summary: terminates process if there was wait timeout
void timeout_handle(int waitcode, HANDLE hprocess) 
{
	if (waitcode != WAIT_TIMEOUT)
		return;
	
	printf("Process wait timeout - terminating process.\n");

	TerminateProcess(hprocess, ERROR_CODE_PROCESS);
	Sleep(10);
	
}






