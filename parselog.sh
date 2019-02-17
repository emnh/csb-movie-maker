#!/bin/bash
file=$1

cat $file |
sed -n 's/^Referee: //p' |
grep -v '^###' |
./parse.py
