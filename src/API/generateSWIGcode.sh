#!/bin/bash
echo "Generating SWIG code needed for creating the shared library..."
swig -java -c++ -package cz.cvut.fel.aic.shortestpaths shortestPathsInterface.i
echo "SWIG code generated!"
