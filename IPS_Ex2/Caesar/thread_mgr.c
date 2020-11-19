#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error_mgr.h"

//  Structs -----------------------------------------------------------------

typedef struct {
    char* f_input;
    char* f_output;
    block_limits line_block_limits;
    HANDLE mutex_handle;

    bool to_decrypt;
    int key;
} cipher_thread_input;

typedef struct
{
    long int Start;
    long int End;
} block_limits;

//  Constants -----------------------------------------------------------------

static const int WAIT_TIME_TO_OBJECTS = 5000;


// Function Declarations ------------------------------------------------------


HANDLE create_thread(  LPTHREAD_START_ROUTINE StartAddress, 
                            LPVOID ParameterPtr, 
                            LPDWORD ThreadIdPtr );

DWORD cipher_thread( LPVOID Argument );

BOOL get_thread_block_limits( 
    block_limits* FirstRangePtr, 
    block_limits* SecondRangePtr );

HANDLE create_mutex( );

DWORD lock_update_and_unlock(  HANDLE MutexHandle );



 int cipher_thread_manager(int threads_num, bool to_decrypt, int key, char* f_input, char* f_output)
{
    block_limits *threads_ranges;
    cipher_thread_input* thread_inputs;

    HANDLE *thread_handles;    
  
    HANDLE mutex_handle;

   // DWORD return_code;
    BOOL return_code; 

    int i;
  
    
    // TODO -
    // for now - works with 1 thread 
    return_code = get_thread_block_limits(&(threads_ranges[0]));

    if ( return_code == FALSE)
    { 
        print_error(MSG_ERR_DIVISION_FAILED, __FILE__, __LINE__, __func__);
        return RETURN_CODE_ERROR; 
    }    
    
    /* ----------------------------- */
    /*  2. Create Mutexes            */
    /* ----------------------------- */

    /* Store Mutex G's handle in a global variable. All threads will */
    /* use this variable. */
    mutex_handle = create_mutex();

    if (mutex_handle == NULL)
    {
        print_error(MSG_ERR_CREATE_MUTEX, __FILE__, __LINE__, __func__);
        return RETURN_CODE_ERROR;
    }



    //if ( MutexHandleC == NULL) 
    //    { printf("CreateMutex error: %d\n", GetLastError()); goto Main_Cleanup_2; }

    int thread_idx;
    for (thread_idx = 0; thread_idx < threads_num; thread_idx++ )
    {        
        thread_inputs[thread_idx].mutex_handle = mutex_handle;

        thread_handles[thread_idx] = create_thread(
            (LPTHREAD_START_ROUTINE)cipher_thread,
            (LPVOID) & (thread_inputs[thread_idx]),
            NULL);

        if ( thread_handles[thread_idx] == NULL )
        {
            print_error(MSG_ERR_CREATE_TREAD, __FILE__, __LINE__, __func__); 
            goto Main_Cleanup_1;
        }
    }

    WaitRes = WaitForMultipleObjects( 
        threads_num, 
        ThreadHandles, 
        TRUE ,         // wait for all  
        WAIT_TIME_TO_OBJECTS );

    if ( WaitRes == WAIT_FAILED )
    { 
        print_error(MSG_ERR_CREATE_TREAD, __FILE__, __LINE__, __func__); 
        goto Main_Cleanup_1; 
    }

     // TODO - close evetthing        

     // release resources


Main_Cleanup_1:
    
    /* The following loop closes handles to all threads that were created. The */
    /* variable 'ThreadInd' is set to one more than the index of the last thread */
    /* that was created */
    for ( thread_idx = 0 ; i < threads_num ; thread_idx++ )
    {
        GetExitCodeThread( thread_handles[thread_idx], &return_code );
        printf("Thread %d ended with exit code 0x%x\n",thread_idx, return_code);
        CloseHandle( thread_handles[thread_idx] );
    }

    CloseHandle(mutex_handle);
}




/**
 * This function divides the positions in the Genome file into two ranges of 
 * positions:
 * the first from the beginning to the middle, and the second from the middle to 
 * the end.
 *
 * Accepts
 * -------
 * FirstRangePtr - output parameter, points to a Range_t struct that will 
 *                 be filled with the lower positions.
 * SecondRangePtr - output parameter, points to a Range_t struct that will 
 *                  be filled with the higher positions.
 * Returns: 
 * -------
 * TRUE on success, FALSE on failure
 */ 
