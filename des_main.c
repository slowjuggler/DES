#include <stdio.h>
#include <string.h>
#include <math.h>
#include "des.h"

int main() {
	char input8[MAX_SIZE] = {0};
	char key8[9] = {0};
	printf("Please input string to encode: ");
	input(input8);
	int len = strlen(input8);
	if (len%8 != 0) {
		for (int i = 0; i < (8 - len%8); i++) {
			*(input8 + len + i) = '0';
		}
	}
	*(input8 + len + 8 - len%8) = '\0';
	printf("Please input 8-key (64 bit key block): ");
	input(key8);
	encode(input8, key8);
    return 0;
}
