/OK/{ 
	lo=$5;
	hi=$6;
	t=hi*256.0+lo;
  if(t > 32767.0){
	 	t-= 65536.0;
  }	
  printf("%f\n",(t/10.0)*1.8+32);
}

