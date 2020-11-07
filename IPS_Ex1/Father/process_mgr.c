//authors:
//	or shahar ( orshahar1@mail.tau.ac.il )
//	michaelz  ( zhitomirsky1@mail.tau.ac.il )

#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include "common.h"

#define TIMEOUT_IN_MILLISECONDS 1000
#define BRUTAL_TERMINATION_CODE -1

bool create_process(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
void timeout_handle(int waitcode, HANDLE hprocess);
char* get_command_with_args(const char* process_path, const char* process_args);

void process_handler(const char* process_path, const char* process_args)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;

	
	TCHAR* command = get_command_with_args(process_path, process_args);

	retVal = create_process(command, &procinfo);

	if (retVal == 0)
		print_error(MSG_ERR_PROCESS_CREATION_FAILED, __FILE__, __LINE__, __func__);

	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS);

	timeout_handle(waitcode, procinfo.hProcess);

	GetExitCodeProcess(procinfo.hProcess, &exitcode);

	CloseHandle(procinfo.hProcess); 
	CloseHandle(procinfo.hThread); 

	return exitcode;
}

char* get_command_with_args(const char* process_path, const char* process_args)
{
	TCHAR* command =(TCHAR*)malloc(strlen(process_path) + strlen(process_args) + 1);

	if (command == NULL)
		print_error(MSG_ERR_MEM_ALLOC, __FILE__, __LINE__, __func__);

	strcpy(command, process_path);
	strcat(command, " ");
	strcat(command, process_args);

	return command; 
}

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


void timeout_handle(int waitcode, HANDLE hprocess) 
{
	if (waitcode != WAIT_TIMEOUT)
		return;
	
	printf("WAIT_TIMEOUT\n");

	TerminateProcess(hprocess, BRUTAL_TERMINATION_CODE);
	Sleep(10);
	
}






