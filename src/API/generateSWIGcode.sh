#!/bin/bash
echo "Generating SWIG code needed for creating the shared library..."
swig -java -c++ -package cz.cvut.fel.aic.shortestpaths shortestPathsInterface.i
echo "SWIG code generated!"

# SIDENOTE: the package option in the swig call must match exactly
# the package in which the classes will be put, otherwise it will
# not be possible to call them from Java.
