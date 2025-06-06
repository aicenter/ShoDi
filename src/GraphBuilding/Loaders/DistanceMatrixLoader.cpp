/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <fstream>
#include "DistanceMatrixLoader.h"
#include "../../Timer/Timer.h"
#include <H5Cpp.h>
#include <boost/numeric/conversion/cast.hpp>

//______________________________________________________________________________________________________________________
DistanceMatrixLoader::DistanceMatrixLoader(std::string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
Distance_matrix_travel_time_provider<dist_t>* DistanceMatrixLoader::loadXDM() {
    std::ifstream input;
    input.open(this->inputFile, std::ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes;
    parseHeader(input, nodes);

    Distance_matrix_travel_time_provider<dist_t>* distanceMatrix = new Distance_matrix_travel_time_provider<dist_t>(nodes);

    parseDistances(input, nodes, *distanceMatrix);

    input.close();

    return distanceMatrix;
}

//______________________________________________________________________________________________________________________
DistanceMatrixInterface* DistanceMatrixLoader::loadHDF() {
    H5::H5File file{this->inputFile, H5F_ACC_RDONLY};
    H5::DataSet dataset = file.openDataSet("dm");
    H5::IntType intType = dataset.getIntType();
    auto size = intType.getSize();
    H5::DataSpace space = dataset.getSpace();

    hsize_t dimsf[2];
    space.getSimpleExtentDims(dimsf, nullptr);
    const auto nodes = dimsf[0];

    if (size <= 2) {
        auto dm = new Distance_matrix_travel_time_provider<uint_least16_t>(boost::numeric_cast<unsigned int>(nodes));
        dataset.read(dm->getRawData().get(), H5::PredType::NATIVE_UINT_LEAST16);
        return dm;
    } else if (size <= 4) {
        auto dm = new Distance_matrix_travel_time_provider<uint_least32_t>(boost::numeric_cast<unsigned int>(nodes));
        dataset.read(dm->getRawData().get(), H5::PredType::NATIVE_UINT_LEAST32);
        return dm;
    } else {
        auto dm = new Distance_matrix_travel_time_provider<dist_t>(boost::numeric_cast<unsigned int>(nodes));
        dataset.read(dm->getRawData().get(), H5::PredType::NATIVE_UINT);
        return dm;
    }
}

//______________________________________________________________________________________________________________________
void DistanceMatrixLoader::parseHeader(std::ifstream & input, unsigned int & nodes) {
    char c1, c2, c3;
    input.read(&c1, sizeof(c1));
    input.read(&c2, sizeof(c2));
    input.read(&c3, sizeof(c3));
    if (c1 != 'X' || c2 != 'D' || c3 != 'M') {
        printf("The input file is missing the Distance Matrix header.\n");
        printf("Are you sure the input file is in the correct format?\n");
        printf("The loading will proceed but the loaded distance matrix might be corrupted.\n");
    }

    input.read((char *) &nodes, sizeof(nodes));
}

//______________________________________________________________________________________________________________________
void DistanceMatrixLoader::parseDistances(std::ifstream & input, const unsigned int nodes, Distance_matrix_travel_time_provider<dist_t>& distanceMatrix) {
    unsigned int distance;
    for(unsigned int i = 0; i < nodes; i++) {
        for(unsigned int j = 0; j < nodes; j++) {
            input.read((char *) &distance, sizeof(distance));
            distanceMatrix.setDistance(i, j, distance);
        }
    }
}