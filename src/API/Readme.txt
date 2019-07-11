Bash scripts in this directory allow you to generate the shared
library which is required for the Contraction Hierarchies to
work from Amodsim.

How to proceed
==============
1) Generate the SWIG code by using the "generateSWIGcode.sh" script,
   this creates the glue code required further.
3) In the "generateSharedLib.sh" script, on line 3, replace the paths
   at the end of the command with paths to the JDK in your system.
2) Run the "generateSharedLib.sh" which creates the actual library,
   in Linux the output will be the "libcontractionHierarchies.so" file.

Requirements
============
The mentioned scripts work in Linux. In other operating systems,
you can use them as some sort of a reference and it should be 
possible to accomplish the same results with appropriate commands
in other operating systems as well.

In Linux, SWIG and g++ is required to run the scripts. Also JDK
has to be installed on the system. For info on how to download
and install SWIG, you can consult http://www.swig.org/ .

Sidenote
========
In "generateSWIGcode.sh", the package option must match exactly
the package in which the classes will be put, otherwise it will
not be possible to call them from Java.
