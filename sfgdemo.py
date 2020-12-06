# first run make swigpython to create the _sfg.so file
# todo to add a solution for (void *) in sfg_addnode() with python

import sfg

version=sfg.sfg_version()

print("sfg.c version is " + str(version));

sfg.sfg_init();

sfg.sfg_deinit();
