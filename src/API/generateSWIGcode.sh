#!/bin/bash
echo "Generating SWIG code needed for creating the shared library..."
swig -java -c++ XBFSQueryManager.i
echo "SWIG code generated!"
