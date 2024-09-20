//
// Author: horakj31 (Jan Horak)
// Created on: 25.07.24
//

#ifndef LOCATIONTRANSFORMER_H
#define LOCATIONTRANSFORMER_H

#include <vector>

class LocationTransformer {
public:
    static void transformLocations(
            std::vector<std::pair<double, double>>& gpsLocations,
            std::vector<std::pair<double, double>>& projectedLocations);
};


#endif //LOCATIONTRANSFORMER_H
