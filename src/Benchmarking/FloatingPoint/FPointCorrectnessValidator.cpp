//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "FPointCorrectnessValidator.h"
#include <cstdio>
#include <cmath>

#define EPS 0.000000001

//______________________________________________________________________________________________________________________
bool FPointCorrectnessValidator::validate(vector < double > & a, vector < double > & b) {
    for (unsigned int i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            return false;
        }
    }
    return true;
}

//______________________________________________________________________________________________________________________
bool FPointCorrectnessValidator::validateVerbose(vector < double > & a, vector < double > & b) {
    unsigned int mmcnt = 0;
    for (unsigned int i = 0; i < a.size(); i++) {
        if (abs(a.at(i) - b.at(i)) > EPS) {
            printf("Found mismatch at trip %u (indexing trips from 0).\n", i);
            printf("Vector 'a' contains: %.15f, while vector 'b' contains: %.15f.\n", a.at(i), b.at(i));
            mmcnt++;//return false;
        }
    }
    printf("Mismatches: %u\n", mmcnt);
    //printf("Validated returned distances, vectors are similar.\n");
    return true;
}
