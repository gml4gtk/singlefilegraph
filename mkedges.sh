#!/bin/sh 
# create dot edgea from asm output for callgraph
# see 'make callgraph' in makefile.am
perl -ne '/^([^. \t#].*):/ and $f=$1;/call\s+([^*]\S*)/ and print "\"$f\" -> \"$1\"\n";'
