#ifndef DES_H
#define DES_H

#include <stdint.h>
#include <string.h>

/*  Source
    https://www.geeksforgeeks.org/computer-networks/data-encryption-standard-des-set-1/
    https://ritul-patidar.medium.com/key-expansion-function-and-key-schedule-of-des-data-encryption-standard-algorithm-1bfc7476157
*/

void permute_bits(const uint8_t *input, uint8_t *output, const int *table, int size);
void left_shift(uint8_t *data, int shifts);
void generate_sub_keys(const uint8_t key[8], uint8_t sub_keys[16][6]);
void feistel_function(const uint8_t r[4], uint8_t f_out[4], const uint8_t sub_key[6]);
void des_crypt(const uint8_t input[8], uint8_t output[8], const uint8_t sub_keys[16][6], int mode);
int des_encrypt_ecb(const uint8_t *plaintext, int plaintext_len, const uint8_t key[8], uint8_t *ciphertext);
int des_decrypt_ecb(const uint8_t *ciphertext, int ciphertext_len, const uint8_t key[8], uint8_t *decryptedtext);

#endif