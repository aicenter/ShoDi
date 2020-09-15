//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#include <iostream>
#include "CorrectnessValidator.h"

//______________________________________________________________________________________________________________________
bool CorrectnessValidator::validate(vector<dist_t> &a, vector<dist_t> &b) {
    for(size_t i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            return false;
        }
    }
    return true;
}

//______________________________________________________________________________________________________________________
bool CorrectnessValidator::validateVerbose(vector<dist_t> &a, vector<dist_t> &b) {
    unsigned int mmcnt = 0;
    for(size_t i = 0; i < a.size(); i++) {
        if (a.at(i) != b.at(i)) {
            cout << "Found mismatch at trip " << i << " (indexing trips from 0)." << endl;
            cout << "Vector 'a' contains: " << a.at(i) << ", while vector 'b' contains: " << b.at(i) << '.' << endl;
            mmcnt++;//return false;
        }
    }
    cout << "Mismatches: " << mmcnt << endl;
    //cout << "Validated returned distances, vectors are similar." << endl;
    return true;
}
