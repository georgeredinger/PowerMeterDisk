#if defined(__unix__)
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "detector.cpp"
int main(int argc, char *argv[]){
std::ifstream infile("samples.dat");
int sample;
int max;
int samp;
 printf("detector testing\n");	
 while (infile >> sample) {
	 samp = detect(sample); 
	 if(samp != max){
	   std::cout << samp << std::endl;
	 }
	 max = samp;
}
}


#endif
