/*
 *	@(#)%Portal Version: psiu_crypt.h:CUPmod7.3PatchInt:1:2006-Oct-16 03:05:33 %
 *      
* Copyright (c) 1996, 2023, Oracle and/or its affiliates.
 *      
 *      This material is the confidential property of Oracle Corporation or its
 *      licensors and may be used, reproduced, stored or transmitted only in
 *      accordance with a valid Oracle license or sublicense agreement.
 */

#ifndef _PSIU_CRYPT_H_
#define _PSIU_CRYPT_H_

#include "pcm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PSIU_CRYPT_META_CHAR    '&'

typedef void psiu_crypt_ctx_t;

#ifdef PSIU_EXPORT
  #define PSIU_API		PIN_EXPORT
#else
  #define PSIU_API		PIN_IMPORT
#endif

PSIU_API void *get_psiu_encrypt_func_ptr();
PSIU_API void set_psiu_encrypt_func_ptr(void *);
PSIU_API void *get_psiu_decrypt_func_ptr();
PSIU_API void set_psiu_decrypt_func_ptr(void *);
PSIU_API void *get_psiu_crypt_scheme_multi_func_ptr();
PSIU_API void set_psiu_crypt_scheme_multi_func_ptr(void *);
PSIU_API void *get_psiu_decrypt_with_key_func_ptr();
PSIU_API void set_psiu_decrypt_with_key_func_ptr(void *);
PSIU_API void *get_psiu_encrypt_with_key_func_ptr();
PSIU_API void set_psiu_encrypt_with_key_func_ptr(void *);
PSIU_API int32 *get_psiu_get_brm_root_key_func_ptr();
PSIU_API void set_psiu_get_brm_root_key_func_ptr(int32 *);



typedef void (*psiu_encrypt_func_ptr_t)(psiu_crypt_ctx_t *,
		      const void *, const size_t, void **,
		      size_t *, pin_errbuf_t *);

typedef void (*psiu_decrypt_func_ptr_t)(psiu_crypt_ctx_t *,
		      const void *, const size_t, void **,
		      size_t *, pin_errbuf_t *);

typedef void (*psiu_crypt_scheme_multi_func_ptr_t)(psiu_crypt_ctx_t *,
			const void ***ptr, void *ptr1,
			int is_aes, pin_errbuf_t *ebufp);

typedef void (*psiu_decrypt_with_key_func_ptr_t)(psiu_crypt_ctx_t *,
			const void*,
        		const void*,
        		const void *,
        		const size_t,
        		void **,
        		size_t *,
        		int,
        		pin_errbuf_t *);

typedef void (*psiu_encrypt_with_key_func_ptr_t)(psiu_crypt_ctx_t *,
        const void* ,
        const void* ,
        const void *,
        const size_t ,
        void **,
        size_t *,
        int ,
        pin_errbuf_t *);

typedef int32 (*psiu_get_brm_root_key_func_ptr_t)(char* key_name, 
			u_char* bufferp, 
			int32 buffer_length, 
			char* wallet_locationp, 
			char* error_bufferp, 
			int32 error_buffer_len);


#if defined(__STDC__) || defined(PIN_USE_ANSI_HDRS) || defined(__cplusplus)

/************************************************************************
 * Used to set cryptography module parameters based on the encryption
 * scheme. The encryption scheme, 'schemep', is specified in the pin.conf 
 * syntax notation for the cryptography module. The optional argument
 * 'ctxp' is used to specify a cryptographic context.
 * 
 * Note that if this function is not called prior to encryption and 
 * decryption calls, then the cryptography module will try to initialize 
 * the parameters by looking for encryption scheme specifications in 
 * actual pin.conf files.
 ************************************************************************/
PIN_EXPORT void
psiu_crypt_scheme(psiu_crypt_ctx_t *ctxp,
	const char *schemep,
	pin_errbuf_t *ebufp);

PIN_EXPORT void
psiu_crypt_scheme_multi(psiu_crypt_ctx_t *ctxp,
	const void ***ptr, void *ptr1,
	int is_aes, pin_errbuf_t *ebufp);

PIN_EXPORT void
psiu_crypt_scheme_malloc(psiu_crypt_ctx_t *ctxp,
        const char *schemep,
        pin_errbuf_t *ebufp);

PIN_EXPORT void
psiu_crypt_scheme_multi_malloc(psiu_crypt_ctx_t *ctxp,
        const void ***ptr, void *ptr1,
        int is_aes, pin_errbuf_t *ebufp);
/************************************************************************
 * Encrypts an 'input' buffer of given length, 'inlen', using a stream 
 * cipher. Stores the result in 'output' after allocating memory for it.
 * The length of the output buffer is returned in 'outlenp'. The optional
 * argument 'ctxp' may be used to provide a cryptographic context.
 ************************************************************************/
PIN_EXPORT void
psiu_encrypt(psiu_crypt_ctx_t *ctxp,
	const void *inp,
	const size_t inlen,
	void **outpp,
	size_t *outlenp,
	pin_errbuf_t *ebufp);

