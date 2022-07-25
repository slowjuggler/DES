#ifndef DES_H
#define DES_H

#define MAX_SIZE 65536

int rnd_time[16];
char IP64[8][8];
char revIP64[8][8];
char tabS[8][64];
char PC_1[8][7];
char PC_2[8][6];
char E[8][6];
char P[4][8];

void bytebit(char *input8, char *output64);
void bitbyte(char *input64, unsigned short *output8_char);
void permutation(char *input, char *output, int lenth, char *table);
void shifting(char *input, char *output, int lenth, int cicle_count);
void xoring(char* input_1, char* input_2, int lenth, char* output);
void key(char* k64, char (*sk48)[49], int *round);
void rfunc(char* left, char* right, char* sk);
void encode(char *input8, char *key8);
void input(char *buffer, int len);

#endif //  DES_H
