//#pragma warning( disable:4715 )
//
//// include headers ------------------------------------------------------------
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "error_mgr.h"
//#include "caesar_encryption_decryption.h"
//
//// enums  ---------------------------------------------------------------------
//
//typedef enum _character_type { 
//	UPPERCASE, 
//	LOWERCASE, 
//	DIGIT,
//	CHAR_TO_IGNORE
//} char_type;
//
//// structs  -------------------------------------------------------------------
//
//typedef struct _cipher_info{
//	char ref_char;
//	int modulo_num;
//
//} cipher_info;
//
//// consts  --------------------------------------------------------------------
//
//static const cipher_info UPPERCASE_CIPHER = { 'A', 26 };
//static const cipher_info LOWERCASE_CIPHER = { 'a', 26 };
//static const cipher_info DIGIT_CIPHER     = { '0', 10 };
//static const int DECRYPTION_MUL = -1;
//static const int ENCRYPTION_MUL = 1;
//
//// functions declarations  ----------------------------------------------------
//
//char char_cipher_execute(bool to_decrypt, char_type type_intro_char, char character, int key);
//cipher_info get_cipher_info(char_type input_char_type);
//char_type classify_char(char input_char);
//bool is_char_in_range(char my_char, char range_start, char range_end);
//int calc_modulo(int divident, int divisor); 
//
//// functions implementations  ----------------------------------------------------
//
///// line_cipher_execute
///// inputs:  line , to_decrypt , key
///// outputs: line
///// summary: return line after encrypting / decrypting line (in place) 
//char* line_cipher_execute(char* line, bool to_decrypt, int key)
//{
//	char current_char, new_char;
//	char_type input_char_type;
//	int line_len = strlen(line);
//	
//	for (int i = 0; i < line_len; i++)
//	{
//		current_char = line[i];
//		input_char_type = classify_char(current_char); 
//
//		if (input_char_type != CHAR_TO_IGNORE)
//		{
//			new_char = char_cipher_execute(to_decrypt, input_char_type, current_char, key);
//			line[i] = new_char;
//		}
//		else
//			line[i] = current_char;
//	}
//	return line;
//}
//
///// char_cipher_execute
///// inputs:  to_decrypt , input_char_type , input_char, key
///// outputs: new_char 
///// summary: char after encrypting / decrypting 
//char char_cipher_execute(bool to_decrypt, char_type input_char_type, char input_char, int key)
//{
//	char new_char; 
//	int cipher_mul = to_decrypt? DECRYPTION_MUL : ENCRYPTION_MUL;
//	
//	cipher_info current_cipher = get_cipher_info(input_char_type);
//
//	new_char = current_cipher.ref_char 
//		+ calc_modulo((input_char - current_cipher.ref_char + cipher_mul * key), current_cipher.modulo_num);
//
//	return new_char;
//
//}
//
///// calc_modulo
///// inputs:  divident , divisor
///// outputs: divident % divisor
///// summary: return the modulo of divident , divisor
//int calc_modulo(int divident, int divisor) 
//{
//	while (divident < 0)
//		divident += divisor;
//	return (divident % divisor); 
//}
//
///// get_cipher_info
///// inputs:  input_char_type
///// outputs: cipher_info  ( UPPERCASE_CIPHER , LOWERCASE_CIPHER , DIGIT_CIPHER )
///// summary: checks if the char_type is a small uppercase letter or digit and returns output accordingly 
//cipher_info get_cipher_info(char_type input_char_type)
//{
//	switch (input_char_type) {
//	case UPPERCASE:
//		return UPPERCASE_CIPHER;
//
//	case LOWERCASE:
//		return LOWERCASE_CIPHER;
//
//	case DIGIT:
//		return DIGIT_CIPHER ;
//	}
//}
//
///// classify_char
///// inputs:  input_char
///// outputs: char_type  ( UPPERCASE , LOWERCASE , DIGIT , CHAR_TO_IGNORE)
///// summary: checks if the character is a small uppercase letter or digit and returns output accordingly 
//char_type classify_char(char input_char)
//{
//
//	if (is_char_in_range(input_char, 'A','Z'))
//		return UPPERCASE;
//
//	if (is_char_in_range(input_char, 'a', 'z'))
//		return LOWERCASE;
//
//	if (is_char_in_range(input_char, '0', '9'))
//		return DIGIT;
//
//	return CHAR_TO_IGNORE;
//
//}
//
///// is_char_in_range
///// inputs:  my_char ,  range_start , range_end
///// outputs: bool 
///// summary:  check if my_char is in range [range_start - range_end]
//bool is_char_in_range(char my_char, char range_start, char range_end)
//{
//	return (my_char >= range_start && my_char <= range_end);
//}