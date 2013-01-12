#!/bin/bash
awk -f convert.awk temperature.log | head -n-1 >plot.dat
gnuplot <  "plot.gnu"
