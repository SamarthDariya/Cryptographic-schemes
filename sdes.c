# include <stdio.h>
# include "crypto.h"


/*from here the code fragemant is for encryption decryption scheme for 8 bits des*/
int subkey[2][8];

void inpermuter(int permutingarray[]);
void ipinverser(int permutingarray[]);
void swappingfunction(int message[]);
void permute4(int permutingarray[]);
void functionf(int message[],int which);


void permute10(int permutingarray[]);
void shifter(int shiftingarray[]);
void permute8(int permutingarray[],int which);
void subkeygenerator(int key[]);

	int initialpermut[8] = {1,5,2,0,3,7,4,6};//the initial permutation function
	int ipinverse[8] = {3,0,2,4,6,1,7,5};//inverse of initial permutation
	int expandandpermut[8] = {3,0,1,2,1,2,3,0};//expansion fuction for 4 bit to 8 bit
	int permut4[4] = {2,4,3,1};//permutation function for 4 bits
	int permut10[] = {2,4,1,6,3,9,0,8,7,5}; //permutation function for 10 bits
    /*permutation matrix
    input 10 bit output 10 bit */

	int permut8[] = {5,2,6,3,7,4,9,8};
        /*permutation matrix
    input 10 bit output 8 bit */

	int s0[4][4][2] = {
				{{0,1},{0,0},{1,1},{1,0}},
				{{1,1},{1,0},{0,1},{0,0}},
				{{0,0},{1,0},{0,1},{1,1}},
				{{1,1},{0,1},{1,1},{1,0}}
			};//s box 1

	int s1[4][4][2] = {
				{{0,0},{0,1},{1,0},{1,1}},
				{{1,0},{0,0},{0,1},{1,1}},
				{{1,1},{0,0},{0,1},{0,0}},
				{{1,0},{0,1},{0,0},{1,1}}
			};// s box 2

			/*encryption function for DES 8 bits*/
	void DES8encrypter(int message[],int key[]){
		subkeygenerator(key);
    	inpermuter(message);
    	functionf(message,0);
    	swappingfunction(message);
    	functionf(message,1);
    	ipinverser(message);
     }

    /*decryption function for DES 8 bits*/
    void DES8decrypter(int cyphertxt[],int key[]){
    	subkeygenerator(key);
    	inpermuter(cyphertxt);
    	functionf(cyphertxt,1);
    	swappingfunction(cyphertxt);
    	functionf(cyphertxt,0);
    	ipinverser(cyphertxt);
     }
     /*function which performs permutation for 8 bits initially*/
    void inpermuter(int permutingarray[]){
		int swapper[8];
		for(int i =0;i<8;i++){
			swapper[i] = permutingarray[i];
		} 	
		for(int i =0;i<8;i++){
			permutingarray[i] = swapper[initialpermut[i]];
		} 
	}
 /*function which performs permutation for 8 bits finally*/
	void ipinverser(int permutingarray[]){
		int swapper[8];
		for(int i =0;i<8;i++){
			swapper[i] = permutingarray[i];
		} 	
		for(int i =0;i<8;i++){
			permutingarray[i] = swapper[ipinverse[i]];
		} 
	}
    /*code for function f which takes subkey as input with message */
    void functionf(int message[],int which){
    	int leftarr[4], rightarr[4];
    	/*breaking message into left half and right half*/
    	for(int i=0;i<4;i++){
    		leftarr[i]=message[i];
    		rightarr[i]=message[4+i];
    	}
    	/*expanding the right side of message from 4 bits to 8 bits*/
    	int expanded[8];
    	for(int i =0;i<8;i++){
			expanded[i] = rightarr[expandandpermut[i]];
		}
		/*XOR of expanded output with subkey*/
		for(int i =0;i<8;i++){
    		expanded[i]=(expanded[i]+subkey[which][i])%2;
    	}
    	/*breaking expanded output into two halves and then using
    	first and last bit to calculate the row and middle two to 
    	calculate the column of the s box*/
    	int s0row = (2*expanded[0])+expanded[3];
    	int s0col = (2*expanded[1])+expanded[2];
    	int s1row = (2*expanded[4])+expanded[7];
    	int s1col = (2*expanded[5])+expanded[6];
    	/*getting output from s box and catenating to get a 4 bit output
		then permuting it 
    	then XOR it with left end of the array*/
    	int toxor[4];
    	for(int i =0;i<2;i++){
    		toxor[i]=s0[s0row][s0col][i];
    		toxor[i+2]=s1[s1row][s1col][i];
    	}
    	permute4(toxor);
    	for(int i =0;i<4;i++){
    		toxor[i]=(toxor[i]+leftarr[i])%2;
    	}
    	for(int i=0;i<4;i++){
    		message[i]=toxor[i];
    		message[4+i]=rightarr[i];
    	}
    }
    /*function to perform swapping the left and the right bits*/
    	void swappingfunction(int message[]){
	    	int leftarr[4], rightarr[4];
	    	for(int i=0;i<4;i++){
	    		leftarr[i]=message[i];
	    		rightarr[i]=message[4+i];
	    	}
	    	for(int i=0;i<4;i++){
	    		message[i]=rightarr[i];
	    		message[4+i]=leftarr[i];
	    	}
    	}


    	/*fuction used to permute 4 bits*/
    	void permute4(int permutingarray[]){
		int swapper[4];
		for(int i =0;i<4;i++){
			swapper[i] = permutingarray[i];
		} 	
		for(int i =0;i<4;i++){
			permutingarray[i] = swapper[permut4[i]];
		} 
	}
	//subkey generation code begins from here
	void subkeygenerator(int key[]){
		int permuted[10];
			for(int i =0;i<10;i++){
			permuted[i] = key[i];
		}
		//first permutation array changing
		permute10(permuted);
		//single shift array changing
		shifter(permuted);
		//k1 generation array not changing
		permute8(permuted,0);
		//double shift array changing
		shifter(permuted);
		shifter(permuted);
		//k2 generation array not changing
		permute8(permuted,1);
			
	}
	/*function to permute the 10 bits*/
	void permute10(int permutingarray[]){
		int swapper[10];
		for(int i =0;i<10;i++){
			swapper[i] = permutingarray[i];
		} 	
		for(int i =0;i<10;i++){
			permutingarray[i] = swapper[permut10[i]];
		} 
	}
	/*in this we break 10 bits into 5 and 5 and then
	right shift each of the smaller block*/
	void shifter(int shiftingarray[]){
		int shifthelp[10];
		for(int i =0;i<10;i++){
			shifthelp[i]=shiftingarray[i];
		}
		for(int i =0;i<5;i++){
			shiftingarray[i]=shifthelp[(i+1)%5];
		}
		for(int i =5;i<10;i++){
			shiftingarray[i]=shifthelp[5+((i+1)%5)];
		}
	}
	/*function to permute the 10 bits*/
	void permute8(int permutingarray[],int which){
			int swapper[10];
			for(int i =0;i<10;i++){
				swapper[i] = permutingarray[i];
			} 	
			for(int i =0;i<8;i++){
				subkey[which][i] = swapper[permut8[i]];
			} 
		}


