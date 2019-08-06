//
// Author: Xenty (Michal Cvach)
// Created on: 01.08.19
//

/*
 * This simple main provides a way to create Contraction Hierarchies for given input files (graphs).
 */


#include <fstream>
#include <iomanip>
#include "GraphBuilding/Loaders/DIMACSLoader.h"
#include "GraphBuilding/Loaders/FloatingPointXenGraphLoader.h"
#include "GraphBuilding/Loaders/IntegerXenGraphLoader.h"
#include "Benchmarking/FloatingPoint/FPointCorectnessValidator.h"
#include "Timer/Timer.h"
#include "CH/Integer/IntegerCHPreprocessor.h"
#include "CH/FloatingPoint/FPointCHPreprocessor.h"
#include "TNR/TNRPreprocessor.h"

using namespace std;

// Prints info about how to use the application to the user.
//______________________________________________________________________________________________________________________
void printUsageInfo(char * appName) {
    printf("This application allows the user to create a Contraction Hierarchy for a given map file.\n"
           "The correct usage is as follows:\n"
           "%s [command] [precision] [input format] [input file path] [output file path]\n"
           "  - Command: currently only 'c' (create) is a valid command.\n"
           "             This means that you want to create a new Contraction Hierarchy.\n"
           "             Other commands might be added in the future.\n"
           "  - Precision: allows you to set whether the hierarchy should work with integers or doubles\n"
           "               internally. 'f' for doubles (floating point), 'i' for integers.\n"
           "  - Input format: currently the application allows two input formats. DIMACS, which is a format\n"
           "                  introduced in the 9th DIMACS Implementation Challenge, and also XenGraph,\n"
           "                  which is a simple text format for graphs. 'D' for DIMACS, 'X' for XenGraph.\n"
           "                  Note that the DIMACS format can only be used with integer precision.\n"
           "  - Input file path: file path to the graph file (in the chosen format) for which you want\n"
           "                     to generate a hierarchy. The path can be in a relative form from the\n"
           "                     current working directory or an absolute path.\n"
           "  - Output file path: determines where the .ch or .chf file containing the Contraction Hierarchy\n"
           "                      will be saved. The .ch or .chf suffix is added automatically so you do not\n"
           "                      to include it in the path. The .ch suffix is for integer hierarchies while\n"
           "                      the .chf suffix is for the floating point hierarchies.\n"
           "\n"
           "A simple example: let's say I have a file 'Prague_map.xeng' which is in a XenGraph format\n"
           "and want to create a integer Contraction Hierarchy for this graph and save it into\n"
           "the current directory with the name 'Prague_map.ch'. I can do this with the following command:\n"
           " '%s c i X Prague_map.xeng Prague_map'\n", appName, appName);
}

// Creates an integer Contraction Hierarchy (.ch) for an input file in a XenGraph format.
//______________________________________________________________________________________________________________________
void createIntegerXenGraphHierarchy(char * inputFilePath, char * outputFilePath) {
    Timer timer("Whole CH construction timer");
    timer.begin();

    IntegerXenGraphLoader graphLoader = IntegerXenGraphLoader(inputFilePath);
    //DIMACSLoader graphLoader = DIMACSLoader("../input/graph.gr");
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates an integer Contraction Hierarchy (.ch) for an input file in a DIMACS format.
//______________________________________________________________________________________________________________________
void createIntegerDIMACSHierarchy(char * inputFilePath, char * outputFilePath) {
    Timer timer("Whole CH construction timer");
    timer.begin();

    DIMACSLoader graphLoader = DIMACSLoader(inputFilePath);
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a floating point Contraction Hierarchy (.chf) for an input file in a XenGraph format.
//______________________________________________________________________________________________________________________
void createFloatingPointXenGraphHierarchy(char * inputFilePath, char * outputFilePath) {
    Timer timer("Whole CH construction timer");
    timer.begin();

    FloatingPointXenGraphLoader graphLoader = FloatingPointXenGraphLoader(inputFilePath);
    FPointUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    FPointCHPreprocessor::preprocessForDDSGF(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgfFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}


//______________________________________________________________________________________________________________________
void createTNR() {
    Timer timer("Whole TNR construction timer");
    timer.begin();

    IntegerXenGraphLoader graphLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    TNRPreprocessor::preprocessUsingCH(*graph, "../input/Prague_map_1000", 1000);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Simple main function parsing the command line input and invoking the relevant functions if needed.
//______________________________________________________________________________________________________________________
int main(int argc, char * argv[]) {
    createTNR();


    /*if (argc != 6) {
        printUsageInfo(argv[0]);
        return 0;
    }

    if (argv[1][0] != 'c') {
        printf("Invalid command. Valid command is only 'c' for create currently.\n"
               "Run the program simply as '%s' to get usage info.\n", argv[0]);
        return 0;
    }

    if (argv[2][0] != 'i' && argv[2][0] != 'f') {
        printf("Invalid precision. Valid are only 'i' and 'f' currently.\n"
               "Run the program simply as '%s' to get usage info.\n", argv[0]);
        return 0;
    }

    if (argv[3][0] != 'X' && argv[3][0] != 'D') {
        printf("Invalid input format. Valid are only 'X' (XenGraph) and 'D' (DIMACS) currently.\n"
               "Run the program simply as '%s' to get usage info.\n", argv[0]);
        return 0;
    }

    if (argv[2][0] == 'i') { // INTEGER
        if (argv[3][0] == 'X') { // int XenGraph
            createIntegerXenGraphHierarchy(argv[4], argv[5]);
        } else { // int DIMACS
            createIntegerDIMACSHierarchy(argv[4], argv[5]);
        }
    } else { // FLOATING POINT
        if (argv[3][0] == 'X') { // double XenGraph
            createFloatingPointXenGraphHierarchy(argv[4], argv[5]);
        } else { // double DIMACS - not implemented.
            printf("Invalid combination 'f D'. Only integer hierarchies can be created for graphs.\n"
                   "in the DIMACS input format. Try 'i D' instead if you have a DIMACS file.\n"
                   "Run the program simply as '%s' to get usage info.\n", argv[0]);
            return 0;
        }
    }*/

    return 0;
}
