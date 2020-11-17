#include <stdio.h>
#include "caesar_encryption_decryption.h"
#include "common.h"
#include "caesar_encryption_decryption.h"
#include "tests.h"


int main(int argc, char* argv[]) 
{
	if (argc != 5)
	{
		//print error
		return RETURN_CODE_ERROR;
	}
	FILE* fp;
	char *file_name = argv[1];
	fopen_s(fp, file_name, 'r')
	if (NULL == fp)
		//print error
		return 1;
	int key = atoi(argv[2]);
	int number_of_thread = atoi(argv[3]);
	char* flag_encrypt_or_decrypt = argv[4];


	// open file input, output

	// get thread number 

	// get key

	// get flag -e -d

	// read and decrypt/ / encrypt  with flag ( -d -e ) 


	return 0;
}