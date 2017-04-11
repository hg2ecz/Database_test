# Database_test
<pre>
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

search_fopen ==>  46.35689 ms/db
search_mmap ==>  57.92695 ms/db
search_indexuse_unordered ==>   1.15577 ms/db ---> indexfile sima append
search_indexuse_partially ==>   0.11131 ms/db    ---> 64k-nként sort + vége lineár
search_indexuse_partially_multi ==>   0.07025 ms/db --> 512k+256k+128k+64k + 16960 db lineár
search_indexuse_ordered ==>   0.02210 ms/db    ---> full sort

Rpi3@32 bit:

search_fopen ==> 468.33437 ms/db
search_mmap ==> 189.33592 ms/db
search_indexuse_unordered ==>   5.71894 ms/db
search_indexuse_partially ==>   0.52165 ms/db
search_indexuse_partially_multi ==>   0.27678 ms/db
search_indexuse_ordered ==>   0.08101 ms/db


SQL:

Rpi3 & Class10 microSD:
$ time ./import_data.py 
real	9m10,327s
user	2m48,850s
sys	0m23,170s
