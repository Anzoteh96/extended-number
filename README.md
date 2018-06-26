# Extended Number System
Here's the Extended Number System That I am Working On

## BigInt: Big Integer Implementation in C++

In C++, numbers are stored in either 32- or 64-bit architecture. Therefore, it is sometimes difficult for people who want to utilize integers exceeding this figure. My project tries to address this problem by introducing an extended number system that literally allows a large number of bits for integers (within the limit of being able to be stored by processor, of course). 

Update: after some consideration I decided to use base 2 storage of digits instead of base (2^31). There are two reasons to do so: 
- it simulates more closely on how a compiler performs arithmetic operations. 
- For my next part of project (NumRing), I decided to do efficient exponentiation, which will be beneficial if the bitwise operations can be efficiently and conveniently done here. 

Status: takes 75.655 seconds in total to go through all the basic arithmetic steps for 100 files (the biggest file, test99 alone takes 1.964 seconds, mainly due to multiplication). All answer correct except test0 where a remainder of 0 is outputted as -0. The next thing to optimize is the multiplication algorithm. 

## NumRing: ring of number modulo n (presented as the field 'base'). 

There are a few features making this different from normal integers: 
- Since numbers wrap around (n is considered as 0), total ordering no longer makes sense. 
- The number a/b can still be an integer if b is relatively prime to n. 
- The fast exponentiation (a**b in python, a^b in LaTeX, pow(a, b) in C++) is possible without going over-memory. 

Status: It does seem like the answers are correct, but the fast exponentiation is not efficient enough if there are more than 20 digits. The bottleneck lies with multiplication of BigInt. 
