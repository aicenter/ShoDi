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