PIN_EXPORT void
psiu_encrypt_with_key(psiu_crypt_ctx_t *ctxp,
	const void* scheme,
	const void* ip_key,
	const void *inp,
	const size_t inlen,
	void **outpp,
	size_t *outlenp,
	int is_hidden,
	pin_errbuf_t *ebufp);

PIN_EXPORT int
psiu_encrypt_pw(const char* plain, 
	char** cipher,
	pin_errbuf_t *ebufp);
PIN_EXPORT int
psiu_encrypt_pw_zt(const char* plain,
        char** cipher,
        pin_errbuf_t *ebufp);
PIN_EXPORT int
psiu_encrypt_pw_zt_malloc(const char* plain,
        char** cipher,
        pin_errbuf_t *ebufp);


PIN_EXPORT void
psiu_encrypt_malloc(psiu_crypt_ctx_t *ctxp,
        const void *inp,
        const size_t inlen,
        void **outpp,
        size_t *outlenp,
        pin_errbuf_t *ebufp);

PIN_EXPORT void
psiu_encrypt_with_key_malloc(psiu_crypt_ctx_t *ctxp,
        const void* scheme,
        const void* ip_key,
        const void *inp,
        const size_t inlen,
        void **outpp,
        size_t *outlenp,
        int is_hidden,
        pin_errbuf_t *ebufp);

PIN_EXPORT int
psiu_encrypt_pw_malloc(const char* plain,
        char** cipher,
        pin_errbuf_t *ebufp);

/************************************************************************
 * Decrypts an 'input' buffer of given length, 'inlen', using a stream
 * cipher. Stores the result in 'output' after allocating memory for it.
 * The length of the output buffer is returned in 'outlenp'. The optional
 * argument 'ctxp' may be used to provide a cryptographic context.
 ************************************************************************/
PIN_EXPORT void
psiu_decrypt(psiu_crypt_ctx_t *ctxp,
	const void *inp,
	const size_t inlen,
	void **outpp,
	size_t *outlenp,
	pin_errbuf_t *ebufp);

PIN_EXPORT void
psiu_decrypt_with_key(psiu_crypt_ctx_t *ctxp,
	const void* scheme,
	const void* ip_key,
	const void *inp,
	const size_t inlen,
	void **outpp,
	size_t *outlenp,
	int is_hidden,
	pin_errbuf_t *ebufp);

PIN_EXPORT int
psiu_decrypt_pw(const char* cipher, 
	char** plain,
	pin_errbuf_t *ebufp);

PIN_EXPORT void
psiu_decrypt_malloc(psiu_crypt_ctx_t *ctxp,
        const void *inp,
        const size_t inlen,
        void **outpp,
        size_t *outlenp,
        pin_errbuf_t *ebufp);

PIN_EXPORT void
psiu_decrypt_with_key_malloc(psiu_crypt_ctx_t *ctxp,
        const void* scheme,
        const void* ip_key,
        const void *inp,
        const size_t inlen,
        void **outpp,
        size_t *outlenp,
        int is_hidden,
        pin_errbuf_t *ebufp);

PIN_EXPORT int
psiu_decrypt_pw_malloc(const char* cipher,
        char** plain,
        pin_errbuf_t *ebufp);

PIN_EXPORT int32 get_brm_root_key(char* key_name, char* bufferp, int32 buffer_length, char* wallet_locationp, char* error_bufferp, int32 error_buffer_len);
PIN_EXPORT int32 set_brm_root_key(psiu_crypt_ctx_t   *ctxp , char* key_name, char* keyp, char* password, char* wallet_locationp, int32 replace, char* error_bufferp, int32 error_buffer_len);
PIN_EXPORT int32 get_brm_conf_entry(char* key_name , char* password, char* bufferp, int32 buffer_length, char* wallet_locationp, char* error_bufferp, int32 error_buffer_len);
PIN_EXPORT int32 set_brm_conf_entry(char* key_name, char* keyp,  char* password, char* wallet_locationp, char* error_bufferp, int32 error_buffer_len);
#else /*__STDC__*/

PIN_EXPORT void psiu_crypt_scheme();
PIN_EXPORT void psiu_crypt_scheme_multi();
PIN_EXPORT void psiu_encrypt();
PIN_EXPORT void psiu_decrypt();
PIN_EXPORT int psiu_encrypt_pw();
PIN_EXPORT int psiu_decrypt_pw();
PIN_EXPORT void psiu_encrypt_with_key();
PIN_EXPORT void psiu_decrypt_with_key();
PIN_EXPORT void psiu_crypt_scheme_malloc();
PIN_EXPORT void psiu_crypt_scheme_multi_malloc();
PIN_EXPORT void psiu_encrypt_malloc();
PIN_EXPORT void psiu_decrypt_malloc();
PIN_EXPORT int psiu_encrypt_pw_malloc();
PIN_EXPORT int psiu_decrypt_pw_malloc();
PIN_EXPORT void psiu_encrypt_with_key_malloc();
PIN_EXPORT void psiu_decrypt_with_key_malloc();

#endif /*__STDC__*/

#ifdef __cplusplus
}
#endif

#endif /*_PSIU_CRYPT_H_*/

