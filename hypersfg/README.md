# hypersfg
GNU GPL Free Graph visualization tool  

Hypersfg does visualize graph formats as GML, simple DOT, graphml or  
matrix data using a QT5 GUI and running the GNU GPL Free sfg.c layout  
library with a layered sugiyma directed graph layout like graphviz dot.  
  
To compile:
qmake hypercube.pro  
make  
then run the hypersfg binary  
in directory examples is graph data to try  
also  
make clean  
make distclean  
  
todo: node sizes and node + edge labels to improve  
  
This is modified from GNU GPL Free Hypercube which uses spring-embedder:  

Hypercube features a Qt based GUI tool for visualizing simple text based  
graph representations as SVG and EPS images as well as a Qt independent  
command line tool. Hypercube uses a simulated annealing algorithm to lay  
out the graph that can be easy parametrized to achieve the desired look.  
  
Main goals in development are portability and easy usage rather than high  
performance and complexity. The command line tool does not use any other  
libraries than the standard C++ library and the GUI tool uses "pure" Qt.  
  
![Hypercube](https://a.fsdn.com/con/app/proj/hypercubegraphv/screenshots/screenshot1.png/1)

## Build
### Linux/OS X
GUI application:
```shell
lrelease hypercube.pro
qmake hypercube.pro
make
```
CLI tool:
```shell
qmake hypercube-cli.pro
make
```
### Windows
GUI application:
```shell
lrelease hypercube.pro
qmake hypercube.pro
nmake release
```
CLI tool:
```shell
qmake hypercube-cli.pro
nmake
```

## Binaries
* [Windows & OS X builds](https://sourceforge.net/projects/hypercubegraphv)
* [Linux packages](https://build.opensuse.org/project/repositories/home:tumic:Hypercube)

## Homepage
Hypercube homepage: http://tumic0.github.io/hypercube/
