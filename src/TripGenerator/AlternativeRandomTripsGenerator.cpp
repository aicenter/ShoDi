//
// Author: Xenty (Michal Cvach)
// Created on: 27.8.18
//

#define NODES_CNT 23947347 //23947347
#define TRIPS_AMOUNT 1000

#include <cmath>
#include <cstdlib>
#include <cstdio>

//______________________________________________________________________________________________________________________
inline unsigned int randomNodeID(unsigned int n) {
    return (unsigned int)(rand() / (double)(RAND_MAX+1.0) * n);
}

// Here we generate trips the same way the researches did in their implementation.
//______________________________________________________________________________________________________________________
int main() {

    printf("%u\n", TRIPS_AMOUNT);
    srand(1);
    for(unsigned int i = 0; i < TRIPS_AMOUNT; i++) {
        unsigned int s = randomNodeID(NODES_CNT);
        unsigned int t = randomNodeID(NODES_CNT);
        printf("%u %u\n", s, t);
    }

    return 0;
}

