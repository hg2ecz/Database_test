#!/usr/bin/pypy

import sys
import time

def search(value):
    for row in open("dblogfile.log", "r"):
	if row.split(';')[1] == value:
	    print row,

starttime = time.time()
search(sys.argv[1])
endtime = time.time()

print "%.3f ms"%(1000.*(endtime-starttime))
