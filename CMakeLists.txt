cmake_minimum_required(VERSION 3.14)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(shortest_paths)
include(GenerateExportHeader)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
option(BUILD_DOC "Build documentation" ON)

include_directories(src/)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
endif()

# Packages

## Java
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

## Boost
find_path(BOOST_CONFIG_INCLUDE_DIRS "/boost/config.hpp")
include_directories(${BOOST_CONFIG_INCLUDE_DIRS})

## CSV
find_package(csv2 CONFIG REQUIRED)
link_libraries(csv2::csv2)

## OpenMP
find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    link_libraries(OpenMP::OpenMP_CXX)
endif ()

## Doxygen
find_package(Doxygen
        OPTIONAL_COMPONENTS dot mscgen dia)
if (DOXYGEN_FOUND)
    # set input and output files
    message(${CMAKE_CURRENT_SOURCE_DIR})
    message(${CMAKE_CURRENT_BINARY_DIR})
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    add_custom_target(doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating documentation with Doxygen."
            VERBATIM)
else (DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate documentation.")
endif (DOXYGEN_FOUND)

# Targets

add_library(shortestPaths SHARED
        src/API/CHDistanceQueryManagerAPI.cpp
        src/API/CHDistanceQueryManagerAPI.h
        src/API/TNRDistanceQueryManagerAPI.cpp
        src/API/TNRDistanceQueryManagerAPI.h
        src/API/TNRAFDistanceQueryManagerAPI.cpp
        src/API/TNRAFDistanceQueryManagerAPI.h
        src/API/shortestPathsInterface_wrap.cxx
        src/CH/Structures/CHNode.cpp
        src/CH/Structures/CHNode.h
        src/CH/Structures/CHpriorityQueue.cpp
        src/CH/Structures/CHpriorityQueue.h
        src/CH/EdgeDifferenceManager.cpp
        src/CH/EdgeDifferenceManager.h
        src/CH/Structures/HopsDijkstraNode.cpp
        src/CH/Structures/HopsDijkstraNode.h
        src/CH/Structures/NodeData.cpp
        src/CH/Structures/NodeData.h
        src/CH/CHDistanceQueryManager.cpp
        src/CH/CHDistanceQueryManager.h
        src/CH/CHDistanceQueryManagerWithMapping.cpp
        src/CH/CHDistanceQueryManagerWithMapping.h
        src/CH/CHPathQueryManager.cpp
        src/CH/CHPathQueryManager.h
        src/CH/CHPreprocessor.cpp
        src/CH/CHPreprocessor.h
        src/Dijkstra/BasicDijkstra.cpp
        src/Dijkstra/BasicDijkstra.h
        src/Dijkstra/DijkstraNode.cpp
        src/Dijkstra/DijkstraNode.h
        src/DistanceMatrix/DistanceMatrix.cpp
        src/DistanceMatrix/DistanceMatrix.h
        src/DistanceMatrix/DistanceMatrixComputorSlow.cpp
        src/DistanceMatrix/DistanceMatrixComputorSlow.h
        src/GraphBuilding/Loaders/DDSGLoader.cpp
        src/GraphBuilding/Loaders/DDSGLoader.h
        src/GraphBuilding/Loaders/DIMACSLoader.cpp
        src/GraphBuilding/Loaders/DIMACSLoader.h
        src/GraphBuilding/Loaders/DistanceMatrixLoader.cpp
        src/GraphBuilding/Loaders/DistanceMatrixLoader.h
        src/GraphBuilding/Loaders/XenGraphLoader.cpp
        src/GraphBuilding/Loaders/XenGraphLoader.h
        src/GraphBuilding/Loaders/TripsLoader.cpp
        src/GraphBuilding/Loaders/TripsLoader.h
        src/GraphBuilding/Loaders/TNRGLoader.cpp
        src/GraphBuilding/Loaders/TNRGLoader.h
        src/GraphBuilding/Loaders/TGAFLoader.cpp
        src/GraphBuilding/Loaders/TGAFLoader.h
        src/GraphBuilding/Structures/FlagsGraph.cpp
        src/GraphBuilding/Structures/FlagsGraph.h
        src/GraphBuilding/Structures/FlagsGraphWithUnpackingData.cpp
        src/GraphBuilding/Structures/FlagsGraphWithUnpackingData.h
        src/GraphBuilding/Structures/Graph.cpp
        src/GraphBuilding/Structures/Graph.h
        src/GraphBuilding/Structures/OutputEdge.cpp
        src/GraphBuilding/Structures/OutputEdge.h
        src/GraphBuilding/Structures/OutputShortcutEdge.cpp
        src/GraphBuilding/Structures/OutputShortcutEdge.h
        src/GraphBuilding/Structures/PreprocessingEdgeData.cpp
        src/GraphBuilding/Structures/PreprocessingEdgeData.h
        src/GraphBuilding/Structures/QueryEdge.cpp
        src/GraphBuilding/Structures/QueryEdge.h
        src/GraphBuilding/Structures/QueryEdgeWithUnpackingData.cpp
        src/GraphBuilding/Structures/QueryEdgeWithUnpackingData.h
        src/GraphBuilding/Structures/ShortcutEdge.cpp
        src/GraphBuilding/Structures/ShortcutEdge.h
        src/GraphBuilding/Structures/SimpleGraph.cpp
        src/GraphBuilding/Structures/SimpleGraph.h
        src/GraphBuilding/Structures/UpdateableGraph.cpp
        src/GraphBuilding/Structures/UpdateableGraph.h
        src/GraphBuilding/Structures/SimpleEdge.cpp
        src/GraphBuilding/Structures/SimpleEdge.h
        src/GraphBuilding/Structures/TransitNodeRoutingGraph.cpp
        src/GraphBuilding/Structures/TransitNodeRoutingGraph.h
        src/GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.cpp
        src/GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.h
        src/GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.cpp
        src/GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.h
        src/TNR/TNRPreprocessor.cpp
        src/TNR/TNRPreprocessor.h
        src/TNR/Structures/AccessNodeData.cpp
        src/TNR/Structures/AccessNodeData.h
        src/TNR/TNRDistanceQueryManager.cpp
        src/TNR/TNRDistanceQueryManager.h
        src/TNR/TNRDistanceQueryManagerWithMapping.cpp
        src/TNR/TNRDistanceQueryManagerWithMapping.h
        src/Timer/Timer.cpp
        src/Timer/Timer.h
        src/TNR/TNRPathQueryManager.cpp
        src/TNR/TNRPathQueryManager.h
        src/TNRAF/TNRAFPreprocessor.cpp
        src/TNRAF/TNRAFPreprocessor.h
        src/TNRAF/TNRAFPathQueryManager.cpp
        src/TNRAF/TNRAFPathQueryManager.h
        src/TNRAF/TNRAFDistanceQueryManager.cpp
        src/TNRAF/TNRAFDistanceQueryManager.h
        src/TNRAF/TNRAFDistanceQueryManagerWithMapping.cpp
        src/TNRAF/TNRAFDistanceQueryManagerWithMapping.h
        src/TNRAF/Structures/AccessNodeDataArcFlags.cpp
        src/TNRAF/Structures/AccessNodeDataArcFlags.h
        src/TNRAF/Structures/RegionsStructure.cpp
        src/TNRAF/Structures/RegionsStructure.h
        src/TNRAF/Structures/NodeDataRegions.cpp
        src/TNRAF/Structures/NodeDataRegions.h
        src/TNRAF/Structures/RegionsStructure.cpp
        src/TNRAF/Structures/RegionsStructure.h
        src/GraphBuilding/Loaders/GraphLoader.h
        src/GraphBuilding/Loaders/CsvGraphLoader.cpp
        src/GraphBuilding/Loaders/CsvGraphLoader.h
        src/Error/Error.h
        src/DistanceMatrix/johnson.cpp
        src/DistanceMatrix/johnson.hpp
        src/DistanceMatrix/DistanceMatrixComputorFast.cpp
        src/DistanceMatrix/DistanceMatrixComputorFast.h
        src/DistanceMatrix/DistanceMatrixComputor.h
        src/DistanceMatrix/DistanceMatrixOutputter.h
        src/DistanceMatrix/DistanceMatrixCsvOutputter.cpp
        src/DistanceMatrix/DistanceMatrixCsvOutputter.h
        src/DistanceMatrix/DistanceMatrixXdmOutputter.cpp
        src/DistanceMatrix/DistanceMatrixXdmOutputter.h
        src/CLI/ProgressBar.cpp
        src/CLI/ProgressBar.hpp
        src/constants.h)

GENERATE_EXPORT_HEADER(shortestPaths
        BASE_NAME shortestPaths
        EXPORT_MACRO_NAME shortestPaths_EXPORTS
        EXPORT_FILE_NAME shortestPaths_EXPORTS.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

add_executable(shortestPathsPreprocessor
        src/Benchmarking/TNRBenchmark.cpp
        src/Benchmarking/TNRBenchmark.h
        src/Benchmarking/CHBenchmark.h
        src/Benchmarking/CHBenchmark.cpp
        src/Benchmarking/DijkstraBenchmark.h
        src/Benchmarking/DijkstraBenchmark.cpp
        src/Benchmarking/CorrectnessValidator.h
        src/Benchmarking/CorrectnessValidator.cpp
        src/Benchmarking/DistanceCorrectnessValidator.h
        src/Benchmarking/DistanceCorrectnessValidator.cpp
        src/Benchmarking/DistanceMatrixBenchmark.cpp
        src/Benchmarking/DistanceMatrixBenchmark.h
        src/Benchmarking/PathCorrectnessValidator.cpp
        src/Benchmarking/PathCorrectnessValidator.h
        src/Benchmarking/TNRAFBenchmark.cpp
        src/Benchmarking/TNRAFBenchmark.h
        src/main.cpp)
target_link_libraries(shortestPathsPreprocessor shortestPaths)

add_executable(thesisTests
        src/Benchmarking/TNRBenchmark.cpp
        src/Benchmarking/TNRBenchmark.h
        src/Benchmarking/CHBenchmark.h
        src/Benchmarking/CHBenchmark.cpp
        src/Benchmarking/DijkstraBenchmark.h
        src/Benchmarking/DijkstraBenchmark.cpp
        src/Benchmarking/CorrectnessValidator.h
        src/Benchmarking/CorrectnessValidator.cpp
        src/Benchmarking/DistanceCorrectnessValidator.h
        src/Benchmarking/DistanceCorrectnessValidator.cpp
        src/Benchmarking/DistanceMatrixBenchmark.cpp
        src/Benchmarking/DistanceMatrixBenchmark.h
        src/Benchmarking/PathCorrectnessValidator.cpp
        src/Benchmarking/PathCorrectnessValidator.h
        src/Benchmarking/TNRAFBenchmark.cpp
        src/Benchmarking/TNRAFBenchmark.h
        src/thesisTestScenarios.cpp)
target_link_libraries(thesisTests shortestPaths)


add_executable(tests
        src/Benchmarking/TNRBenchmark.cpp
        src/Benchmarking/TNRBenchmark.h
        src/Benchmarking/CHBenchmark.h
        src/Benchmarking/CHBenchmark.cpp
        src/Benchmarking/DijkstraBenchmark.h
        src/Benchmarking/DijkstraBenchmark.cpp
        src/Benchmarking/CorrectnessValidator.h
        src/Benchmarking/CorrectnessValidator.cpp
        src/Benchmarking/DistanceCorrectnessValidator.h
        src/Benchmarking/DistanceCorrectnessValidator.cpp
        src/Benchmarking/DistanceMatrixBenchmark.cpp
        src/Benchmarking/DistanceMatrixBenchmark.h
        src/Benchmarking/PathCorrectnessValidator.cpp
        src/Benchmarking/PathCorrectnessValidator.h
        src/Benchmarking/TNRAFBenchmark.cpp
        src/Benchmarking/TNRAFBenchmark.h
        src/tests.cpp)
target_link_libraries(tests shortestPaths)
