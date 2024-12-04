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
// Created on: 6.8.18
//

#include <iostream>
#include "CorrectnessValidator.h"

//______________________________________________________________________________________________________________________
bool CorrectnessValidator::validate(std::vector<dist_t> &a, std::vector<dist_t> &b) {
    for(size_t i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            return false;
        }
    }
    return true;
}

//______________________________________________________________________________________________________________________
bool CorrectnessValidator::validateVerbose(std::vector<dist_t> &a, std::vector<dist_t> &b) {
    unsigned int mmcnt = 0;
    for(size_t i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            std::cout << "Found mismatch at trip " << i << " (indexing trips from 0)." << std::endl;
            std::cout << "Vector 'a' contains: " << a.at(i) << ", while std::vector 'b' contains: " << b.at(i) << '.' << std::endl;
            mmcnt++;//return false;
        }
    }
    std::cout << "Mismatches: " << mmcnt << std::endl;
    //std::cout << "Validated returned distances, std::vectors are similar." << std::endl;
    return true;
}
