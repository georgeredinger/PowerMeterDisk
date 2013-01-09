#if defined(__unix__)
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "detector.cpp"
int main(int argc, char *argv[]){
std::ifstream infile("samples.dat");
int sample;
 printf("detector testing\n");	
 while (infile >> sample) {
	 std::cout << sample << std::endl;
		 }


}


#endif
