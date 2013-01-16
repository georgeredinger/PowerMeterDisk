#include <stdlib.h>
#include <stdio.h>
int main(int argc, char *argv[]){
unsigned lo = 221;
unsigned hi = 255;
int t=(hi<<8|lo);	
if (t> 32767) t-= 65536;
printf("%d\n",);
}

