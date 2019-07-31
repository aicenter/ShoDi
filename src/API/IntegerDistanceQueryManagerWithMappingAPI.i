 %module contractionHierarchies
%include "std_string.i"
 %{
 /* Includes the header in the wrapper code */
 #include "IntegerDistanceQueryManagerWithMappingAPI.h"

 %}
 
 /* Parse the header file to generate wrappers */
 %include "IntegerDistanceQueryManagerWithMappingAPI.h"
