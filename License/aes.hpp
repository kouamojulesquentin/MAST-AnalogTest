#ifndef _AES_H
#define _AES_H

#ifndef uint8
#define uint8  unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

typedef struct
{
    uint32 erk[64];     /* encryption round keys */
    uint32 drk[64];     /* decryption round keys */
    int nr;             /* number of rounds */
}
aes_context;

extern "C" int  aes_set_key( aes_context *ctx, uint8 *key, int nbits );
extern "C" void aes_encrypt( aes_context *ctx, uint8 input[16], uint8 output[16] );
extern "C" void aes_decrypt( aes_context *ctx, uint8 input[16], uint8 output[16] );

unsigned char MAST_master_key[32] = {0x9f, 0xdb, 0x6b, 0xb2, 0xc5, 0x32, 0xb7, 0x7f, 0x4f, 0x95, 0xc8, 0x70, 0x86, 0x68, 0xdf, 0xea, 0x96, 0xfb, 0xe7, 0x11, 0x58, 0xab, 0x6c, 0xe8, 0x66, 0xce, 0x80, 0x56, 0xb1, 0x93, 0xcd, 0x7d};

#endif /* aes.h */
