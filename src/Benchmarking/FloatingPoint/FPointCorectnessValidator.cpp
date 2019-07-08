//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "FPointCorectnessValidator.h"
#include <cstdio>

//______________________________________________________________________________________________________________________
bool FPointCorectnessValidator::validate(vector < double > & a, vector < double > & b) {
    for (unsigned int i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            return false;
        }
    }
    return true;
}

//______________________________________________________________________________________________________________________
bool FPointCorectnessValidator::validateVerbose(vector < double > & a, vector < double > & b) {
    unsigned int mmcnt = 0;
    for (unsigned int i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            printf("Found mismatch at trip %u (indexing trips from 0).\n", i);
            printf("Vector 'a' contains: %llu, while vector 'b' contains: %llu.\n", a.at(i), b.at(i));
            mmcnt++;//return false;
        }
    }
    printf("Mismatches: %u\n", mmcnt);
    //printf("Validated returned distances, vectors are similar.\n");
    return true;
}
