//
//
//
//
//HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE StartAddress,
//    LPVOID ParameterPtr,
//    LPDWORD ThreadIdPtr)
//{
//    return CreateThread(
//        NULL,            /*  default security attributes */
//        0,                /*  use default stack size */
//        StartAddress,    /*  thread function */
//        ParameterPtr,    /*  argument to thread function */
//        0,                /*  use default creation flags */
//        ThreadIdPtr);    /*  returns the thread identifier */
//}
//
//
//// TODO 
//int thread_lines_size(int total_line_in_file, int number_of_threads, bool last_time)
//{
//    int divisor = total_line_in_file / number_of_threads;
//    int remainder = 0;
//
//    if (last_time)
//        remainder = total_line_in_file % divisor;
//
//    return divisor + remainder;
//}
//
//int get_current_place(int thread_number, int current_line_size)
//{
//    return current_line_size * thread_number;
//}
//
//void line_buffer_fill(FILE* p_file_input, char* line_buffer, int line_size)
//{
//    char char_input;
//    int index;
//    for (char_input = getc(index), index = 0; char_input != EOF && index < line_size; char_input = getc(p_file_input), index++)
//        line_buffer[index] = char_input;
//
//    line_buffer[index] = '\0';
//
//    return line_buffer;
//}
//
//char* get_line_buffer(FILE* p_file_input, int thread_number, const int current_line_size)
//{
//    rewind(p_file_input);
//    char line_buffer[current_line_size + 1];
//    int index;
//
//    fseek(p_file_input, get_current_place(thread_number, current_line_size));
//
//    line_buffer = line_buffer_fill(p_file_input, line_buffer, current_line_size);
//
//    return line_buffer;
//}
//
//
//
//
//
//
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
///**
// * This function uses the win32 api function CreateMutex() to create
// * a named mutex, without accepting the ownership for it (in other words,
// * without locking it).
// */
//HANDLE create_mutex()
//{
//    return CreateMutex(
//        NULL,              // default security attributes
//        FALSE,             // initially not owned
//        NULL);
//}
//
//
///**
// * This function implements the basic lock, critical section, unlock
// * part of the algorithm. In the critical section, this function
// * writes into the global variable Histogram.
// */
//DWORD LockUpdateAndUnlock( Bases_t Base, HANDLE MutexHandle )
//{
//    BOOL ReleaseRes;
//
//    DWORD WaitRes = WaitForSingleObject( MutexHandle, INFINITE );
//                
//    if ( WaitRes != WAIT_OBJECT_0 )
//    { 
//        if ( WaitRes == WAIT_ABANDONED )
//        {
//            printf("Some thread has previously exited without releasing a mutex."
//                    " This is not good programming. Please fix the code.\n" );
//            return ( ISP_MUTEX_ABANDONED );
//        }
//        else
//            return( ISP_MUTEX_WAIT_FAILED );
//    }
//                
//    /* ........Critical Section Start................ */
//
//    Histogram[ Base ]++;
//
//    /* ........Critical Section End.................. */
//    
//    ReleaseRes = ReleaseMutex( MutexHandle );
//
//    if ( ReleaseRes == FALSE )
//        return ( ISP_MUTEX_RELEASE_FAILED );
//
//    return ( ISP_SUCCESS );
//}
//
///**
// * This function goes over a part of the file, and records the number of 
// * occurrences of each letter in the global Histogram array.
// */
//DWORD ScanFileAndRecordOccurences(
//    FILE* GenomeFile,
//    ThreadFuncInput_t *I,
//    HANDLE MutexHandleT,
//    HANDLE MutexHandleC )
//{
//    DWORD ExitCode;
//    int Letter;
//    BOOL Stop = FALSE;
//
//    while ( !Stop )
//    {
//        long int Position = ftell(GenomeFile);
//
//        if ( Position == -1L ) return ( ISP_FTELL_FAILED );
//
//        if ( Position == I->FilePosition.End ) 
//            Stop = TRUE; /* handle last letter, then exit loop */
//
//        Letter = fgetc( GenomeFile ); /* automatically advances the file */ 
//        /* position indicator, affecting the result of the next call to ftell() */
//
//        if ( Letter == EOF ) return ( ISP_FILE_READING_FAILED );
//
//        switch( Letter )
//        {
//        case 'G':
//            ExitCode = LockUpdateAndUnlock( BASE_G, MutexHandleG );  //MutexHandleG is global
//            if ( ExitCode != ISP_SUCCESS ) return( ExitCode );
//            break;
//        case 'A':
//            ExitCode = LockUpdateAndUnlock( BASE_A, I->MutexHandleA ); 
//            if ( ExitCode != ISP_SUCCESS ) return( ExitCode );
//            break;
//        case 'T':
//            ExitCode = LockUpdateAndUnlock( BASE_T, MutexHandleT );
//            if ( ExitCode != ISP_SUCCESS ) return( ExitCode );
//            break;
//        case 'C':
//            ExitCode = LockUpdateAndUnlock( BASE_C, MutexHandleC );
//            if ( ExitCode != ISP_SUCCESS ) return( ExitCode );
//            break;
//        default:
//            return( ISP_ILLEGAL_LETTER_WAS_READ );
//            
//        };
//
//        Sleep(10); /* <ISP> slowing the operation down so that threading */
//                  /* behavior becomes more visible */
//    }
//
//    return ISP_SUCCESS;
//}
//
//
//  //FILE* GenomeFile = fopen(GenomeFileName, "r");
//    //if (GenomeFile == NULL) return ISP_FILE_OPEN_FAILED;
//    // SeekRes = fseek(GenomeFile, I->FilePosition.Start, SEEK_SET);
//    // if (SeekRes != 0)
//    //   status = ISP_FILE_SEEK_FAILED; goto CountBases_Cleanup_3;
//    
//
//    /* 
//    Get a handle to MutexC, that was already created by the main thread: 
//    MutexHandleC = OpenMutex(
//        SYNCHRONIZE, default value 
//        FALSE,       default value 
//        MutexNameC); <ISP> This MUST be the EXACT same name as was used when */
//                      /* the mutex was created. To save heartache, use a string */
//                      /* constants ( as is done here ). 
//     */
//
//
//
//       /*
//    
//     mutex_handle = create_mutex();
//    
//    if (mutex_handle == NULL)
//    {
//         print_error(MSG_ERR_CREATE_MUTEX, __FILE__, __LINE__, __func__);
//         return RETURN_CODE_ERROR;
//    }
//
//    if ( MutexHandleC == NULL) 
//        { printf("CreateMutex error: %d\n", GetLastError()); goto Main_Cleanup_2; }
//    */
//  