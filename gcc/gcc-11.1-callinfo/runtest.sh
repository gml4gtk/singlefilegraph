#!/bin/sh -x
CC=$HOME/mygcc/bin/gcc
CFLAGS=" -fcallgraph-info"
rm -v -f O1
rm -v -f O2
rm -v -f *.ci
rm -v -f *.o
rm -v -f *.svg
rm -v -f *.gv
$CC $CFLAGS -c -I. foo.c 1>O1 2>O2
