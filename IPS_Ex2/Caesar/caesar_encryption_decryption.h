#ifndef CAESAR_ENCRYPTION_DECRYPTION_H
#define CAESAR_ENCRYPTION_DECRYPTION_H

// include headers ------------------------------------------------------------
#include <stdbool.h>

// functions declarations -----------------------------------------------------

/// line_cipher_execute
/// inputs:  line , to_decrypt , key
/// outputs: line
/// summary: return line after encrypting / decrypting line (in place) 
char* line_cipher_execute(char* line, bool to_decrypt, int key);

#endif // CAESAR_ENCRYPTION_DECRYPTION_H
