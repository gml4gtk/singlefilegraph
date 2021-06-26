#!/bin/sh -x
rm -v -f log1
rm -v -f log2
rm -v -f log2-warning
qmake -r
make clean
scan-build -v -v -maxloop 300 --force-analyze-debug-code make 1>log1 2>log2
grep warning log2 >log2-warning


