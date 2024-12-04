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
import argparse

# Author: Michal Cvach
# This script allows you to transform a map represented by a pair of .geojson files into the XenGraph format which is
# then used as an input for the Contraction Hierarchies creation. This script creates an integer variant of the
# XenGraph file.


def load_graph(nodes_filepath: str, edges_filepath: str):
    """
    Gets paths to nodes file and edges file (both in the GeoJSON format) and loads them using roadmaptools.
    """
    nodes_collection = load_geojson(nodes_filepath)
    edges_collection = load_geojson(edges_filepath)
    return nodes_collection, edges_collection


def create_xengraph_file(nodes_feature_collection, edges_feature_collection, output_filepath: str, precision=1):
    """
    Generates the actual graph file (suffix .xeng). Basically, the edges are processed one after each other and are
    transformed into a simple plain-text representation. The last parameter (precision) determines how precise the
    weights (which are basically travel times) should be. Precision '1' means the weighs will denote seconds,
    precision '10' means the weights will denote tenths of a second, precision '1000' means the weights will denote
    milliseconds and so on. Too big precision could cause overflow though, as the weights are loaded as unsigned ints.
    """
    mapping = {}
    i = 0
    for node in nodes_feature_collection['features']:
        mapping[node['properties']['node_id']] = i
        i = i + 1

    with open(output_filepath, 'w') as graph_file:
        print("XGI", str(len(nodes_feature_collection['features'])), str(len(edges_feature_collection['features'])),
              file=graph_file)
        for edge in edges_feature_collection['features']:
            if edge['properties']['maxspeed'] != 0:
                if 'oneway' in edge['properties']:
                    if edge['properties']['oneway'] == "yes":
                        flag = 1
                    else:
                        flag = 0
                else:
                    flag = 0
                print(mapping[edge['properties']['from_id']], mapping[edge['properties']['to_id']],
                      str(int(round((edge['properties']['length'] / (edge['properties']['maxspeed'] / 3.60)) * precision
                              ))), str(flag), file=graph_file)


def create_xenindices_file(nodes_feature_collection, output_filepath: str):
    """
    Generates the mapping file. This helps when the user wants to query using the original IDs from the GeoJSON files.
    In that case, the indices file is required as a mapping data for the query manager.
    """
    with open(output_filepath, 'w') as indices_file:
        print("XID", str(len(nodes_feature_collection['features'])), file=indices_file)
        for node in nodes_feature_collection['features']:
            print(node['properties']['node_id'], file=indices_file)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Allows to obtain a XenGraph representation from a graph represented'
                                                 ' by two GeoJSON files, one containing the nodes and the other '
                                                 'containing the edges.',
                                     epilog='This script will generate two files, one with the .xeng suffix containing '
                                            'the graph in the XenGraph format and a second one with the .xeni suffix '
                                            'containing the mapping information.'
                                     )
    parser.add_argument('nodes_input_path', metavar='N', nargs=1, type=str,
                        help='Path to the nodes GeoJSON file.')
    parser.add_argument('edges_input_path', metavar='E', nargs=1, type=str,
                        help='Path to the edges GeoJSON file.')
    parser.add_argument('precision', metavar='P', nargs=1, type=float,
                        help='Precision for the edge weights (1 means seconds, 1000 means milliseconds...).')
    parser.add_argument('output_path', metavar='O', nargs=1, type=str,
                        help='Path where you want to output the transformed '
                             'files without suffixes (those are added automatically).')
    args = parser.parse_args()
    args = vars(args)

    nodes_file = args['nodes_input_path'][0]
    edges_file = args['edges_input_path'][0]
    graph_output_file = args['output_path'][0]+'.xeng'
    indices_output_file = args['output_path'][0]+'.xeni'
    precision = args['precision'][0]

    print("Loading graph...")
    nodes, edges = load_graph(nodes_file, edges_file)
    print("Graph loaded, creating xengraph files.")
    create_xengraph_file(nodes, edges, graph_output_file, precision)
    print("Created graph file (" + graph_output_file + ").")
    create_xenindices_file(nodes, indices_output_file)
    print("Created auxiliary indices file (" + indices_output_file + ").")
