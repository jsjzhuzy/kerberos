#include "DesCrypt.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
using namespace std;

DesCrypt::DesCrypt()
{
    this->endata = "";
    this->dedata = "";
}

void DesCrypt::setKey(char *key)
{
    if(strlen(key)==64){
        memcpy(this->key,key,64);
    }
    if(strlen(key)==8){
        _u8to64(key,this->key);
    }
    this->endata = "";
    this->dedata = "";
//    this->key = key;
}

DesCrypt::~DesCrypt()
{
    free(this->key);
}

const int DesCrypt::IP[64] = { 57,49,41,33,25,17,9,1,
                               59,51,43,35,27,19,11,3,
                               61,53,45,37,29,21,13,5,
                               63,55,47,39,31,23,15,7,
                               56,48,40,32,24,16,8,0,
                               58,50,42,34,26,18,10,2,
                               60,52,44,36,28,20,12,4,
                               62,54,46,38,30,22,14,6};
const int DesCrypt::IP_1[64] = {39,7,47,15,55,23,63,31,
                                38,6,46,14,54,22,62,30,
                                37,5,45,13,53,21,61,29,
                                36,4,44,12,52,20,60,28,
                                35,3,43,11,51,19,59,27,
                                34,2,42,10,50,18,58,26,
                                33,1,41,9,49,17,57,25,
                                32,0,40,8,48,16,56,24};
const int DesCrypt::E[64] = {31, 0, 1, 2, 3, 4,
                             3, 4, 5, 6, 7, 8,
                             7, 8,9,10,11,12,
                             11,12,13,14,15,16,
                             15,16,17,18,19,20,
                             19,20,21,22,23,24,
                             23,24,25,26,27,28,
                             27,28,29,30,31, 0};
const int DesCrypt::P[32] = {15,6,19,20,28,11,27,16,
                             0,14,22,25,4,17,30,9,
                             1,7,23,13,31,26,2,8,
                             18,12,29,5,21,10,3,24};
