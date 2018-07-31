//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "GraphBuilder/Loader.h"
#include "Dijkstra/BasicDijkstra.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Timer/Timer.h"
#include "TNR/Grid.h"
#include "Utils/MinMaxCoordsFinder.h"

int main() {
    //Loader graphLoader = Loader("../input/USA-road-t.USA.gr");
    //Graph * graph = graphLoader.loadGraph();
    //Loader tripsLoader = Loader("../input/100trips1");
    //vector< pair < unsigned int, unsigned int > > trips;
    //tripsLoader.loadTrips(trips);
    //DijkstraBenchmark::runAndMeasure(trips, *graph);
    Loader coordsLoader = Loader("../input/USA-road-d.USA.co");
    vector< pair < int, int > > coords;
    coordsLoader.loadCoordinates(coords);
    Grid grid;
    MinMaxCoordsFinder::setMinMaxCoords(coords, grid);
    //printf("Min coord1: %i, max coord1: %i, min coord2: %i, max coord2: %i.\n", grid.getMincoord1(), grid.getMaxcoord1(), grid.getMincoord2(), grid.getMaxcoord2());

    Timer gridCreationTimer("Grid creation from nodes");
    gridCreationTimer.begin();

    grid.createGridFromNodes(coords);

    gridCreationTimer.finish();
    gridCreationTimer.printMeasuredTime();

    grid.printSomeGridInfo();


    //delete graph;
    return 0;
}