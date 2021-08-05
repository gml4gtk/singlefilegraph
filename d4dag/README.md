## ㄟ(≧◇≦)ㄏ

## About

This is a single file directed graph layout librry in C and javascript with extra graph features  
  
- Multiple graphs at once  
- Subgraph support in a graph  
- Folding of subgraphs in a graph  
- Layout and re-layout  
- Superbubbles algorithm to automatic find subparts of the graph  
  
<details>

- [About](#about)
- [Install](#install)
- [Usage](#usage)
  - [API](#api)
- [Usage examples](#usage-examples)
- [Goals](#goals)
- [License](#license)

</details>

## Install

make

## Usage

The user program must supply nodes with the needed (x,y) size of the node with the label text.  
The user program must supply edges between the nodes with optional size (x,y) for edge label text.  
After layout the data of the drawing is available using routines and the user program can draw a image or gui window.  

### Api

The api is in d4dag.h

## Usage-examples

Embedded in qvge this creates layout and qvge does the drawing and qt5 gui controls.

## Goals
  
This is not mission-critial software but attempts are made to follow some guides  
https://github.com/abougouffa/awesome-coding-standards  
https://en.wikipedia.org/wiki/The_Power_of_10:_Rules_for_Developing_Safety-Critical_Code  
* Avoid complex flow constructs, such as goto and recursion.  
* All loops must have fixed bounds. This prevents runaway code.  
* Avoid heap memory allocation.  
* Restrict functions to a single printed page.  
* Use a minimum of two runtime assertions per function.  
* Restrict the scope of data to the smallest possible.  
* Check the return value of all non-void functions, or cast to void to indicate the return value is useless.  
* Use the preprocessor sparingly.  
* Limit pointer use to a single dereference, and do not use function pointers.  
* Compile with all possible warnings active; all warnings should then be addressed before release of the software.  
  
Also the Free rosechecker project on sourceforge has a tool for cert standards  
There are more Free tools as Linux kernel sparse source checker and others available to use  
This source is not sabotaged because of wrongdoing politics as some other GPL Free projects do  
This source has no linkage to other libs to make it possible on embedded devices  
  
## Notes
When compiled as C with gcc valgrind reports  
total heap usage: 2 allocs, 2 frees, 1,040 bytes allocated  
When compiled as C++ wiht g++ valgrind reports  
total heap usage: 3 allocs, 3 frees, 73,744 bytes allocated  
The memory usage in the C++ version is more then 70 times higher as proof how bad ugly C++ really is  
  
## License

This is GNU GPL Version 3+ Free Software

SPDX-License-Identifier: GPL-3.0+  
License-Filename: LICENSE  

![gplv3](gplv3.png)

<span>🇳🇱</span>