BOOL get_thread_block_limits(block_limits* FirstRangePtr, block_limits* SecondRangePtr)
{
    int Res;
    long int LastPosition;
    long int MiddlePosition;
    long int FirstPosition;

    FILE* GenomeFile = fopen( GenomeFileName, "r" );
    
    if ( GenomeFile == NULL ) return FALSE;

    FirstPosition = ftell( GenomeFile );
    
    if ( FirstPosition == -1L ) { fclose( GenomeFile ); return FALSE; }

    Res = fseek( GenomeFile, -1, SEEK_END );

    if ( Res != 0 ) { fclose( GenomeFile ); return FALSE; }    

    LastPosition = ftell( GenomeFile );

    if ( LastPosition == -1L ) { fclose( GenomeFile ); return FALSE; }

    MiddlePosition = LastPosition / 2; 
    
    FirstRangePtr->Start = FirstPosition;
    FirstRangePtr->End = MiddlePosition ;
    SecondRangePtr->Start = MiddlePosition + 1;
    SecondRangePtr->End = LastPosition;

    fclose( GenomeFile );
    return TRUE;  
}

/**
 * This function creates a thread by calling Win32 Api's CreateThread()
 * function, and setting some of the parameters to default value. 
 *
 * parameters:
 * ----------
 * StartAddress - a pointer to the function that will be run by the thread.
 * ParameterPtr - a pointer to the parameter that will be supplied to the 
 *                function run by the thread.
 * ThreadIdPtr - return argument: a pointer to a DWORD variable into which 
 *               the function will write the created thread's ID.
 *
 * returns:
 * --------
 * On success, a handle to the created thread. On Failure - NULL.
 */
HANDLE CreateThreadSimple(  LPTHREAD_START_ROUTINE StartAddress, 
                            LPVOID ParameterPtr, 
                            LPDWORD ThreadIdPtr )
{
    return CreateThread(
                        NULL,            /*  default security attributes */
                        0,                /*  use default stack size */
                        StartAddress,    /*  thread function */
                        ParameterPtr,    /*  argument to thread function */
                        0,                /*  use default creation flags */
                        ThreadIdPtr );    /*  returns the thread identifier */
}

/**
 * This is the thread function. It reads a part of a file and counts occurences
 * of the different letters in it, and records them in the global variable
 * Histogram. 
 * <ISP> this function demonstrates creation, destruction and use of Mutexes.
 */ 
DWORD cipher_thread( LPVOID Argument )
{
    cipher_thread_input *thread_input = (cipher_thread_input*)Argument;
    int SeekRes;
    BOOL Stop = FALSE ;
    HANDLE MutexHandleC;
    HANDLE MutexHandleT;
    DWORD ExitCode = ISP_SUCCESS; /* if no errors occur, this value */ 
                                    /* will be returned */

    FILE* GenomeFile = fopen( GenomeFileName, "r" );

    if ( GenomeFile == NULL ) return ISP_FILE_OPEN_FAILED;

    SeekRes = fseek( GenomeFile, I->FilePosition.Start, SEEK_SET );

    if ( SeekRes != 0 ) 
        { ExitCode = ISP_FILE_SEEK_FAILED; goto CountBases_Cleanup_3; }

    /* Get a handle to MutexC, that was already created by the main thread: */
    MutexHandleC = OpenMutex( 
        SYNCHRONIZE, /* default value */
        FALSE,       /* default value */
        MutexNameC ); /* <ISP> This MUST be the EXACT same name as was used when */
                      /* the mutex was created. To save heartache, use a string */
                      /* constants ( as is done here ). */
    
    if ( MutexHandleC == NULL )
    {
        if ( GetLastError() == ERROR_FILE_NOT_FOUND )
            printf("A mutex with the requested name does not exit.\n");
        
        ExitCode = ISP_MUTEX_OPEN_FAILED; 
        goto CountBases_Cleanup_3;
    }

    MutexHandleT = CreateMutexSimple( MutexNameT );

    if ( MutexHandleT == NULL ) 
    { 
        ExitCode = ISP_MUTEX_CREATE_FAILED; 
        goto CountBases_Cleanup_2; 
    }
    else
    {
        if ( GetLastError() == ERROR_ALREADY_EXISTS )
            printf("Thread %d using already existing mutex T\n", 
                    GetCurrentThreadId() );
        else
            printf("Thread %d created mutex T\n", GetCurrentThreadId() );
    }
    
    ExitCode = ScanFileAndRecordOccurences( 
        GenomeFile,
        I, 
        MutexHandleT,
        MutexHandleC );

    if ( ExitCode != ISP_SUCCESS ) goto CountBases_Cleanup_1;

CountBases_Cleanup_1:
    CloseHandle(MutexHandleC);
CountBases_Cleanup_2:
    CloseHandle(MutexHandleT);
CountBases_Cleanup_3:
    fclose( GenomeFile ); 

    /* Do not close I->MutexHandleA !!! a copy of it exists in each thread, */
    /* so if we close it here, it will be closed multiple times. Instead,  */
    /* the main thread should close it. */

    return ExitCode;
}

/**
 * This function uses the win32 api function CreateMutex() to create 
 * a named mutex, without accepting the ownership for it (in other words, 
 * without locking it).
 */
HANDLE create_mutex()
{
    return CreateMutex( 
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);             
}

/**
 * This function implements the basic lock, critical section, unlock
 * part of the algorithm. In the critical section, this function
 * writes into the global variable Histogram.
 */
