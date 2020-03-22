#!/bin/bash
echo "Generating SWIG code needed for creating the shared library..."
swig -java -c++ -package cz.cvut.fel.aic.contractionhierarchies shortestPathsInterface.i
echo "SWIG code generated!"
