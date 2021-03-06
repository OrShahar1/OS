
// include headers ------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

#include "error_mgr.h"
#include "primes_handler.h"


// consts  --------------------------------------------------------------------

static const char* HEADER_PRIMES_STRING = "The prime factors of %d are:";

static const char* LINE_END = "\r\n";

// functions declarations  ----------------------------------------------------

int get_primes_with_duplicates(int number, int potential_prime, int* primes_array, int* p_primes_amount); 
char* build_primes_string(char* primes_string, int primes_string_length, int  number, int* primes_array, int primes_amount);
error_code_t get_primes(int number, int** p_primes_array, int* p_primes_amount);
int calc_primes_string_len(int number, int* primes_array, int primes_amount);
int get_number_len(int number);

// functions implementations  ----------------------------------------------------

/// get_primes_string
/// inputs:  number , p_primes_string
/// outputs: error_code
/// summary: Calls functions that calculate the decomposition into primary factors.
///          Creates the decomposition format for primary factors of number by using build_primes_string.
///          allcote memory for the string with the appropriate size.
///          Stores the result in p_primes_string.
///
error_code_t get_primes_string(int number, char** p_primes_string)
{
	error_code_t status = SUCCESS_CODE;
	char* primes_string = NULL;
	int* primes_array = NULL;
	int primes_amount, primes_string_length = 0;

	status = get_primes(number, &primes_array, &primes_amount);

	if (status != SUCCESS_CODE)
		return status;

	primes_string_length = calc_primes_string_len(number, primes_array, primes_amount);

	primes_string_length += strlen(LINE_END) + 1;

	primes_string = (char*)realloc(*p_primes_string, primes_string_length * sizeof(char));

	status = check_mem_alloc(primes_string, __FILE__, __LINE__, __func__);

	if (status != SUCCESS_CODE)
		goto get_primes_string_exit;

	build_primes_string(primes_string, primes_string_length, number, primes_array, primes_amount);

	snprintf(primes_string, primes_string_length, "%s\r\n", primes_string);
	
	*p_primes_string = primes_string;

get_primes_string_exit:
	if (primes_array != NULL)
		free(primes_array);

	return status;
}

/// build_primes_string
/// inputs:  primes_string , primes_string_length , number , primes_array , primes_amount
/// outputs: char*
/// summary: Creates the decomposition format for primary factors of number 
///
char* build_primes_string(char* primes_string, int primes_string_length, int  number, int* primes_array, int primes_amount)
{
	
	snprintf(primes_string, primes_string_length, HEADER_PRIMES_STRING, number);

	if (primes_amount == 0) 
		return primes_string;

	snprintf(primes_string, primes_string_length, "%s %d", primes_string, primes_array[0]);

	for (int i = 1; i < primes_amount; i++)
		snprintf(primes_string, primes_string_length, "%s, %d", primes_string, primes_array[i]);

	return primes_string; 
	
}

/// get_primes
/// inputs:  number , p_primes_array , p_primes_amount  
/// outputs: error_code_t
/// summary: Calculate the decomposition into primary factors of a number by get_primes_with_duplicates. 
///          Uses the algorithm presented in the homework
///         
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

	
	for (int i = 3; i < (int)sqrt(number) + 1; i += 2)
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

/// get_primes_with_duplicates
/// inputs:  number , potential_prime , primes_array , p_primes_amount
/// outputs: error_code_t
/// summary: Calculate the decomposition into primary factors of a number. 
///           
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

/// calc_primes_string_len
/// inputs:  number , primes_array , primes_amount 
/// outputs: int
/// summary: Calculates the length of the result string for the need of dynamic allocation 
///  
int calc_primes_string_len(int number, int* primes_array, int primes_amount)
{
	int index;
	int primes_string_length = strlen(HEADER_PRIMES_STRING) + get_number_len(number);

	for (index = 0; index < primes_amount; index++)
		primes_string_length += get_number_len(primes_array[index]);

	primes_string_length += 2 * primes_amount - 1;

	return primes_string_length;
}

/// get_number_len
/// inputs:  number  
/// outputs: int
/// summary: Returns the number of digits in the number  
///  
int get_number_len(int number)
{
	int len = 0;
	while (number != 0)
	{
		number = number / 10;
		len++;
	}
 	return len;
}
