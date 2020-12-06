#
# make file for sfgdemo and sfg graph library
#

sfgdemo: sfg.c sfgdemo.c
	$(CC) -g -Wall -Wextra sfgdemo.c sfg.c -o sfgdemo

sfga:
	rm -v -f *.o *.a
	gcc -g -c sfg.c
	ar -cvq libsfg.a sfg.o

sfgso:
	rm -v -f *.o *.so.1.0 *.so.1 *.so
	gcc -g -c -fPIC sfg.c
	gcc -g -shared -Wl,-soname,libsfg.so.1 -o libsfg.so.1.0 sfg.o
	ln -sf libsfg.so.1.0 libsfg.so.1
	ln -sf libsfg.so.1.0 libsfg.so

# using emscripten for the javascript version
emcc:
	emcc -O0 -s WASM=0 sfg.c

# swig python wrapper
# specify the include directory with <Python.h> here
swigpython:
	rm -v -f ./sfg.py
	rm -v -f ./sfg.pyc
	rm -v -f ./*.o
	rm -v -f ./sfg_wrap.c
	swig -python sfg.i
	gcc -fPIC -c sfg.c sfg_wrap.c -I/usr/include/python2.7
	ld -shared sfg.o sfg_wrap.o -o _sfg.so

# swig perl wrapper
# perl -V:ccflags
# specify the location of directory CORE
swigperl:
	rm -v -f ./sfg.pm
	rm -v -f ./sfg_wrap.c
	swig -perl5 sfg.i
	gcc -c -I/usr/lib/x86_64-linux-gnu/perl/5.28.1/CORE sfg.c sfg_wrap.c
	ld -G sfg.o sfg_wrap.o -o sfg.so

# swig tcl wrapper
# specify the location of <tcl.h>
swigtcl:
	rm -v -f ./sfg_wrap.c
	rm -v -f ./*.o
	rm -v -f ./*.so
	swig -tcl sfg.i
	gcc -c -fpic sfg.c sfg_wrap.c -I/usr/include/tcl8.6
	gcc -shared sfg.o sfg_wrap.o -o sfg.so

# swig c++ wrapper
swigcplus:
	rm -v -f ./sfg_wrap.cxx
	swig -c++ -tcl sfg.i
	g++ -c -fpic sfg_wrap.cxx -I/usr/include/tcl8.6

# swig go
swiggo:
	rm -v -f ./sfg_wrap.c
	rm -v -f ./sfg_gc.c
	rm -v -f ./sfg.go
	swig -go -intgosize 64 sfg.i

# swig java
# see the swig docs
swigjava:
	rm -v -f ./*.java
	rm -v -f ./sfg_wrap.c
	swig -java sfg.i

clean:
	rm -v -f ./*.java
	rm -v -f ./sfg_gc.c
	rm -v -f ./sfg.go
	rm -v -f ./sfg.pm
	rm -v -f ./sfg.py
	rm -v -f ./sfg.pyc
	rm -v -f ./sfg_wrap.c
	rm -v -f ./sfg_wrap.cxx
	rm -v -f ./a.out*
	rm -v -f ./O
	rm -v -f ./O1
	rm -v -f ./O2
	rm -v -f ./OO
	rm -v -f ./sfgdemo
	rm -v -f ./*~
	rm -v -f ./dsmake.errors
	rm -v -f ./dsmake.warnings
	rm -v -f ./dsmake.output
	rm -v -f ./*.o
	rm -v -f ./*.a
	rm -v -f ./*.so.1
	rm -v -f ./*.so
	rm -v -f ./*.so.1.0

indent:
	./Lindent sfg.h sfg.c sfgdemo.c

valgrind:
	valgrind --leak-check=full --show-reachable=yes ./sfgdemo 1>O1 2>O2

# end
