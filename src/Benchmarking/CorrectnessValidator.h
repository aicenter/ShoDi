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

#ifndef TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H
#define TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H

#include <vector>
#include "../constants.h"



/**
 * This class is used mainly for validation whether the Dijkstra and Contraction Hierarchies provide the same results.
 * Since Dijkstra is pretty simple, it serves us as a reference - if Contraction Hierarchies results differ from the
 * Dijkstra, something is probably wrong with the Contraction Hierarchies implementation. This class can be also used
 * to compare results of two Contaction Hierarchies implementation (or any algorithms for that matter), but in that
 * case you should always be sure that at least one of the compared implementations gives correct results.
 */
class CorrectnessValidator {
public:
    /**
     * Compares the two std::vectors containing some computed distances element by element. Returns either 'true' if all
     * the pairs are exactly the same, of 'false' if at least one mismatch is found.
     *
     * @param a[in] One std::vector of values to be compared.
     * @param b[in] A second std::vector of values to be compared.
     * @return Returns 'true' if all the pairs of values in the two std::vectors are the same, 'false' otherwise.
     */
    static bool validate(std::vector<dist_t> &a, std::vector<dist_t> &b);

    /**
     * Compares the two std::vectors containing some computed distances element by element. Prints out the all the mismatches
     * along with the values in both of the std::vectors on the mismatch positions. Also prints the number of mismatches
     * at the end.
     *
     * @param a[in] One std::vector of values to be compared.
     * @param b[in] A second std::vector of values to be compared.
     * @return Currently returns 'true' all the time, so you should make decisions based on the printed output.
     * If you want some machine processing for example, use the basic 'validate' function instead.
     */
    static bool validateVerbose(std::vector<dist_t> &a, std::vector<dist_t> &b);

private:
};


#endif //TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H