const int DesCrypt::S[8][4][16] = {{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
                                    {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
                                    {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
                                    {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
                                    /* S2 */
                                    {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
                                    {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
                                    {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
                                    {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
                                    /* S3 */
                                    {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
                                    {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
                                    {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
                                    {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
                                    /* S4 */
                                    {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
                                    {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
                                    {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
                                    {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
                                    /* S5 */
                                    {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
                                    {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
                                    {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
                                    {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
                                    /* S6 */
                                    {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
                                    {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
                                    {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
                                    {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
                                    /* S7 */
                                    {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
                                    {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
                                    {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
                                    {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
                                    /* S8 */
                                    {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
                                    {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
                                    {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
                                    {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};
const int DesCrypt::PC_1[56] = {56,48,40,32,24,16,8,
                                0,57,49,41,33,25,17,
                                9,1,58,50,42,34,26,
                                18,10,2,59,51,43,35,
                                62,54,46,38,30,22,14,
                                6,61,53,45,37,29,21,
                                13,5,60,52,44,36,28,
                                20,12,4,27,19,11,3};
const int DesCrypt::PC_2[56] = {13,16,10,23,0,4,2,27,
                                14,5,20,9,22,18,11,3,
                                25,7,15,6,26,19,12,1,
                                40,51,30,36,46,54,29,39,
                                50,44,32,47,43,48,38,55,
                                33,52,45,41,49,35,28,31};
const int DesCrypt::MOV[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

void DesCrypt::hextobin(char h[16], char bit[64]) /* hex to binary */
{
    int i;
    int s(0);
    char tmp[2];
    for(i=0; i<8; i++){
        memcpy(tmp,h+(i<<1),2);
        if(isdigit(tmp[0]))  s = (s + tmp[0] - '0')<<4;
        else if((('a'<=tmp[0])&&(tmp[0]<='f'))||(('A'<=tmp[0])&&(tmp[0]<='F')))
            s = (s + tmp[0] - 'a' + 10) << 4;
        else continue; /* never goto here */

        if(isdigit(tmp[1]))  s += tmp[1] - '0';
        else if((('a'<=tmp[1])&&(tmp[1]<='f'))||(('A'<=tmp[1])&&(tmp[1]<='F')))
            s += tmp[1] - 'a' + 10;
        else continue; /* never goto here */
        _bytetobit((char)s,bit+(i<<3));
        s = 0;
    }
}

void DesCrypt::bintohex(char bit[64], char h[16]) /* binary to hex */
{
    int i,s(0);
    for(i=0; i<16; i++){
        s += ((bit[(i<<2)+0]-'0')<<3) + ((bit[(i<<2)+1]-'0')<<2) + ((bit[(i<<2)+2]-'0')<<1) + (bit[(i<<2)+3]-'0');
        sprintf(h+i,"%x",s);
        s = 0;
    }
}

void DesCrypt::_bytetobit(char ch, char bit[8])/* 1byte to 8bit */
{
    int i;
    int c = (char)ch;
    for(i=0; i<=7; i++){
        bit[7-i] = (c&0x01)+'0';
        c >>= 1;
    }
}

void DesCrypt::_bittobyte(char bit[8], char *ch) /* 8bit to 1byte */
{
    int i,c(0);
    for(i=0; i<8; i++){
        c += (*(bit+i)-'0')<<(7-i);
    }
    *ch = (char)c;
}

void DesCrypt::_u8to64(char ch[8], char bit[64])/* 8*8 to 64 bit */
{
    int i;
    for(i=0; i<8; i++){
        _bytetobit(*(ch+i),bit+(i<<3));
    }
}

void DesCrypt::_64tou8(char bit[64], char ch[8])/* 8bit to char */
{
    int i;
    for(i=0; i<8; i++){
        _bittobyte(bit+(i<<3),ch+i);
    }
}

void DesCrypt::IP_Trans(char data[64]) /* IP Trans */
{
    int i;
    char tmp[64];
    for(i = 0; i < 64; i++){
        tmp[i] = data[IP[i]];
    }
    memcpy(data,tmp,64);
}

void DesCrypt::IP_1_Trans(char data[64]) /*IP traverse Trans*/
{
    int i;
    char tmp[64];
    for(i = 0; i < 64; i++){
        tmp[i] = data[IP_1[i]];
    }
    memcpy(data,tmp,64);
}

void DesCrypt::E_Trans(char data[48]) /* E Trans */
{
    int i;
    char tmp[48];
    for(i = 0; i < 48; i++){
        tmp[i] = data[E[i]];
    }
    memcpy(data,tmp,48);
}

void DesCrypt::P_Trans(char data[32]) /* P Trans */
{
    int i;
    char tmp[32];
    for(i = 0; i < 32; i++){
        tmp[i] = data[P[i]];
    }
    memcpy(data,tmp,32);
}

void DesCrypt::S_Trans(char data[48]) /* S box Trans */
{
    int i;
    int line,row,out;
    int j(0),k(0);
    for(i=0; i<8; i++){
        j = i*6;
        k = i<<2;

        /* calculate the pos in S_BOX */
        line = ((data[j]-'0')<<1) + (data[j+5]-'0');
        row = ((data[j+1]-'0')<<3) + ((data[j+2]-'0')<<2) + ((data[j+3]-'0')<<1) + (data[j+4]-'0');

        out = S[i][line][row];
        /* to binary */
        data[k] = ((out&0X08)>>3)+'0';
        data[k+1] = ((out&0X04)>>2)+'0';
        data[k+2] = ((out&0X02)>>1)+'0';
        data[k+3] = (out&0x01)+'0';
    }
}

void DesCrypt::PC_1_Trans(char key[64], char bit[56]) /* PC_1 Trans */
{
    int i;
    for(i = 0; i < 56; i++){
        bit[i] = key[PC_1[i]];
    }
}

void DesCrypt::PC_2_Trans(const char key[56], char bit[48]) /* PC_2 Trans */
{
    int i;
    for(i = 0; i < 48; i++){
       bit[i] = key[PC_2[i]];
    }
}

void DesCrypt::ROL(char data[56], int time) /* cycle shift */
{
    char tmp[56];
    /* save the bit which mov to right */
    memcpy(tmp,data,time);
    memcpy(tmp+time,data+28,time);
    /* mov 1-28 */
    memcpy(data,data+time,28-time);
    memcpy(data+28-time,tmp,time);
    /* mov 29-56 */
    memcpy(data+28,data+28+time,28-time);
    memcpy(data+56-time,tmp+time,time);
}

void DesCrypt::XOR(char R[48], char L[48], int times) /* XOR */
{
    int i;
    for(i = 0; i < times; i++){
        R[i] = (R[i]^L[i]) + '0';
    }
}

void DesCrypt::SWAP(char L[32], char R[32]) /* swap */
{
    char tmp[32];
    memcpy(tmp,L,32);
    memcpy(L,R,32);
    memcpy(R,tmp,32);
}

void DesCrypt::subKeys(char key[64], char subKeys[16][48]) /* make the subkeys */
{
    char tmp[56];
    int i;
    PC_1_Trans(key,tmp); /* PC1 Trans */
    /*reduce 16 times*/
    for(i=0; i<16; i++){
        ROL(tmp,MOV[i]); /* mov left */
        PC_2_Trans(tmp,subKeys[i]);/* PC2 Trans to make subkeys */
    }
}

void DesCrypt::encrypt64bit(char data[8], char subKeys[16][48], char cipher[8])
{
    char plainbit[64];
    char right[48];
    int i;

    _u8to64(data,plainbit);
    IP_Trans(plainbit);
    for(i = 0; i < 16; i++){
        memcpy(right,plainbit+32,32);

        E_Trans(right);
        XOR(right,subKeys[i],48);
        S_Trans(right);
        P_Trans(right);

        XOR(plainbit,right,32);
        if(i != 15){
            SWAP(plainbit,plainbit+32);
        }
    }
    IP_1_Trans(plainbit);
    _64tou8(plainbit,cipher);
    char c[16];
    bintohex(plainbit,c);
    string str(&c[0],&c[16]);
    this->endata += str;
}

void DesCrypt::decrypt64bit(char cipher[64], char subKeys[16][48], char data[8])
{
    char cipherbit[64];
    memcpy(cipherbit,cipher,64);
    char right[48];
    int i;
    IP_Trans(cipherbit);

    for(i = 15; i >= 0; i--){
        memcpy(right,cipherbit+32,32);

        E_Trans(right);
        XOR(right,subKeys[i],48);
        S_Trans(right);
        P_Trans(right);

        XOR(cipherbit,right,32);
        if(i != 0){
            SWAP(cipherbit,cipherbit+32);
        }
    }
    IP_1_Trans(cipherbit);
    _64tou8(cipherbit,data);
    string str(&data[0],&data[8]);
    this->dedata += str;
}

void DesCrypt::encrypt(char *data)
{
    char plainBlock[8],cipher[8];
    char subk[16][48];
    char eof = '\0';
    long i(0);

    subKeys(this->key,subk);

    long len = strlen(data);
    while(len%8 != 0){
        strcat(data,&eof);
        len++;
    }
    for(i=0; i<len/8; i++){
        memcpy(plainBlock,data+(i<<3),8);
        encrypt64bit(plainBlock,subk,cipher);
    }
}

void DesCrypt::decrypt(char *data)
{
    char plainBlock[8],cipher[64],tmp[16];
    char subk[16][48];
    char eof = '\0';
    long i(0);

    subKeys(this->key,subk);
    long len = strlen(data);
    while(len%16 != 0){
        strcat(data,&eof);
        len++;
    }
    for(i=0; i<len/16; i++){
        memcpy(tmp,data+(i<<4),16);
        hextobin(tmp,cipher);
        decrypt64bit(cipher,subk,plainBlock);
    }
}
