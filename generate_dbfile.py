#!/usr/bin/pypy

import os
import sys
import time
import random

def gen(rownum):
    f = open("dblogfile.log", "w", 0644)
    for cnt in xrange(rownum):
	num = random.randint(0, 100*1000*1000)
	row = "%d;%d;string %d = %d == %d\n"%(cnt,num,num,cnt,num)
	f.write(row)
	if cnt % int(rownum/10) == 0:
	    print num, "-->", row,
    f.close()

if __name__ == '__main__':
    if len(sys.argv) != 2:
	print "Parameter: number of data row. For example: 1000000"
	sys.exit(-1)
    rownum = int(sys.argv[1])

    starttime = time.time()
    gen(rownum)
    endtime = time.time()
    print "%.3f ms"%(1000.*(endtime-starttime))

    os.system('./indexgen_unordered_ordered')
