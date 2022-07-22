#include <stdio.h>
#include <string.h>
#include <math.h>
#include "des.h"

int rnd_time[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1}; // round counter

char IP64[8][8] = {
	{58,50,42,34,26,18,10,2}, // initial 64 bit permutation table
	{60,52,44,36,28,20,12,4},
	{62,54,46,38,30,22,14,6},
	{64,56,48,40,32,24,16,8},
	{57,49,41,33,25,17,9,1},
	{59,51,43,35,27,19,11,3},
	{61,53,45,37,29,21,13,5},
	{63,55,47,39,31,23,15,7}
};
				   
char revIP64[8][8] = {
    {40,8,48,16,56,24,64,32}, // reverse 64 bit permutation table
    {39,7,47,15,55,23,63,31},
    {38,6,46,14,54,22,62,30},
    {37,5,45,13,53,21,61,29},
    {36,4,44,12,52,20,60,28},
    {35,3,43,11,51,19,59,27},
    {34,2,42,10,50,18,58,26},
    {33,1,41,9,49,17,57,25}
};
    
char tabS[8][64] = {
    {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,  // 64 bit S tables (8 tables for 64 bit)
      0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
      4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
      15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
    {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
      3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
      0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
     13,8,10,1,3,15,4,2,11,6,7,12,10,5,14,9},
    {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
     13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
     13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
      1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12},
    { 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
     13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
     10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
      3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14},
    { 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
     14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
      4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
     11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3},
    {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
     10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
      9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
      4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13},
    { 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
     13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
      1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
      6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12},
    {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
      1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
      7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
      2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
};

char PC_1[8][7] = {
    {57,49,41,33,25,17,9}, // 56 bit PC-1 table
    {1,58,50,42,34,26,18},
    {10,2,59,51,43,35,27},
    {19,11,3,60,52,44,36},
    {63,55,47,39,31,23,15},
    {7,62,54,46,38,30,22},
    {14,6,61,53,45,37,29},
    {21,13,5,28,20,12,4}
};

char PC_2[8][6] = {
    {14,17,11,24,1,5}, // 48 bit PC-2 table
    {3,28,15,6,21,10},
    {23,19,12,4,26,8},
    {16,7,27,20,13,2},
    {41,52,31,37,47,55},
    {30,40,51,45,33,48},
    {44,49,39,56,34,53},
    {46,42,50,36,29,32}
};

char E[8][6] = {
    {32,1,2,3,4,5}, // 48 bit E table
    {4,5,6,7,8,9},
    {8,9,10,11,12,13},
    {12,13,14,15,16,17},
    {16,17,18,19,20,21},
    {20,21,22,23,24,25},
    {24,25,26,27,28,29},
    {28,29,30,31,32,1}
};

char P[4][8] = {
    {16,7,20,21,29,12,28,17}, // 32 bit P table
    {1,15,23,26,5,18,31,10},
    {2,8,24,14,32,27,3,9},
    {19,13,30,6,22,11,4,25}
};

void permutation(char *input, char *output, int lenth, char *table) {
    int i = 0;
    for(i = 0; i < lenth; i++) {
        *(output + i) = *(input + *(table + i) - 1);
    }
    *(output + i) = '\0';
}

void shifting(char *input, char *output, int lenth, int cicle_count) {
    int i = 0;
    for(i = 0; i < lenth; i++) {
        *(output + i) = *(input + (cicle_count + i) % lenth);
    }
    *(output + i)='\0';
}

void xoring(char* input_1, char* input_2, int lenth, char* output) {
    int i = 0;
    for(i = 0; i < lenth; i++) {
        if (*(input_1 + i) != *(input_2 + i)) {
		*(output + i) = '1';
	} else {
	    *(output + i) = '0';
	}
    }
}

void bytebit(char *input8, char *output64) {
    int i = 0, k = 0;
    while (i < 64) {
	char temp[8] = {0};
	for (int j = 0; j < 8; j++) {
	    if ((input8[i/8] >> (i%8)) & 0x01) {
		*(temp + j) = '1';		
 	    } else {
		*(temp + j) = '0';
	    }
	    i++;
	}
	for (int j = 7; j >= 0; j--) {
	    output64[k++] = *(temp + j);
	}
    }
    output64[64] = '\0';                               
}

void bitbyte(char *input64, unsigned short *output8_char) {
    int i = 0, k = 0;
    while(input64[i] != '\0') {
	for (int j = 7; j >= 0; j--) { 
	    if (input64[i] == '1') {
		output8_char[k] += pow(2, j) * 1;
	    } else if (input64[i] == '0') {
		output8_char[k] += pow(2, j) * 0;
	    }
	    i++;
	}
	printf("%x ", output8_char[k]);
	k++;
    }
    printf("\n");
    output8_char[8] = '\0';                         
}

void key(char* k64, char (*sk48)[49], int *rnd_time) {
    char output[57], coutput[57], half[29], elf[29], temp[57];
    int j = 0;
    permutation(k64, output, 56, *PC_1);
    strcpy(coutput, output);  
    strcpy(half, output+28);
    for(j = 0; j < 16; j++) {
        shifting(coutput, elf, 28, *(rnd_time + j));
        strcpy(coutput, elf);   
        shifting(half, elf, 28, *(rnd_time + j));
        strcpy(half, elf);                   
        strncpy(temp, coutput, 28);
        strncpy(temp + 28, half, 28);
        permutation(temp, *(sk48 + j), 48, *PC_2);
    }
}

void rfunc(char* left, char* right, char* sk) {
    int i = 0, j = 0;
    int rows, columns;
    char output_1[49] = {0};
    char output_2[49] = {0};
    char output_3[33] = {0};
    char output_4[33] = {0};
    char temp[33] = {0};
    permutation(right, output_1, 48, *E);
    xoring(output_1, sk, 48, output_2);
    for(i = 0; i < 8; i++) {
        rows = ((output_2[i*6] - '0') << 1 ) + (output_2[i*6 + 5] - '0');
        columns = ((output_2[i*6 + 1] - '0') << 3) + ((output_2[i*6 + 2] - '0') << 2) +
         ((output_2[i*6 + 3] - '0') << 1) + (output_2[i*6 + 4] - '0');
	for(j=3; j >= 0; j--) {
           *(output_3 + (i*4 + 3 - j)) = ((tabS[i][rows*16 + columns] >> j) & 1) + '0';
	}
    }
    *(output_3 + 32) = '\0';
    permutation(output_3, output_4, 32, *P);
    strcpy(temp, right);
    xoring(left, output_4, 32, right);
    strcpy(left, temp);
}

void encode(char *input8, char *key8) {	
    unsigned short output8_char[9] = {0};
    char input64[65] = {0};
    char key64[65] = {0};
    char output[65] = {0};
    char left[33] = {0};
    char right[33] = {0};
    char sk[16][49] = {0};
    char cipher[65] = {0};
    bytebit(key8, key64);
    key(key64, sk, rnd_time);
    int i = 0, j = 0;
    int num = strlen(input8) / 8;
    for (j = 0; j < num; j++) {
	char tmpstr[9] = {0};
	strncpy(tmpstr, input8 + j*8, 8);
	bytebit(tmpstr, input64);
	printf("Raw block %d in hex    : ", j + 1);
	memset(output8_char, 0x00, sizeof(output8_char));
	bitbyte(input64, output8_char);	
	permutation(input64, output, 64, *IP64);
	strcpy(left, output);
	strcpy(right, output + 32);
	for(i = 0; i < 16; i++) {
	    rfunc(left, right, *(sk + i));
	}
	strncpy(output, right, 32);
	strncpy(output + 32, left, 32);
	permutation(output, cipher, 64, *revIP64);
	printf("Encoded block %d in hex: ", j + 1);
	memset(output8_char, 0x00, sizeof(output8_char));
	bitbyte(cipher, output8_char);
    }
}

void input(char *buffer) {
    int i = 0;
    char c;
    while ((c = getchar()) != '\n') {
	buffer[i++] = c;
    }
    buffer[i] = '\0';
}

