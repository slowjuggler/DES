all: des_func.c des_main.c des.h
	gcc -o des -Wall -Werror -Wextra des_main.c des_func.c -lm
