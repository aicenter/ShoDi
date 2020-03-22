 %module shortestPaths
%include "std_string.i"
 %{
 /* Includes the header in the wrapper code */
 #include "CHDistanceQueryManagerAPI.h"

 %}
 
 /* Parse the header file to generate wrappers */
 %include "CHDistanceQueryManagerAPI.h"
