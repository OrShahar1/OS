

// include headers -----------------------------


// enum 

typedef enum _character_type {UPPERCASE , LOWERCASE , DIGIT } char_type;


// declarations 
char encrypt_or_decrypt(bool to_decrypt, char character, int key, char_type type);
char_type get_char_type(char character);




char encrypt_or_decrypt(bool to_decrypt, char character, int key, char_type type) 
{
	int decrypt_mul = 1;
	int modulo_num;
	char result_char, ref_char;

	switch (type)
	{
	case UPPERCASE:

	case LOWERCASE:

	case DIGIT:

	}

	if (!to_decrypt)
		decrypt_mul = -1;

	result_char = ref_char + (character - ref_char - decrypt_mul * key) % modulo_num;


	return result_char;

}
