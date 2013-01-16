set ytics 5
set grid ytics lw .1 
set yrange [-10 : 145]
plot "<(tail -n -86400 plot.dat)" 
#plot 'plot.dat'  with lines 
pause mouse any "Any key or button will terminate"


