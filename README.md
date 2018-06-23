# Extended Number System
Here's the Extended Number System That I am Working On

## BigInt: Big Integer Implementation in C++

In C++, numbers are stored in either 32- or 64-bit architecture. Therefore, it is sometimes difficult for people who want to utilize integers exceeding this figure. My project tries to address this problem by introducing an extended number system that literally allows a large number of bits for integers (within the limit of being able to be stored by processor, of course). 

Update: after some consideration I decided to use base 2 storage of digits instead of base (2^31). There are two reasons to do so: 
-it simulates more closely on how a compiler performs arithmetic operations. 
-For my next part of project (NumRing), I decided to do efficient exponentiation, which will be beneficial if the bitwise operations can be efficiently and conveniently done here. 

Current progress: basic input/output and arithmetic done, but still plenty of rooms for optimization. Test cases written but haven't really tested them extensively omn my code. 

## NumRing: ring of number modulo n (presented as the field 'base'). 

Just started to do this new part of my project. An interesting feature is that for this ring, those numbers relatively prime to n has an inverse. Thus division of two numbers (with divisor relatively prime to n) is an integer. 
