#!/bin/bash
./plot $@ > plot.csv
return_value=$?
if [ $return_value -eq 0 ] ; then
	gnuplot plot.gp
fi
