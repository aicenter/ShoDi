/******************************************************************************
 * File:             CsvGraphLoader.cpp
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          07/27/20
 *****************************************************************************/

#include "CsvGraphLoader.h"
#include "CLI/ProgressBar.hpp"
#include "GraphBuilding/Structures/Graph.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
#include <boost/numeric/conversion/cast.hpp>
#include <limits>
#include <stdexcept>
#include <iostream>



CsvGraphLoader::CsvGraphLoader(std::string inputFile) : inputFile(inputFile) {
    if (!reader.mmap(inputFile)) {
        throw std::runtime_error(std::string("Error reading file ") + this->inputFile + " using mmap.\n");
    }
}

inline dist_t parse_distance(std::string str, unsigned int nodeFrom, unsigned int nodeTo, std::string inputFile,
                             double precisionLoss) {
    double val;
    try {
        val = stod(str);
    }
    catch (std::invalid_argument &) {
        std::cerr << "Warning: Found an unexpected value (" << str << ") in '" << inputFile
             << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
        return std::numeric_limits<dist_t>::max();
    }
    catch (std::out_of_range &) {
        std::cerr << "Warning: Found an out of range value (" << str << ") in '" << inputFile
             << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
        return std::numeric_limits<dist_t>::max();
    }

    if (isnan(val)) {
        return std::numeric_limits<dist_t>::max();
    }

    if (val < 0) {
        std::cerr << "Warning: Found a negative value (" << str << ") in '" << inputFile
             << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
        return std::numeric_limits<dist_t>::max();
    }

    return (dist_t) round(val / (double) precisionLoss);
}

unsigned int CsvGraphLoader::nodes() {
    return boost::numeric_cast<unsigned int>(reader.cols());
}

void CsvGraphLoader::loadGraph(BaseGraph &graph, unsigned int precisionLoss) {
    const unsigned int size = nodes();

    if (size != reader.rows())
        throw std::runtime_error(this->inputFile +
                            " does not contain a square matrix. Found " +
                            std::to_string(reader.rows()) + " rows and " +
                            std::to_string(size) + " cols.\n");

    const dist_t max = std::numeric_limits<dist_t>::max();
    ProgressBar progress(size, "Loading CSV file:");

    unsigned int i = 0;
    for (const auto &row: reader) {
        unsigned int j = 0;
        for (const auto &cell: row) {
            std::string val;
            cell.read_value(val);
            const dist_t dist = parse_distance(val, i, j, inputFile, (double) precisionLoss);
            if (dist != max)
                graph.addEdge(i, j, dist);
            ++j;
        }
        ++progress;
        ++i;
    }
}
