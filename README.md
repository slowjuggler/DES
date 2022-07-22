1. Description.

Program in C language implementing the DES algorithm in string style (https://en.wikipedia.org/wiki/Data_Encryption_Standard).
Now in its pure form it is not used, but is still quite often used by many companies and payment systems in the form of 3DES. 
You can enter a string of up to 65536 characters. The string will be converted to 64-bit blocks. If the string length is not multiple of 
64 bits, then it will be padded with the required number of 'zeros'.

2. Installation.

On Linux compile the software using "make". On Windows use "make" from Cygwin (http://cygwin.com).

