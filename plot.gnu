dy=2
set mytics 5

set yrange [-10 : 100]
set grid ytics  lw .1 
plot "<(tail -n -17200 plot.dat)" 
#plot 'plot.dat'  with lines 
pause mouse any "Any key or button will terminate"