/*here the code fragemant for encryption decryption scheme for 8 bits des ends*/




/*from here the code fragemant is for encryption decryption scheme for 12 bits des*/


	void Des12Encrypter(int key[],int message[]){
		int subkey1[8],subkey2[8];
		//generation of subkey
		for (int i = 0; i < 8; i++)
		{
			subkey1[i]=key[i];
			subkey2[i]=key[i+1];
		}
		//breaking message into two blocks
		int left[6],right[6],temp[6];

		for (int i = 0; i < 6; ++i)
		{
			left[i]=message[i];
			right[i]=message[i+6];
			temp[i]=right[i];
		}

		int expansion[8] = {0,1,3,2,3,2,4,5};
		int expanded[8];
		//expanding the right block of the message
		for (int i = 0; i < 8; ++i)
		{
			expanded[i]=right[expansion[i]];
		}
		//XOR of expanded right block of the message with the subkey
		for (int i = 0; i < 8; ++i)
		{
			expanded[i]=(expanded[i]+subkey1[i])%2;
		}

		int s1[2][8][3] = {
							{{1,0,1},{0,1,0},{0,0,1},{1,1,0},{0,1,1},{1,0,0},{1,1,1},{0,0,0}},
							{{0,0,1},{1,0,0},{1,1,0},{0,1,0},{0,0,0},{1,1,1},{1,0,1},{0,1,1}}
				};//s box 1
		int s2[2][8][3] = {
							{{1,0,0},{0,0,0},{1,1,0},{1,0,1},{1,1,1},{0,0,1},{0,1,1},{0,1,0}},
							{{1,0,1},{0,1,1},{0,0,0},{1,1,1},{1,1,0},{0,1,0},{0,0,1},{1,0,0}}
				};// s box 2
				// using first bit to get the row of sbox and next 3 to get the column of the s box
		int row,col;
		row = expanded[0];
		col = 4*expanded[1]+2*expanded[2]+expanded[3];
		//catanation of two outputs of the s boxes
		for (int i = 0; i < 3; ++i)
		{
			right[i]=s1[row][col][i];
		}
		row = expanded[4];
		col = 4*expanded[5]+2*expanded[6]+expanded[7];
		for (int i = 0; i < 3; ++i)
		{
			right[i+3]=s2[row][col][i];
		}
		//XOR the left block of message with the catenated output
		for (int i = 0; i < 6; ++i)
		{
			right[i]=(right[i]+left[i])%2;
		}
		//swapping left and right blocks
		for (int i = 0; i < 6; ++i)
		{
			left[i]=temp[i];
			temp[i]= right[i];
		}
		// repeating the process for round 2
		for (int i = 0; i < 8; ++i)
		{
			expanded[i]=right[expansion[i]];
		}
		for (int i = 0; i < 8; ++i)
		{
			expanded[i]=(expanded[i]+subkey2[i])%2;
		}
		row = expanded[0];
		col = 4*expanded[1]+2*expanded[2]+expanded[3];
		for (int i = 0; i < 3; ++i)
		{
			right[i]=s1[row][col][i];
		}
		row = expanded[4];
		col = 4*expanded[5]+2*expanded[6]+expanded[7];
		for (int i = 0; i < 3; ++i)
		{
			right[i+3]=s2[row][col][i];
		}
		for (int i = 0; i < 6; ++i)
		{
			right[i]=(right[i]+left[i])%2;
		}
		for (int i = 0; i < 6; ++i)
		{
			left[i]=temp[i];
		}	
		for (int i = 0; i < 6; ++i)
		{
			message[i]=right[i];
			message[i+6]=left[i];
		}
	}

	//function for decryption of 12 bit des
	void Des12Decrypter(int key[],int message[]){
		int subkey1[8],subkey2[8];
		//generation of subkey

		for (int i = 0; i < 8; i++)
		{
			subkey1[i]=key[i+1];
			subkey2[i]=key[i];
		}
		//breaking message into two blocks
		int left[6],right[6],temp[6];

		for (int i = 0; i < 6; ++i)
		{
			left[i]=message[i];
			right[i]=message[i+6];
			temp[i]=right[i];
		}
		//expanding the right block of the message
		int expansion[8] = {0,1,3,2,3,2,4,5};
		int expanded[8];

		for (int i = 0; i < 8; ++i)
		{
			expanded[i]=right[expansion[i]];
		}
		//XOR of expanded message with subkey
		for (int i = 0; i < 8; ++i)
		{
			expanded[i]=(expanded[i]+subkey1[i])%2;
		}

		int s1[2][8][3] = {
							{{1,0,1},{0,1,0},{0,0,1},{1,1,0},{0,1,1},{1,0,0},{1,1,1},{0,0,0}},
							{{0,0,1},{1,0,0},{1,1,0},{0,1,0},{0,0,0},{1,1,1},{1,0,1},{0,1,1}}
				};//s box 1
		int s2[2][8][3] = {
							{{1,0,0},{0,0,0},{1,1,0},{1,0,1},{1,1,1},{0,0,1},{0,1,1},{0,1,0}},
							{{1,0,1},{0,1,1},{0,0,0},{1,1,1},{1,1,0},{0,1,0},{0,0,1},{1,0,0}}
				};//s box 2
				// using first bit to get the row of sbox and next 3 to get the column of the s box
		int row,col;
		row = expanded[0];
		col = 4*expanded[1]+2*expanded[2]+expanded[3];

		for (int i = 0; i < 3; ++i)
		{
			right[i]=s1[row][col][i];
		}

		row = expanded[4];
		col = 4*expanded[5]+2*expanded[6]+expanded[7];
		//catenating two outputs of s boxes
		for (int i = 0; i < 3; ++i)
		{
			right[i+3]=s2[row][col][i];
		}
		//XOR of left block of the message with the output
		for (int i = 0; i < 6; ++i)
		{
			right[i]=(right[i]+left[i])%2;
		}
		//swapping
		for (int i = 0; i < 6; ++i)
		{
			left[i]=temp[i];
			temp[i]= right[i];
		}
		//repeating the process for round 2
		for (int i = 0; i < 8; ++i)
		{
			expanded[i]=right[expansion[i]];
		}

		for (int i = 0; i < 8; ++i)
		{
			expanded[i]=(expanded[i]+subkey2[i])%2;
		}
		row = expanded[0];
		col = 4*expanded[1]+2*expanded[2]+expanded[3];

		for (int i = 0; i < 3; ++i)
		{
			right[i]=s1[row][col][i];
		}

		row = expanded[4];
		col = 4*expanded[5]+2*expanded[6]+expanded[7];

		for (int i = 0; i < 3; ++i)
		{
			right[i+3]=s2[row][col][i];
		}

		for (int i = 0; i < 6; ++i)
		{
			right[i]=(right[i]+left[i])%2;
		}

		for (int i = 0; i < 6; ++i)
		{
			left[i]=temp[i];
		}
		
		for (int i = 0; i < 6; ++i)
		{
			message[i]=right[i];
			message[i+6]=left[i];
		}
	}
