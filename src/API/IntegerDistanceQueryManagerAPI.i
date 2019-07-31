 %module contractionHierarchies
%include "std_string.i"
 %{
 /* Includes the header in the wrapper code */
 #include "IntegerDistanceQueryManagerAPI.h"

 %}
 
 /* Parse the header file to generate wrappers */
 %include "IntegerDistanceQueryManagerAPI.h"
