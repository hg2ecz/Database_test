#!/usr/bin/python

import time
import MySQLdb

def insert():
    conn = MySQLdb.connect(host="localhost", user="speedtest", passwd="spdtest", db="speedtest")
    cur = conn.cursor()
    cur.execute('drop table if exists test')
    cur.execute('create table test (id int not null auto_increment primary key, keyint int not null, nokeyint int not null, str varchar(50))')
    cur.execute('alter table test add key(keyint)')

    conn.autocommit(False)
    for row in open("../dblogfile.log", "r"):
	f = row.split(';')
	cur.execute("INSERT test (keyint, nokeyint, str) values (%d, %d, \"%s\")"%(int(f[1]), int(f[1]), f[2]))

    conn.commit()
    cur.close()
    conn.close()

starttime = time.time()
insert()
endtime = time.time()

print "%.3f ms"%(1000.*(endtime-starttime))
