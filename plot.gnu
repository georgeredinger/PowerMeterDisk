set grid ytics  lw 1 
plot "<(tail -n -1800 plot.dat)"
#plot 'plot.dat'  with lines 
pause mouse any "Any key or button will terminate"


