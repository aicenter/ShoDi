 %module contractionHierarchies
%include "std_string.i"
 %{
 /* Includes the header in the wrapper code */
 #include "FPointDistanceQueryManagerAPI.h"

 %}
 
 /* Parse the header file to generate wrappers */
 %include "FPointDistanceQueryManagerAPI.h"
