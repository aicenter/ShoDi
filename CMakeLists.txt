cmake_minimum_required(VERSION 3.14)
#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
#  CACHE STRING "Vcpkg toolchain file")

project(shortest_distances VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options("/W4")
else ()
    add_compile_options("-Wall" "-Wextra")
    # "-Werror"
endif ()

# cmake modules
include(FetchContent) # for dependencies that should be downloaded and built from source
include(GenerateExportHeader) # for the GENERATE_EXPORT_HEADER macro
include(GoogleTest) # for the gtest_discover_tests macro

#set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

option(BUILD_DOC "Build documentation" ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wconversion -pedantic")
endif ()


# DEPENDENCIES
find_package(indicators CONFIG REQUIRED)

## Java
find_package(JNI)

## Boost
find_package(Boost REQUIRED COMPONENTS program_options REQUIRED)

# Check boost header-only components
if (NOT EXISTS ${Boost_INCLUDE_DIRS}/boost/graph)
    message(FATAL_ERROR "Boost graph component not found. Search path: ${Boost_INCLUDE_DIRS}/boost/graph")
endif ()

## CSV
find_path(P_RANAV_CSV2_INCLUDE_DIRS "csv2/mio.hpp")

## HDF5
find_package(HDF5 REQUIRED COMPONENTS C CXX HL)

## PROJ
find_package(PROJ CONFIG REQUIRED)

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

# Google Test
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/d66ce585109eed6d2d891b7ed7ab3ca96e854483.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# TARGETS

add_library(shortestPaths SHARED
        src/constants.h
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
        src/CH/CHDistanceQueryManager.tpp
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
        src/Astar/Astar.cpp
        src/Astar/Astar.h
        src/Astar/AstarNode.cpp
        src/Astar/AstarNode.h
        src/DistanceMatrix/Distance_matrix_travel_time_provider.cpp
        src/DistanceMatrix/Distance_matrix_travel_time_provider.h
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
        src/GraphBuilding/Loaders/TNRGLoader.tpp
        src/GraphBuilding/Loaders/TNRGLoader.h
        src/GraphBuilding/Loaders/TGAFLoader.cpp
        src/GraphBuilding/Loaders/TGAFLoader.h
        src/GraphBuilding/Structures/FlagsGraph.h
        src/GraphBuilding/Structures/FlagsGraphWithUnpackingData.cpp
        src/GraphBuilding/Structures/FlagsGraphWithUnpackingData.h
        src/GraphBuilding/Structures/BaseGraph.h
        src/GraphBuilding/Structures/Graph.cpp
        src/GraphBuilding/Structures/Graph.h
        src/GraphBuilding/Structures/AdjMatrixGraph.cpp
        src/GraphBuilding/Structures/AdjMatrixGraph.h
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
        src/GraphBuilding/Loaders/AdjGraphLoader.cpp
        src/GraphBuilding/Loaders/AdjGraphLoader.h
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
        src/DistanceMatrix/DistanceMatrixHdfOutputter.cpp
        src/DistanceMatrix/DistanceMatrixHdfOutputter.h
        src/CLI/ProgressBar.cpp
        src/CLI/ProgressBar.hpp
        src/DistanceMatrix/Distance_matrix_reader.cpp src/DistanceMatrix/Distance_matrix_reader.h)
target_link_libraries(shortestPaths PRIVATE ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})

#GENERATE_EXPORT_HEADER(shortestPaths
#        BASE_NAME shortestPaths
#        EXPORT_MACRO_NAME shortestPaths_EXPORTS
#        EXPORT_FILE_NAME shortestPaths_EXPORTS.h
#        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

add_executable(shortestPathsPreprocessor
    src/main.cpp
    src/Benchmarking/CHBenchmark.cpp
    src/Benchmarking/CorrectnessValidator.cpp
    src/Benchmarking/DijkstraBenchmark.cpp
    src/Benchmarking/AstarBenchmark.cpp
    src/Benchmarking/LocationTransformer.cpp
    src/Benchmarking/TNRAFBenchmark.cpp
    src/Benchmarking/TNRBenchmark.cpp
    src/Benchmarking/DistanceMatrixBenchmark.cpp
    src/Benchmarking/memory.cpp
    src/CH/CHDistanceQueryManager.tpp
    src/CH/CHDistanceQueryManagerWithMapping.cpp
    src/CH/CHPreprocessor.cpp
    src/CH/EdgeDifferenceManager.cpp
    src/CH/Structures/CHNode.cpp
    src/CH/Structures/CHpriorityQueue.cpp
    src/CH/Structures/HopsDijkstraNode.cpp
    src/CH/Structures/NodeData.cpp
    src/CLI/ProgressBar.cpp
    src/Dijkstra/BasicDijkstra.cpp
    src/Dijkstra/DijkstraNode.cpp
    src/Astar/Astar.cpp
    src/Astar/AstarNode.cpp
    src/DistanceMatrix/DistanceMatrixComputorFast.cpp
    src/DistanceMatrix/DistanceMatrixComputorSlow.cpp
    src/DistanceMatrix/DistanceMatrixCsvOutputter.cpp
    src/DistanceMatrix/DistanceMatrixHdfOutputter.cpp
    src/DistanceMatrix/DistanceMatrixXdmOutputter.cpp
    src/DistanceMatrix/Distance_matrix_travel_time_provider.cpp
    src/DistanceMatrix/johnson.cpp
    src/GraphBuilding/Loaders/AdjGraphLoader.cpp
    src/GraphBuilding/Loaders/CsvGraphLoader.cpp
    src/GraphBuilding/Loaders/DDSGLoader.cpp
    src/GraphBuilding/Loaders/DIMACSLoader.cpp
    src/GraphBuilding/Loaders/DistanceMatrixLoader.cpp
    src/GraphBuilding/Loaders/TGAFLoader.cpp
    src/GraphBuilding/Loaders/TNRGLoader.tpp
    src/GraphBuilding/Loaders/TripsLoader.cpp
    src/GraphBuilding/Loaders/XenGraphLoader.cpp
    src/GraphBuilding/Structures/AdjMatrixGraph.cpp
    src/GraphBuilding/Structures/FlagsGraph.h
    src/GraphBuilding/Structures/FlagsGraphWithUnpackingData.cpp
    src/GraphBuilding/Structures/Graph.cpp
    src/GraphBuilding/Structures/OutputEdge.cpp
    src/GraphBuilding/Structures/OutputShortcutEdge.cpp
    src/GraphBuilding/Structures/PreprocessingEdgeData.cpp
    src/GraphBuilding/Structures/QueryEdge.cpp
    src/GraphBuilding/Structures/QueryEdgeWithUnpackingData.cpp
    src/GraphBuilding/Structures/ShortcutEdge.cpp
    src/GraphBuilding/Structures/SimpleGraph.cpp
    src/GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.cpp
    src/GraphBuilding/Structures/TransitNodeRoutingGraph.h
    src/GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.cpp
    src/GraphBuilding/Structures/UpdateableGraph.cpp
    src/TNR/TNRDistanceQueryManager.cpp
    src/TNR/TNRDistanceQueryManagerWithMapping.cpp
    src/TNR/TNRPreprocessor.cpp
    src/TNR/Structures/AccessNodeData.cpp
    src/TNRAF/TNRAFDistanceQueryManager.cpp
    src/TNRAF/TNRAFDistanceQueryManagerWithMapping.cpp
    src/TNRAF/TNRAFPreprocessor.cpp
    src/TNRAF/Structures/AccessNodeDataArcFlags.cpp
    src/TNRAF/Structures/NodeDataRegions.cpp
    src/TNRAF/Structures/RegionsStructure.cpp
    src/Timer/Timer.cpp
)

add_executable(thesisBenchmark
        src/Benchmarking/TNRBenchmark.cpp
        src/Benchmarking/CHBenchmark.cpp
        src/Benchmarking/DijkstraBenchmark.cpp
        src/Benchmarking/AstarBenchmark.cpp
        src/Benchmarking/CorrectnessValidator.cpp
        src/Benchmarking/DistanceCorrectnessValidator.cpp
        src/Benchmarking/DistanceMatrixBenchmark.cpp
        src/Benchmarking/LocationTransformer.cpp
        src/Benchmarking/PathCorrectnessValidator.cpp
        src/Benchmarking/TNRAFBenchmark.cpp
        src/CH/Structures/CHNode.cpp
        src/CH/Structures/CHpriorityQueue.cpp
        src/CH/EdgeDifferenceManager.cpp
        src/CH/Structures/HopsDijkstraNode.cpp
        src/CH/Structures/NodeData.cpp
        src/CH/CHDistanceQueryManager.tpp
        src/CH/CHDistanceQueryManagerWithMapping.cpp
        src/CH/CHPathQueryManager.cpp
        src/CH/CHPreprocessor.cpp
        src/Dijkstra/BasicDijkstra.cpp
        src/Dijkstra/DijkstraNode.cpp
        src/Astar/Astar.cpp
        src/Astar/AstarNode.cpp
        src/DistanceMatrix/Distance_matrix_travel_time_provider.cpp
        src/DistanceMatrix/DistanceMatrixComputorSlow.cpp
        src/GraphBuilding/Loaders/DDSGLoader.cpp
        src/GraphBuilding/Loaders/DIMACSLoader.cpp
        src/GraphBuilding/Loaders/DistanceMatrixLoader.cpp
        src/GraphBuilding/Loaders/XenGraphLoader.cpp
        src/GraphBuilding/Loaders/TripsLoader.cpp
        src/GraphBuilding/Loaders/TNRGLoader.tpp
        src/GraphBuilding/Loaders/TGAFLoader.cpp
        src/GraphBuilding/Structures/FlagsGraph.h
        src/GraphBuilding/Structures/FlagsGraphWithUnpackingData.cpp
        src/GraphBuilding/Structures/Graph.cpp
        src/GraphBuilding/Structures/AdjMatrixGraph.cpp
        src/GraphBuilding/Structures/OutputEdge.cpp
        src/GraphBuilding/Structures/OutputShortcutEdge.cpp
        src/GraphBuilding/Structures/PreprocessingEdgeData.cpp
        src/GraphBuilding/Structures/QueryEdge.cpp
        src/GraphBuilding/Structures/QueryEdgeWithUnpackingData.cpp
        src/GraphBuilding/Structures/ShortcutEdge.cpp
        src/GraphBuilding/Structures/SimpleGraph.cpp
        src/GraphBuilding/Structures/UpdateableGraph.cpp
        src/GraphBuilding/Structures/SimpleEdge.cpp
        src/GraphBuilding/Structures/TransitNodeRoutingGraph.h
        src/GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.cpp
        src/GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.cpp
        src/TNR/TNRPreprocessor.cpp
        src/TNR/Structures/AccessNodeData.cpp
        src/TNR/TNRDistanceQueryManager.cpp
        src/TNR/TNRDistanceQueryManagerWithMapping.cpp
        src/Timer/Timer.cpp
        src/TNR/TNRPathQueryManager.cpp
        src/TNRAF/TNRAFPreprocessor.cpp
        src/TNRAF/TNRAFPathQueryManager.cpp
        src/TNRAF/TNRAFDistanceQueryManager.cpp
        src/TNRAF/TNRAFDistanceQueryManagerWithMapping.cpp
        src/TNRAF/Structures/AccessNodeDataArcFlags.cpp
        src/TNRAF/Structures/RegionsStructure.cpp
        src/TNRAF/Structures/NodeDataRegions.cpp
        src/TNRAF/Structures/RegionsStructure.cpp
        src/GraphBuilding/Loaders/CsvGraphLoader.cpp
        src/DistanceMatrix/DistanceMatrixXdmOutputter.cpp
        src/CLI/ProgressBar.cpp
        src/thesisTestScenarios.cpp)

add_executable(benchmark
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

add_executable(test_runner
        src/CH/CHDistanceQueryManager.tpp
        src/CH/CHPreprocessor.cpp
        src/CH/EdgeDifferenceManager.cpp
        src/CH/Structures/CHNode.cpp
        src/CH/Structures/CHpriorityQueue.cpp
        src/CH/Structures/HopsDijkstraNode.cpp
        src/CH/Structures/NodeData.cpp
        src/CLI/ProgressBar.cpp
        src/Dijkstra/BasicDijkstra.cpp
        src/Dijkstra/DijkstraNode.cpp
        src/DistanceMatrix/CSV_reader.h
        src/DistanceMatrix/CSV_reader.cpp
        src/DistanceMatrix/DistanceMatrixComputorFast.cpp
        src/DistanceMatrix/DistanceMatrixComputorSlow.cpp
        src/DistanceMatrix/Distance_matrix_travel_time_provider.cpp
        src/DistanceMatrix/johnson.cpp
        src/GraphBuilding/Loaders/AdjGraphLoader.cpp
        src/GraphBuilding/Loaders/DDSGLoader.cpp
        src/GraphBuilding/Loaders/TGAFLoader.cpp
        src/GraphBuilding/Loaders/TNRGLoader.tpp
        src/GraphBuilding/Structures/AdjMatrixGraph.cpp
        src/GraphBuilding/Structures/FlagsGraph.h
        src/GraphBuilding/Structures/FlagsGraphWithUnpackingData.cpp
        src/GraphBuilding/Structures/Graph.cpp
        src/GraphBuilding/Structures/OutputEdge.cpp
        src/GraphBuilding/Structures/OutputShortcutEdge.cpp
        src/GraphBuilding/Structures/PreprocessingEdgeData.cpp
        src/GraphBuilding/Structures/QueryEdge.cpp
        src/GraphBuilding/Structures/QueryEdgeWithUnpackingData.cpp
        src/GraphBuilding/Structures/ShortcutEdge.cpp
        src/GraphBuilding/Structures/SimpleGraph.cpp
        src/GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.cpp
        src/GraphBuilding/Structures/TransitNodeRoutingGraph.h
        src/GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.cpp
        src/GraphBuilding/Structures/UpdateableGraph.cpp
        src/progress_bar.h
        src/Timer/Timer.cpp
        src/TNR/Structures/AccessNodeData.cpp
        src/TNR/TNRPreprocessor.cpp
        src/TNRAF/Structures/AccessNodeDataArcFlags.cpp
        src/TNRAF/Structures/NodeDataRegions.cpp
        src/TNRAF/Structures/RegionsStructure.cpp
        src/TNRAF/TNRAFPreprocessor.cpp

        test/ch_tests.cpp
        test/common.cpp
        test/dm_test.cpp
        test/tnr_tests.cpp
        test/tnraf_tests.cpp)

add_executable(func_test_runner
    functest/ch_test.cpp
    functest/common.cpp
    functest/dm_test.cpp
    functest/dm_test_hdf.cpp
    functest/expected_graphs.cpp
    functest/tnr_test.cpp
    functest/tnraf_test.cpp
    src/CH/CHDistanceQueryManager.tpp
    src/CH/Structures/NodeData.cpp
    src/Dijkstra/BasicDijkstra.cpp
    src/Dijkstra/DijkstraNode.cpp
    src/DistanceMatrix/DistanceMatrixComputorSlow.cpp
    src/DistanceMatrix/DistanceMatrixHdfOutputter.cpp
    src/DistanceMatrix/Distance_matrix_travel_time_provider.cpp
    src/GraphBuilding/Loaders/DDSGLoader.cpp
    src/GraphBuilding/Loaders/TGAFLoader.cpp
    src/GraphBuilding/Loaders/TNRGLoader.tpp
    src/GraphBuilding/Loaders/XenGraphLoader.cpp
    src/GraphBuilding/Structures/FlagsGraph.h
    src/GraphBuilding/Structures/FlagsGraphWithUnpackingData.cpp
    src/GraphBuilding/Structures/Graph.cpp
    src/GraphBuilding/Structures/OutputEdge.cpp
    src/GraphBuilding/Structures/OutputShortcutEdge.cpp
    src/GraphBuilding/Structures/PreprocessingEdgeData.cpp
    src/GraphBuilding/Structures/QueryEdge.cpp
    src/GraphBuilding/Structures/QueryEdgeWithUnpackingData.cpp
    src/GraphBuilding/Structures/SimpleGraph.cpp
    src/GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.cpp
    src/GraphBuilding/Structures/TransitNodeRoutingGraph.h
    src/GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.cpp
    src/GraphBuilding/Structures/UpdateableGraph.cpp
    src/TNR/TNRPreprocessor.cpp
    src/TNR/Structures/AccessNodeData.cpp
    src/TNRAF/TNRAFPreprocessor.cpp
    src/TNRAF/Structures/AccessNodeDataArcFlags.cpp
    src/TNRAF/Structures/NodeDataRegions.cpp
    src/TNRAF/Structures/RegionsStructure.cpp
    src/Timer/Timer.cpp
)
# functional tests depends on the main binary
add_dependencies(func_test_runner shortestPathsPreprocessor)


# HEADER INCLUDE

# to enable includes relative to project root
target_include_directories(shortestPathsPreprocessor PRIVATE src)
target_include_directories(func_test_runner PRIVATE src)

target_include_directories(shortestPathsPreprocessor PRIVATE ${P_RANAV_CSV2_INCLUDE_DIRS})
target_include_directories(shortestPathsPreprocessor PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(shortestPaths PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(func_test_runner PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(test_runner PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(thesisBenchmark PRIVATE ${P_RANAV_CSV2_INCLUDE_DIRS})

# include JNI dirs if found
if (JNI_FOUND)
    target_include_directories(shortestPaths PRIVATE ${JNI_INCLUDE_DIRS})
endif ()


# LINKING

# old linking against lib: should be removed
target_link_libraries(benchmark shortestPaths)

target_link_libraries(test_runner PRIVATE GTest::gtest_main)
target_link_libraries(func_test_runner PRIVATE GTest::gtest_main)
target_link_libraries(shortestPathsPreprocessor PRIVATE ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(shortestPathsPreprocessor PRIVATE ${Boost_LIBRARIES})
target_link_libraries(func_test_runner PRIVATE ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})
target_link_libraries(thesisBenchmark PRIVATE PROJ::proj)
target_link_libraries(shortestPathsPreprocessor PRIVATE PROJ::proj)


# RESOURCES
add_custom_command(
        TARGET test_runner POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/test $<TARGET_FILE_DIR:func_test_runner>/test)
add_custom_command(
        TARGET func_test_runner POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/functest $<TARGET_FILE_DIR:func_test_runner>/functest)


# TESTING
enable_testing()
gtest_discover_tests(test_runner)
