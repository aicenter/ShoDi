 %module contractionHierarchies
%include "std_string.i"
 %{
 /* Includes the header in the wrapper code */
 #include "DistanceQueryManagerAPI.h"
 %}
 
 /* Parse the header file to generate wrappers */
 %include "DistanceQueryManagerAPI.h"
