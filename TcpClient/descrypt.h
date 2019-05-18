#ifndef DESCRYPT_H
#define DESCRYPT_H
#include <string>
#include <iostream>
#include <cstdlib>

using std::string;

class DesCrypt
{
public:
    DesCrypt();
    ~DesCrypt();
    void setKey(char *key);
    void encrypt(char *data);
    void decrypt(char *data);

private:
    static void inline hextobin(char h[16], char bit[64]); /* hex to binary */
    static void inline bintohex(char bit[64], char h[16]); /* binary to hex */
    static void inline _bytetobit(char ch, char bit[8]);   /* 1byte to 8bit */
    static void inline _bittobyte(char bit[8], char* ch); /* 8bit to char */
    static void inline _u8to64(char ch[8], char bit[64]); /* 8*8 to 64 bit */
    static void inline _64tou8(char bit[64], char ch[8]); /* 64 bit to 8*8 */
    static void inline IP_Trans(char data[64]);   /*IP transform*/
    static void inline IP_1_Trans(char data[64]); /*IP traverse transform*/
    static void inline E_Trans(char data[48]);    /* E transform */
    static void inline P_Trans(char data[32]);    /* P transform */
    static void inline S_Trans(char data[48]);    /* S box transform */
    static void inline PC_1_Trans(char key[64], char bit[56]);       /* PC_1 transform */
    static void inline PC_2_Trans(const char key[56], char bit[48]); /* PC_2 transform */
    static void inline ROL(char data[56], int time);                 /* cycle shift */
    static void inline XOR(char R[48], char L[48], int times);       /* XOR */
    static void inline SWAP(char L[32], char R[32]);  /* swap */
    void subKeys(char key[64], char subkeys[16][48]);  /* make the subkeys */
    void encrypt64bit(char data[8], char subkeys[16][48], char cipher[8]); /* encrypt a block */
    void decrypt64bit(char data[8], char subkeys[16][48], char cipher[8]); /* decrypt a block */

public:
    string endata;
    string dedata;
private:
    char *key = (char*)malloc(64);
    static const int IP[64];
    static const int IP_1[64];
    static const int E[64];
    static const int P[32];
    static const int S[8][4][16];
    static const int PC_1[56];
    static const int PC_2[56];
    static const int MOV[16];
};
#endif // DESCRYPT_H
