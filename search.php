#!/usr/bin/php

<?php
function search($value) {
    $f = fopen("dblogfile.log", "r");
    while (($row = fgets($f, 100))) {
	if (explode(';', $row)[1] == $value) print $row;
    }
}

$starttime = microtime(true);
search($argv[1]);
$endtime = microtime(true);
echo 1000.*($endtime-$starttime)." ms\n";
?>
