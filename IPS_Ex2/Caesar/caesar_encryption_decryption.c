#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"
#include "caesar_encryption_decryption.h"


typedef enum _character_type_ { 
	UPPERCASE_INTRO_CHAR = 'A', 
	LOWERCASE_INTRO_CHAR = 'a', 
	DIGIT_INTRO_CHAR = '0' 
} char_type;

typedef enum _mode_type { 
	ENCRYPTION = 'e', 
	DECRYPTION = 'd' 
} mode_type;

typedef enum _modulo_type { 
	LETTER = 26,
	DIGIT = 10
} modulo_type;

char encrypt_or_decrypt(mode_type encrypt_decrypt, char_type type_intro_char, char character, int key);
bool is_not_char_to_ignore(char c);
char_type classification_char(char input_char);

void start_exe(FILE* f_input, FILE* f_output, int key, int flag_encrypt_or_decrypt, int number_of_thread)
{
	char c;
	do
	{
		c = fgetc(f_input);
		if (c == '\n')
			fputc(c, f_output);
			//printf("%c", c);

		if (is_not_char_to_ignore(c))
			fputc(encrypt_or_decrypt(flag_encrypt_or_decrypt, classification_char(c), c, key), f_output);
			//printf("%c", encrypt_or_decrypt(flag_encrypt_or_decrypt, classification_char(c), c, key));
		else
			fputc(c, f_output);
			//printf("%c", c);

	} while (c != EOF);

}

bool is_not_char_to_ignore(char input_char)
{
	if (input_char >= 'A' && input_char <= 'Z')
		return true;

	if (input_char >= 'a' && input_char <= 'z')
		return true;

	if (input_char >= '0' && input_char <= '9')
		return true;

	return false;
}

char encrypt_or_decrypt(mode_type encrypt_decrypt, char_type type_intro_char, char character, int key )
{
	modulo_type modulo_num = LETTER;

	if (!encrypt_decrypt)
		encrypt_decrypt = -1;

	if (type_intro_char == DIGIT_INTRO_CHAR)
		modulo_num = DIGIT;

	return ( type_intro_char + (character - type_intro_char -  key) % modulo_num );

}

char_type classification_char(char input_char)
{
	if (input_char >= 'A' && input_char <= 'Z')
		return UPPERCASE_INTRO_CHAR;

	if (input_char >= 'a' && input_char <= 'z')
		return LOWERCASE_INTRO_CHAR;

	return DIGIT_INTRO_CHAR;

}