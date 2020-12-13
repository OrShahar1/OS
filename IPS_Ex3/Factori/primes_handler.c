
// include headers ------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include "error_mgr.h"
#include "primes_handler.h"

// enums  ---------------------------------------------------------------------


// structs  -------------------------------------------------------------------


// consts  --------------------------------------------------------------------


// functions declarations  ----------------------------------------------------
int get_primes_with_duplicates(int number, int potential_prime, int* primes_array, int* p_primes_amount); 


// functions implementations  ----------------------------------------------------

/// 
/// inputs:  
/// outputs: 
/// summary:  
error_code_t get_primes(int number, int** p_primes_array, int* p_primes_amount)
{
	error_code_t status = SUCCESS_CODE;
	int* primes_array = NULL;
	int primes_amount = 0;

	// maybe find a smaller size limit
	// NOT SURE ABOUT CURRENT LIMIT 
	primes_array = (int*)malloc((sqrt(number) + 1) * sizeof(int)); 

	status = check_mem_alloc(primes_array, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		return status;

	number = get_primes_with_duplicates(number, 2, primes_array, &primes_amount);

	for (int i = 3; i < sqrt(number); i += 2)
		number = get_primes_with_duplicates(number, i, primes_array, &primes_amount);
	
	if (number > 2)
	{
		primes_array[primes_amount] = number;
		primes_amount++;
	}

	*p_primes_array = primes_array;
	*p_primes_amount = primes_amount;

	return status;
}

/// 
/// inputs:  
/// outputs: 
/// summary:  
int get_primes_with_duplicates(int number, int potential_prime, int* primes_array, int* p_primes_amount)
{
	while (number % potential_prime == 0)
	{
		number = number / potential_prime;
		primes_array[*p_primes_amount] = potential_prime;
		(*p_primes_amount)++;
	}
	return number;
}
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