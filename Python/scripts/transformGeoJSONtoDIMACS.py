#
# MIT License
#
# Copyright (c) 2024 Czech Technical University in Prague
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.#
from shortestdistances.inout import load_geojson

# Author: Michal Cvach
# This script allows you to transform a map represented by a pair of .geojson files into the DIMACS format which is
# one of the formats supported by the Contraction Hierarchies creation application. It is preferred to use the
# XenGraph format when transforming GeoJSON maps to input files for Contraction Hierarchies, as the XenGraph format
# is more simple and was used for all the test data and so on, but this script should also work. The weights in this
# case will be travel times in seconds.


def load_graph(nodes_filepath: str, edges_filepath: str):
    """
    Gets paths to nodes file and edges file (both in the GeoJSON format) and loads them using roadmaptools.
    """
    nodes_collection = load_geojson(nodes_filepath)
    edges_collection = load_geojson(edges_filepath)
    return nodes_collection, edges_collection


def create_DIMACS_file(nodes_feature_collection, edges_feature_collection, output_filepath: str):
    """
    Generates the graph file in the DIMACS format. The DIMACS format is described here:
    http://users.diag.uniroma1.it/challenge9/format.shtml#graph along with some examples.
    """
    mapping = {}
    i = 0
    for node in nodes_feature_collection['features']:
        mapping[node['properties']['node_id']] = i
        i = i + 1

    with open(output_filepath, 'w') as graph_file:
        print("c This file was created using a simple python script.", file=graph_file)
        print("c This should only be used for debug purposes.", file=graph_file)
        print("p sp", str(len(nodes_feature_collection['features'])), str(len(edges_feature_collection['features'])),
              file=graph_file)
        for edge in edges_feature_collection['features']:
            print("a", str(int(mapping[edge['properties']['from_id']])+1), str(int(mapping[edge['properties']['to_id']])+1), str(int(round(
                edge['properties']['length'] / (edge['properties']['maxspeed'] / 3.60)))), file=graph_file)


if __name__ == '__main__':
    nodes_file = "nodes.geojson"
    edges_file = "edges.geojson"
    graph_output_file = "graph.gr"
    print("Loading graph...")
    nodes, edges = load_graph(nodes_file, edges_file)
    print("Graph loaded, creating DIMACS graph file.")
    create_DIMACS_file(nodes, edges, graph_output_file)
    print("Created graph file (" + graph_output_file + ").")