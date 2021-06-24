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
cp foo.ci foo.bare.ci
$CC -c -fcallgraph-info=su foo.c
cp foo.ci foo.su.ci
$CC -c -fcallgraph-info=su,da foo.c
cp foo.ci foo.da.ci
$CC -c -fcallgraph-info=da,su foo.c
cp foo.ci foo.dasu.ci