/*here the code fragemant for encryption decryption scheme for 12 bits des ends*/




/*from here the code fragemant is for encryption decryption scheme for RSA*/
/*
	we have used p = 11 and q = 23 our n = 253 and phi(n) = 220 
	(13,17);(3,147);(7,63);(9,49)
	(21,21);(23,67);(31,71);(19,139)
	are some examples of public private key pairs which can be used
*/
	void RsaEncrypter(int publickey,int message[]){
		int plaintxt = 0,power = 1;
		//calculating the decimal value of message
		for(int i=8;i>0;){
				--i;
				plaintxt += power*message[i];
				power = power*2;
			}
		//caclculation of cypher text
		// calculating (plain text)^public key
		int cyphertxt=1;
		for (int i = 0; i < publickey; ++i)
		{
			cyphertxt = cyphertxt*plaintxt;
			cyphertxt = cyphertxt%253;
		}
		//converting cyphert text into biary format
		cyphertxt = cyphertxt%253;
		for(int i =8;i>0;){
			--i;
			message[i]=cyphertxt%2;
			cyphertxt=cyphertxt/2;
		}
	}


	void RsaDecrypter(int privatekey,int message[]){
		int cyphertxt = 0,power = 1;
		//calculating the decimal value of cypher text
		for(int i=8;i>0;){
				--i;
				cyphertxt += power*message[i];
				power = power*2;
			}
		//caclculation of plain text
		// calculating (cypher text)^private key
		int plaintxt=1;
		for (int i = 0; i < privatekey; ++i)
		{
			plaintxt = plaintxt*cyphertxt;
			plaintxt = plaintxt%253;
		}
		//converting plain text into biary format
		plaintxt = plaintxt%253;
		for(int i =8;i>0;){
			--i;
			message[i]=plaintxt%2;
			plaintxt=plaintxt/2;
		}
	}

