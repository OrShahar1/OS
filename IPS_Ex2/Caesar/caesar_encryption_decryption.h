#ifndef CAESAR_ENCRYPTION_DECRYPTION_H
#define CAESAR_ENCRYPTION_DECRYPTION_H

#include <stdbool.h>

void cipher_exe(FILE* f_input, FILE* f_output, bool to_decrypt, int key, int number_of_thread);
char* line_cipher_execute(char* line, bool to_decrypt, int key);


#endif // CAESAR_ENCRYPTION_DECRYPTION_H
