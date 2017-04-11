# Database_test
<pre>
Compile & generate datafiles and indexfiles:
$ cd search_C && make && cd ..
$ ./generate_dbfile.py           # generate example data
$ ./indexgen_unordered_ordered   # generate index files

Test:
$ ./search_c <number>

SQL test:
SQL: create database speedtest default character set = utf8  default collate utf8_general_ci;
SQL: grant all privileges on speedtest.* to speedtest@localhost identified by 'spdtest';

$ cd MySQL
$ ./import_data.py  # import example data to MySQL with user:speedtest and passwd: spdtest
