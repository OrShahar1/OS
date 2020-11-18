// include headers --------------------------------------

#include <stdio.h>
#include <stdbool.h>
#include <windows.h> 

// functions declarations  ----------------------------------------------



int count_chars_in_block(HANDLE* p_block_start, HANDLE* p_block_end)
{
	int total_chars_in_file = 0;

	rewind(p_file_input);

	while(getc(p_file_input) != EOF) 
        total_chars_in_file ++; 
}

// TODO 
int thread_lines_size(int total_line_in_file, int number_of_threads, bool last_time)
{
	int divisor    = total_line_in_file / number_of_threads ;
	int remainder  = 0 ; 

	if (last_time)
		remainder  = total_line_in_file % divisor;

	return divisor + remainder;
}

int get_current_place(  int thread_number , int current_line_size  )
{
	return current_line_size * thread_number;
}

void line_buffer_fill(FILE* p_file_input, char * line_buffer, int line_size )
{
	char char_input;
	int index;
	for (char_input = getc(index) ,index = 0  ; char_input != EOF &&  index < line_size ; char_input = getc(p_file_input), index++) 
		line_buffer[index] = char_input;
		
	line_buffer[index] = '\0';

	return line_buffer;
}

char* get_line_buffer(FILE* p_file_input, int thread_number , const int current_line_size )
{
	rewind(p_file_input);
	char line_buffer [current_line_size + 1] ;
	int index;

	fseek(p_file_input, get_current_place(thread_number, current_line_size ));
	
	line_buffer = line_buffer_fill(p_file_input , line_buffer, current_line_size );

	return line_buffer;
}




