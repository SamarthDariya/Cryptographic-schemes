# include <stdio.h>
#include "crypto.h"
	


	int main(){
					

					int person;
					printf("____________________________________________________________________\n\n");

					printf("enter 0 if you are Alice\n");
					printf("enter 1 if you are Bob\n");
					printf("enter any other integer to terminate the program\n");
					scanf("%d",&person);
					printf("____________________________________________________________________\n\n");

					
					/*
						user will justify if he is from sender side or reciver side
					*/



						if(person==0){
							/*here is what will happen in senders side*/

							int messagenum,deskeynum,hcbckeynum,publickey;
							int message[12],deskey[9],hcbckey[10];
							/*
							 user will be asked to input message and keys as decimal numbers 
							   then the numbers will be converted in binary format and stored 
							   in respective arrays
							*/
							printf("Hi Alice !!\n");
							printf("____________________________________________________________________\n\n");
							printf("enter the message number which you want to encrypt\n");
							printf("it will be a 12 bit number enter a non negative number  \n");
							printf("the number should be less than 4096\n");
							scanf("%d",&messagenum);
							printf("____________________________________________________________________\n\n");

							/*
							 message is a 12 bits string of 0s and 1s decimal representation 
							of 12 bits numbers is a number smaller than 4096 so a number less than
							4096 is taken as user input
							*/

							for(int i =12;i>0;){
								--i;
								message[i]=messagenum%2;
								messagenum=messagenum/2;
							}
							/*the decimal number is converted in binary representation and stored 
							 in message array 
							*/

							printf("enter the key number for S-DES\n");
							printf("it will be a 9 bit number enter a non negative number  \n");
							printf("the number should be less than 512\n");
							scanf("%d",&deskeynum);
							printf("____________________________________________________________________\n\n");
							/*
							 12 bit SDES Key is a 9 bits string of 0s and 1s decimal representation 
							of 9 bits numbers is a number smaller than 512 so a number less than
							512 is taken as user input
							*/
							for(int i =9;i>0;){
								--i;
								deskey[i]=deskeynum%2;
								deskeynum=deskeynum/2;
							}
							/*the decimal number is converted in binary representation and stored 
							 in deskey array 
							*/
							printf("enter the key number for HCBC\n");
							printf("it will be a 10 bit number enter a non negative number  \n");
							printf("the number should be less than 1024\n");
							scanf("%d",&hcbckeynum);
							printf("____________________________________________________________________\n\n");
							/*
							HCBC Key is basically key for 8 bits SDES which is 
							10 bits string of 0s and 1s decimal representation 
							of 10 bits numbers is a number smaller than 1024 so a number less than
							1024 is taken as user input
							*/
							for(int i =10;i>0;){
								--i;
								hcbckey[i]=hcbckeynum%2;
								hcbckeynum=hcbckeynum/2;
							}
							/*the decimal number is converted in binary representation and stored 
							 in hcbckey array 
							*/
							printf("enter your public key\n");
							scanf("%d",&publickey);
							printf("____________________________________________________________________\n\n");
							/*
								public key is for rsa taken as decimal input for rsa 
								(13,17);(3,147);(7,63);(9,49)
								(21,21);(23,67);(31,71);(19,139)
								are some examples of public private key pairs which can be use
							*/
							printf("your plain text is\n");
							for (int i = 0; i < 12; ++i)
							{
								printf("%d ",message[i]);
							}
							printf("\n");
							printf("____________________________________________________________________\n\n");

							/*printing the plain text*/
							int hcbcinput[16],hcbcoutput[8];
							for(int i =0;i<12;i++)hcbcinput[i]=message[i];
							hcbcinput[12]=0;
							hcbcinput[13]=0;
							hcbcinput[14]=0;
							hcbcinput[15]=1;
							/*Padding of the message to send it to HCBC*/
							Hcbc(hcbckey,hcbcinput,hcbcoutput);
							/*sending the padded message to HCBC function*/
							RsaEncrypter(publickey,hcbcoutput);
							/*Rsa encryption of output of HCBC now the hash output is ready*/
							int sdes1[12],sdes2[12];
							/*creation of sdes input arrays splitting the current string into 
							2 different string also padding them */
							for(int i =0;i<12;i++)sdes1[i]=message[i];
							for(int i =0;i<8;i++)sdes2[i]=hcbcoutput[i];
							sdes2[8]=0;
							sdes2[9]=0;
							sdes2[10]=0;
							sdes2[11]=1;
							
							/*encryption with DES using CBC scheme*/
							int iv[12]={0,0,0,0,0,0,0,0,0,0,0,0};
							/*initial vector*/
							for (int i = 0; i < 12; ++i)
							{
								sdes1[i]=(sdes1[i]+iv[i])%2;
							}
							/*XOR of IV with the first block*/
							Des12Encrypter(deskey,sdes1);//DES encryption of the output after XOR
							/*XOR of DES first block cypher text and the second block*/
							for (int i = 0; i < 12; ++i)
								{
									sdes2[i]=(sdes2[i]+sdes1[i])%2; 
								}
							Des12Encrypter(deskey,sdes2);//DES encryption of the output after XOR

							/*finally storing the cypher text into an array*/
							int cyphertxt[24];
							for (int i = 0; i < 12; ++i)
							{
								cyphertxt[i]=sdes1[i];
								cyphertxt[i+12]=sdes2[i];
							}

							/*calculating the decimal value of Cypher Text*/
							int cyphertxtnum=0;
							for(int i=24,power =1;i>0;){
								--i;
								cyphertxtnum += power*cyphertxt[i];
								power = power*2;
							}


							printf("your cypher text is\n");
											for (int i = 0; i < 24; ++i)
											{
												printf("%d ",cyphertxt[i]);
											}
											printf("\n");
							printf("your encrypted number is %d\n",cyphertxtnum );
							printf("____________________________________________________________________\n\n");

						}






							if(person==1){
								/*here is what will happen in senders side*/
								int cyphertxtnum,deskeynum,hcbckeynum,privatekey;
								int cyphertxt[24],deskey[9],hcbckey[10];
									/*
								 user will be asked to input message and keys as decimal numbers 
								   then the numbers will be converted in binary format and stored 
								   in respective arrays
								*/
								printf("Hi Bob !!\n");
								printf("____________________________________________________________________\n\n");

								printf("enter the cypher text number which you want to decrypt\n");
								printf("it will be a 24 bit number enter a non negative number  \n");
								scanf("%d",&cyphertxtnum);
								printf("____________________________________________________________________\n\n");
								/*
							 message is a 12 bits string of 0s and 1s decimal representation 
							of 12 bits numbers is a number smaller than 4096 so a number less than
							4096 is taken as user input
							*/
								for(int i =24;i>0;){
									--i;
									cyphertxt[i]=cyphertxtnum%2;
									cyphertxtnum=cyphertxtnum/2;
								}
								/*the decimal number is converted in binary representation and stored 
							 in message array 
							*/
								printf("enter the key number for S-DES\n");
								printf("it will be a 9 bit number enter a non negative number  \n");
								printf("the number should be less than 512\n");
								scanf("%d",&deskeynum);
								printf("____________________________________________________________________\n\n");
								/*
							 12 bit SDES Key is a 9 bits string of 0s and 1s decimal representation 
							of 9 bits numbers is a number smaller than 512 so a number less than
							512 is taken as user input
							*/
								for(int i =9;i>0;){
									--i;
									deskey[i]=deskeynum%2;
									deskeynum=deskeynum/2;
								}
								/*the decimal number is converted in binary representation and stored 
							 in deskey array 
							*/

								printf("enter the key number for HCBC\n");
								printf("it will be a 10 bit number enter a non negative number  \n");
								printf("the number should be less than 1024\n");
								scanf("%d",&hcbckeynum);
								printf("____________________________________________________________________\n\n");
								/*
							HCBC Key is basically key for 8 bits SDES which is 
							10 bits string of 0s and 1s decimal representation 
							of 10 bits numbers is a number smaller than 1024 so a number less than
							1024 is taken as user input
							*/
								for(int i =10;i>0;){
									--i;
									hcbckey[i]=hcbckeynum%2;
									hcbckeynum=hcbckeynum/2;
								}
								/*the decimal number is converted in binary representation and stored 
							 in hcbckey array 
							*/

								printf("enter your private key\n");
								scanf("%d",&privatekey);
								printf("____________________________________________________________________\n\n");
								/*the decimal number is converted in binary representation and stored 
							 in hcbckey array 
							*/

								printf("your cypher text is\n");
								for (int i = 0; i < 24; ++i)
								{
									printf("%d ",cyphertxt[i]);
								}
								printf("\n");
								printf("____________________________________________________________________\n\n");
								/*printing the plain text*/

								/*finding the two blocks to be used for CBC mode decryption*/
								int sdes1[12],sdes2[12];
								for (int i = 0; i < 12; ++i)
								{
									sdes1[i]=cyphertxt[i];
									sdes2[i]=cyphertxt[i+12];
								}
								/*decrypting the second block of the cypher text*/
								Des12Decrypter(deskey,sdes2);
								/*XOR the decrypt output with first block of cypher text to get 
								input for next DES decrypt*/
								for (int i = 0; i < 12; ++i)
								{
									sdes2[i]=(sdes2[i]+sdes1[i])%2; 
								}
								/*creating iv vector*/
								int iv[12]={0,0,0,0,0,0,0,0,0,0,0,0};
								Des12Decrypter(deskey,sdes1);
								/*XOR the decrypt output to get the plain text*/
								for (int i = 0; i < 12; ++i)
								{
									sdes1[i]=(sdes1[i]+iv[i])%2; 
								}
								/*calculating value of plain text*/
								int messagenum = 0;
								for(int i=12,power =1;i>0;){
									--i;
									messagenum += power*sdes1[i];
									power = power*2;
								}

								printf("your plain text is\n");
												for (int i = 0; i < 12; ++i)
												{
													printf("%d ",sdes1[i]);
												}
												printf("\n");

								printf("your decrypted number is %d\n",messagenum );
								printf("____________________________________________________________________\n\n");								
								
								int hcbcinput[16],hcbcoutput[8];
								for(int i =0;i<12;i++)hcbcinput[i]=sdes1[i];
								hcbcinput[12]=0;
								hcbcinput[13]=0;
								hcbcinput[14]=0;
								hcbcinput[15]=1;

								int rsainput[8];
								for(int i =0;i<8;i++)rsainput[i]=sdes2[i];

								/*decrypting the hash tag */
								RsaDecrypter(privatekey,rsainput);

								/*checking the hash teg for decrpted message*/
								Hcbc(hcbckey,hcbcinput,hcbcoutput);
								int hcbcnum=0,rsanum=0;
								for(int i=8,power =1;i>0;){
									--i;
									hcbcnum += power*hcbcoutput[i];
									rsanum += power*rsainput[i];
									power = power*2;
								}
								
								if(rsanum==hcbcnum)printf("Authentication Successful\n");
								if(rsanum!=hcbcnum)printf("Authentication Successful \n");
								printf("____________________________________________________________________\n\n");

							}
		
		return 0;
	}