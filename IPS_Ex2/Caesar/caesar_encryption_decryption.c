// include headers ------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "caesar_encryption_decryption.h"

// enums  ---------------------------------------------------------------------

typedef enum _character_type { 
	UPPERCASE, 
	LOWERCASE, 
	DIGIT,
	CHAR_TO_IGNORE
} char_type;


// structs  -------------------------------------------------------------------

typedef struct _cipher_info{
	char ref_char;
	int modulo_num;

} cipher_info;

// consts  --------------------------------------------------------------------

static const cipher_info UPPERCASE_CIPHER = { 'A', 26 };
static const cipher_info LOWERCASE_CIPHER = { 'a', 26 };
static const cipher_info DIGIT_CIPHER     = { '0', 10 };
static const int DECRYPTION_MUL = -1;
static const int ENCRYPTION_MUL = 1;

// functions declarations  ----------------------------------------------

char char_cipher_execute(bool to_decrypt, char_type type_intro_char, char character, int key);
cipher_info get_cipher_info(char_type input_char_type);
char_type classify_char(char input_char);
bool is_char_in_range(char my_char, char range_start, char range_end);

void cipher_exe(FILE* f_input, FILE* f_output,  bool to_decrypt, int key, int number_of_thread)
{
	char input_char, new_char; 
	char_type input_char_type;
	do
	{
		input_char = fgetc(f_input);

		if (classify_char(input_char) != CHAR_TO_IGNORE)
		{
			input_char_type = classify_char(input_char);
			new_char = char_cipher_execute(to_decrypt, input_char_type, input_char, key);
			fputc(new_char, f_output);
		}
		else
			fputc(input_char, f_output);


	} while (input_char != EOF);

}

char* line_cipher_execute(char* line, bool to_decrypt, int key)
{
	char current_char, new_char;
	char_type input_char_type;
	int line_len = strlen(line);
	
	for (int i = 0; i < line_len; i++)
	{
		current_char = line[i];
		input_char_type = classify_char(current_char); 

		if (input_char_type != CHAR_TO_IGNORE)
		{
		//	printf("%d", classify_char(current_char));
			new_char = char_cipher_execute(to_decrypt, input_char_type, current_char, key);
			line[i] = new_char;
		}
		else
			line[i] = current_char;
	}

	return line;
}

char char_cipher_execute(bool to_decrypt, char_type input_char_type, char input_char, int key)
{
	char new_char; 
	int cipher_mul = to_decrypt? DECRYPTION_MUL : ENCRYPTION_MUL;
	
	cipher_info current_cipher = get_cipher_info(input_char_type);

	new_char = current_cipher.ref_char + (input_char - current_cipher.ref_char + cipher_mul * key) % current_cipher.modulo_num;

	//printf("%d %c --> %c\n", input_char_type, input_char, new_char);

	return new_char;

}

cipher_info get_cipher_info(char_type input_char_type)
{
	switch (input_char_type) {
	case UPPERCASE:
		return UPPERCASE_CIPHER;

	case LOWERCASE:
		return LOWERCASE_CIPHER;

	case DIGIT:
		return DIGIT_CIPHER ;
	}
}

char_type classify_char(char input_char)
{

	if (is_char_in_range(input_char, 'A','Z'))
		return UPPERCASE;

	if (is_char_in_range(input_char, 'a', 'z'))
		return LOWERCASE;

	if (is_char_in_range(input_char, '0', '9'))
		return DIGIT;

	return CHAR_TO_IGNORE;

}

bool is_char_in_range(char my_char, char range_start, char range_end)
{
	return (my_char >= range_start && my_char <= range_end);
}