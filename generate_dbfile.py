#!/usr/bin/pypy

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

starttime = time.time()
gen(1000*1000)
endtime = time.time()

print "%.3f ms"%(1000.*(endtime-starttime))
