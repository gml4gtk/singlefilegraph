%module sfg
%{
/* Includes the header in the wrapper code */
#include "sfg.h"
%}

/* Parse the header file to generate wrappers */
%include "sfg.h"
