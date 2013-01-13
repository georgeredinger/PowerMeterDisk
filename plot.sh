#!/bin/bash
awk -f decode.awk temperature.log | head -n-1 >plot.dat
gnuplot <  "plot.gnu"