/*here the code fragemant for encryption decryption scheme for RSA ends*/




/*from here the code fragemant is for HCBC*/

	void Hcbc(int key[],int message[],int output[]){
			
			int block1[8],block2[8];
			//breaking message into two blocks
			for (int i = 0; i < 16; ++i)
			{
				block1[i]=message[i];
				block2[i]=message[i+8];
			}
			//initial vector
			int iv[8]={0,0,0,0,0,0,0,0};
			//XOR of block 1 and initial vector
			for (int i = 0; i < 8; ++i)
			{
				block1[i]=(block1[i]+iv[i])%2;
			}
			int temp[10];

			for (int i = 0; i < 9; ++i)
			{
				temp[i]= key[i];
			}
			//encryption of block 1 
			DES8encrypter(block1,key);
			//XOR of encrypted block 1 and block 2 
			for (int i = 0; i < 8; ++i)
			{
				block2[i]=(block2[i]+block1[i])%2;
			}
			//encryption of block 1 
			DES8encrypter(block2,temp);
			//storing block 2 into output array
			for (int i = 0; i < 8; ++i)
			{
				output[i]=block2[i];
			}
			int outputnum=0, power =1;
			for(int i=8;i>0;){
				--i;
				outputnum = power*output[i];
				power = power*2;
			}
			//calculate the value of output bits
			/*
			the following is a pre processing step it is done as hcbc ouput is a number 
			less than 256 but we need number less than 253 so if the output is reduced to mod 253
			*/
		if(outputnum==253){
			for (int i = 0; i < 8; ++i)
			{
				output[i]=0;
			}
		}
		if(outputnum == 254){
			for (int i = 0; i < 7; ++i)
			{
				output[i]=0;
			}
			output[7]=1;
		}
		if(outputnum == 255){
			for (int i = 0; i < 8; ++i)
			{
				output[i]=0;
			}
			output[6]=1;
		}
	}
