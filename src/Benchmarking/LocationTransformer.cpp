//
// Author: horakj31 (Jan Horak)
// Created on: 25.07.24
//

#include "LocationTransformer.h"
#include <proj.h>
#include <proj/coordinateoperation.hpp>
#include <proj/crs.hpp>
#include <proj/io.hpp>
#include <proj/util.hpp> // for nn_dynamic_pointer_cast

#include <iostream>
#include <cmath>

#include <cassert>



using namespace NS_PROJ::crs;
using namespace NS_PROJ::io;
using namespace NS_PROJ::operation;
using namespace NS_PROJ::util;

void LocationTransformer::transformLocations(std::vector<std::pair<double, double>>& gpsLocations,
                                             std::vector<std::pair<double, double>>& projectedLocations) {
    int utm_zone = static_cast<int>(std::floor((gpsLocations[0].first + 180) / 6)) + 1;

    auto dbContext = DatabaseContext::create();
    auto authFactory = AuthorityFactory::create(dbContext, std::string());
    auto coord_op_ctxt = CoordinateOperationContext::create(authFactory, nullptr, 0.0);
    auto authFactoryEPSG = AuthorityFactory::create(dbContext, "EPSG");
    auto sourceCRS = authFactoryEPSG->createCoordinateReferenceSystem("4326");
    std::string utm_proj_string = "+proj=utm +zone=" + std::to_string(utm_zone) + " +datum=WGS84 +type=crs";
    auto targetCRS = NN_CHECK_THROW(nn_dynamic_pointer_cast<CRS>(createFromUserInput(utm_proj_string, dbContext)));
    auto list = CoordinateOperationFactory::create()->createOperations(sourceCRS, targetCRS, coord_op_ctxt);
    assert(!list.empty());
    PJ_CONTEXT *ctx = proj_context_create();
    auto transformer = list[0]->coordinateTransformer(ctx);

    for (auto loc: gpsLocations) {
        PJ_COORD c = {{loc.second, loc.first, 0.0, HUGE_VAL}};
        c = transformer->transform(c);
        projectedLocations.emplace_back(c.v[0], c.v[1]);
    }

    proj_context_destroy(ctx);
}
