#ifndef CRYPTO_H
#define CRYPTO_H

void DES8encrypter(int message[],int key[]);
void DES8decrypter(int cyphertxt[],int key[]);
void Des12Encrypter(int key[],int message[]);
void Des12Decrypter(int key[],int message[]);
void RsaEncrypter(int publickey,int message[]);
void RsaDecrypter(int privatekey,int message[]);
void Hcbc(int key[],int message[],int output[]);

#endif