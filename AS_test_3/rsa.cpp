#include"rsa.h"

std::string RSA_r::sha256(const std::string str)
{
    char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::string newString = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(buf,"%02x",hash[i]);
        newString = newString + buf;
    }
    return newString;
}
char * RSA_r::my_encrypt(char *str,char *path_key){
   char *p_en;
   RSA *p_rsa;
   FILE *file;
   int flen,rsa_len;
   if((file=fopen(path_key,"r"))==NULL){
       perror("open key file error");
       return NULL;
   }
   else{
       printf("open sussecful!");
   }
   if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL){
    //if((p_rsa=PEM_read_RSAPublicKey(file,NULL,NULL,NULL))==NULL){   换成这句死活通不过，无论是否将公钥分离源文件
      ERR_print_errors_fp(stdout);
       return NULL;
   }
   flen=strlen(str);
   rsa_len=RSA_size(p_rsa);
   p_en=(char *)malloc(rsa_len+1);
   memset(p_en,0,rsa_len+1);
   if(RSA_public_encrypt(rsa_len,(unsigned char *)str,(unsigned char*)p_en,p_rsa,RSA_NO_PADDING)<0){
       return NULL;
    }
    RSA_free(p_rsa);
     fclose(file);
     return p_en;
 }
 char * RSA_r::my_decrypt(char *str,char *path_key){
    char *p_de;
     RSA *p_rsa;
     FILE *file;
    int rsa_len;
    if((file=fopen(path_key,"r"))==NULL){
        perror("open key file error");
         return NULL;
    }
    if((p_rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL))==NULL){
        ERR_print_errors_fp(stdout);
        return NULL;
    }
     rsa_len=RSA_size(p_rsa);
     p_de=(char *)malloc(rsa_len+1);
     memset(p_de,0,rsa_len+1);
    if(RSA_private_decrypt(rsa_len,(unsigned char *)str,(unsigned char*)p_de,p_rsa,RSA_NO_PADDING)<0){
         return NULL;
     }
    RSA_free(p_rsa);
     fclose(file);
     return p_de;
 }
