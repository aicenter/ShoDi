#!/bin/bash
echo "Creating object files..."
g++ -fPIC -std=c++11 -c *.cpp *.cxx ../*/*.cpp ../*/*/*.cpp .. -I/home/xenty/jdk/jdk1.8.0_171/include -I/home/xenty/jdk/jdk1.8.0_171/include/linux 
echo "Creating shared library..."
g++ -std=c++11 -shared *.o -o libshortestPaths.so
echo "Cleaning object files..."
rm *.o
echo "Finished creating shared library!"
