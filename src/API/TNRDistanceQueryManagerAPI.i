%module shortestPaths
%include "std_string.i"
%{
/* Includes the header in the wrapper code */
#include "TNRDistanceQueryManagerAPI.h"

%}

/* Parse the header file to generate wrappers */
%include "TNRDistanceQueryManagerAPI.h"
