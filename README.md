<pre>
Configuration:
* x86-64: i5-3337u with SSD
* Rpi3 with Class10 microSD

First steps:
------------

Compile & generate datafiles and indexfiles:
$ cd search_C && make && cd ..
$ ./generate_dbfile.py           # generate example data
$ ./indexgen_unordered_ordered   # generate index files

Test:
$ ./search_c &lt;number&gt;

SQL test:
SQL: create database speedtest default character set = utf8  default collate utf8_general_ci;
SQL: grant all privileges on speedtest.* to speedtest@localhost identified by 'spdtest';

$ cd MySQL
$ ./import_data.py  # import example data to MySQL with user:speedtest and passwd: spdtest

Times:

x86-64 (i5-3337u):

indexgen (all 4 index): 342 ms

search.php: 3160 ms
search.py: 1905 ms

search_fopen ==>  46.35 ms/db
search_mmap ==>  57.927 ms/db
search_indexuse_unordered ==>   1.1558 ms/db ---> indexfile sima append
search_indexuse_partially ==>   0.11131 ms/db    ---> 64k-nként sort + vége lineár
search_indexuse_partially_multi ==>   0.07025 ms/db --> 512k+256k+128k+64k + 16960 db lineár
search_indexuse_ordered ==>   0.02210 ms/db    ---> full sort

Rpi3@32 bit:

indexgen (all 4 index): 1609 ms

search.php: 369.15 ms
search.py: 248.6 ms

search_fopen ==> 468.3 ms/db
search_mmap ==> 189.336 ms/db
search_indexuse_unordered ==>   5.7189 ms/db
search_indexuse_partially ==>   0.52165 ms/db
search_indexuse_partially_multi ==>   0.27678 ms/db
search_indexuse_ordered ==>   0.08101 ms/db


SQL:
----

x86-64 (i5-3337u):
mysql_insert ==> 54428.3 ms
mysql_select ==>   0.34300 ms # keyint
mysql_select ==> 597.97000 ms # nokeyint

Rpi3:
mysql_insert ==> 209994 ms
mysql_select ==>   0.66700 ms # keyint
mysql_select ==> 3221.41100 ms # nokeyint
