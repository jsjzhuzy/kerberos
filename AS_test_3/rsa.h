#ifndef RSA_H
#define RSA_H

#include<stdio.h>
#include<string>
#include<stdlib.h>
#include<string.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#define OPENSSLKEY "E:\\QtProgram\\AS_test_3\\rsa.key"
#define PUBLICKEY "E:\\QtProgram\\AS_test_3\\pub.key"
#define BUFFSIZE 1024

class RSA_r{
public:
    char* my_encrypt(char *str,char *path_key);//加密
    char* my_decrypt(char *str,char *path_key);//解密
    std::string sha256(const std::string str);//hash
};

#endif // RSA_H
