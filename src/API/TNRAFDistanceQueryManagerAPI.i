%module shortestPaths
%include "std_string.i"
%{
/* Includes the header in the wrapper code */
#include "TNRAFDistanceQueryManagerAPI.h"

%}

/* Parse the header file to generate wrappers */
%include "TNRAFDistanceQueryManagerAPI.h"
