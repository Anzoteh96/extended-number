# Extended Number System
Here's the Extended Number System That I am Working On

## BigInt: Big Integer Implementation in C++

In C++, numbers are stored in either 32- or 64-bit architecture. Therefore, it is sometimes difficult for people who want to utilize integers exceeding this figure. My project tries to address this problem by introducing an extended number system that literally allows a large number of bits for integers (within the limit of being able to be stored by processor, of course). 

Update: after some consideration I decided to use base 2 storage of digits instead of base (2^31). There are two reasons to do so: 
- it simulates more closely on how a compiler performs arithmetic operations. 
- For my next part of project (NumRing), I decided to do efficient exponentiation, which will be beneficial if the bitwise operations can be efficiently and conveniently done here. 

Testing: I generated random integers using Python and tested the arithmetic operations on them (using Python code to get the correct output for comparison). The C++ BigInt gives correct answer. 

Running time: for optimization, O2 is being used. The execution time with the largest file (test99.in) is 0.081 seconds while running through all 100 test files take 3.485 seconds. 

Update 2: That was Karatsuba's algorithm. It's 0.016s for test99.in and 0.845s for all files when I did fast fourier transform (plus some other optimizations).  

## NumRing: ring of number modulo n (presented as the field 'base'). 

There are a few features making this different from normal integers: 
- Since numbers wrap around (n is considered as 0), total ordering no longer makes sense. 
- The number a/b can still be an integer if b is relatively prime to n. 
- The fast exponentiation (a**b in python, a^b in LaTeX, pow(a, b) in C++) is possible without going over-memory. 

Status: It does seem like the answers are correct. The running time is mainly dragged down by exponentiation (which requires repeated iteration of multiplication) with the following results, all compiled using O2 optimization: 
- test10.in (30 digits) : 0.242s
- test20.in (60 digits) : 1.498s
- test30.in (90 digits) : 4.424s
- test40.in (120 digits) : 9.183s
- test50.in (150 digits) : 16.615s
- test60.in (180 digits) : 28.301s
- test70.in (210 digits) : 43.338s
- test80.in (240 digits) : 57.965s
- test90.in (270 digits) : 78.701s
- test99.in (297 digits) : 98.829s

Update: after changing from Karatsuba's multiplication to fast fourier transform here's the new result: 
- test10.in (30 digits) : 0.033s
- test20.in (60 digits) : 0.175s
- test30.in (90 digits) : 0.567s
- test40.in (120 digits) : 1.127s
- test50.in (150 digits) : 2.084s
- test60.in (180 digits) : 3.739s
- test70.in (210 digits) : 5.567s
- test80.in (240 digits) : 8.214s
- test90.in (270 digits) : 11.335s
- test99.in (297 digits) : 14.961s