DWORD LockUpdateAndUnlock( Bases_t Base, HANDLE MutexHandle )
{
    BOOL ReleaseRes;

    DWORD WaitRes = WaitForSingleObject( MutexHandle, INFINITE );
                
    if ( WaitRes != WAIT_OBJECT_0 )
    { 
        if ( WaitRes == WAIT_ABANDONED )
        {
            printf("Some thread has previously exited without releasing a mutex."
                    " This is not good programming. Please fix the code.\n" );
            return ( ISP_MUTEX_ABANDONED );
        }
        else
            return( ISP_MUTEX_WAIT_FAILED );
    }
                
    /* ........Critical Section Start................ */

    Histogram[ Base ]++;

    /* ........Critical Section End.................. */
    
    ReleaseRes = ReleaseMutex( MutexHandle );

    if ( ReleaseRes == FALSE )
        return ( ISP_MUTEX_RELEASE_FAILED );

    return ( ISP_SUCCESS );
}

/**
 * This function goes over a part of the file, and records the number of 
 * occurrences of each letter in the global Histogram array.
 */
DWORD ScanFileAndRecordOccurences(
    FILE* GenomeFile,
    ThreadFuncInput_t *I,
    HANDLE MutexHandleT,
    HANDLE MutexHandleC )
{
    DWORD ExitCode;
    int Letter;
    BOOL Stop = FALSE;

    while ( !Stop )
    {
        long int Position = ftell(GenomeFile);

        if ( Position == -1L ) return ( ISP_FTELL_FAILED );

        if ( Position == I->FilePosition.End ) 
            Stop = TRUE; /* handle last letter, then exit loop */

        Letter = fgetc( GenomeFile ); /* automatically advances the file */ 
        /* position indicator, affecting the result of the next call to ftell() */

        if ( Letter == EOF ) return ( ISP_FILE_READING_FAILED );

        switch( Letter )
        {
        case 'G':
            ExitCode = LockUpdateAndUnlock( BASE_G, MutexHandleG );  //MutexHandleG is global
            if ( ExitCode != ISP_SUCCESS ) return( ExitCode );
            break;
        case 'A':
            ExitCode = LockUpdateAndUnlock( BASE_A, I->MutexHandleA ); 
            if ( ExitCode != ISP_SUCCESS ) return( ExitCode );
            break;
        case 'T':
            ExitCode = LockUpdateAndUnlock( BASE_T, MutexHandleT );
            if ( ExitCode != ISP_SUCCESS ) return( ExitCode );
            break;
        case 'C':
            ExitCode = LockUpdateAndUnlock( BASE_C, MutexHandleC );
            if ( ExitCode != ISP_SUCCESS ) return( ExitCode );
            break;
        default:
            return( ISP_ILLEGAL_LETTER_WAS_READ );
            
        };

        Sleep(10); /* <ISP> slowing the operation down so that threading */
                  /* behavior becomes more visible */
    }

    return ISP_SUCCESS;
}















//--------------------------------------------------------
// include headers --------------------------------------

#include <stdio.h>
#include <stdbool.h>
#include <windows.h> 

// functions declarations  ----------------------------------------------



int cipher_thread_manager() {



    return RETURN_CODE_NORMAL;
}


HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE StartAddress,
    LPVOID ParameterPtr,
    LPDWORD ThreadIdPtr)
{
    return CreateThread(
        NULL,            /*  default security attributes */
        0,                /*  use default stack size */
        StartAddress,    /*  thread function */
        ParameterPtr,    /*  argument to thread function */
        0,                /*  use default creation flags */
        ThreadIdPtr);    /*  returns the thread identifier */
}

int count_chars_in_block(HANDLE* p_block_start, HANDLE* p_block_end)
{
    int total_chars_in_file = 0;

    rewind(p_file_input);

    while (getc(p_file_input) != EOF)
        total_chars_in_file++;
}

// TODO 
int thread_lines_size(int total_line_in_file, int number_of_threads, bool last_time)
{
    int divisor = total_line_in_file / number_of_threads;
    int remainder = 0;

    if (last_time)
        remainder = total_line_in_file % divisor;

    return divisor + remainder;
}

int get_current_place(int thread_number, int current_line_size)
{
    return current_line_size * thread_number;
}

void line_buffer_fill(FILE* p_file_input, char* line_buffer, int line_size)
{
    char char_input;
    int index;
    for (char_input = getc(index), index = 0; char_input != EOF && index < line_size; char_input = getc(p_file_input), index++)
        line_buffer[index] = char_input;

    line_buffer[index] = '\0';

    return line_buffer;
}

char* get_line_buffer(FILE* p_file_input, int thread_number, const int current_line_size)
{
    rewind(p_file_input);
    char line_buffer[current_line_size + 1];
    int index;

    fseek(p_file_input, get_current_place(thread_number, current_line_size));

    line_buffer = line_buffer_fill(p_file_input, line_buffer, current_line_size);

    return line_buffer;
}




