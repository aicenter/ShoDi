//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#include <cstdio>
#include "CorrectnessValidator.h"

//______________________________________________________________________________________________________________________
bool CorrectnessValidator::validate(vector < unsigned int > & a, vector < unsigned int > & b) {
    for (unsigned int i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            return false;
        }
    }
    return true;
}

//______________________________________________________________________________________________________________________
bool CorrectnessValidator::validateVerbose(vector < unsigned int > & a, vector < unsigned int > & b) {
    unsigned int mmcnt = 0;
    for (unsigned int i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            printf("Found mismatch at trip %u (indexing trips from 0).\n", i);
            printf("Vector 'a' contains: %u, while vector 'b' contains: %u.\n", a.at(i), b.at(i));
            mmcnt++;//return false;
        }
    }
    printf("Mismatches: %u\n", mmcnt);
    //printf("Validated returned distances, vectors are similar.\n");
    return true;
}
