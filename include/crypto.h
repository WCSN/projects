/***
 *
 * crypto.h
 *
 * Cryptographic functions for licensing
 *
 ***/

#ifndef CRYPTO_H_INCLUDED
#define CRYPTO_H_INCLUDED

#define HASH_LEN 20

#ifdef __cplusplus
extern "C" {
#endif	
/**
 * Calculate hash function for a file 
 * buf should be HASH_LEN in size
 */
void hash_function(char *buf, char *str, char *secret);

/**
 * BASE64 encode and decode
 */
void base64_encode(char *res, int *res_len, char* src, int len);
void base64_decode(char *res, int *res_len, char* src, int len);

/**
 * Simple XOR-based decrypt and encrypt
 */
void xor_encrypt(char *res, char* src, int len);
void xor_decrypt(char *res, char* src, int len);

#ifdef __cplusplus
}
#endif	
#endif
