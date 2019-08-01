from roadmaptools import inout

# Author: Michal Cvach
# This script allows you to transform a map represented by a pair of .geojson files into the XenGraph format which is
# then used as an input for the Contraction Hierarchies creation. This script creates a floating point variant of the
# XenGraph file.


def load_graph(nodes_filepath: str, edges_filepath: str):
    """
    Gets paths to nodes file and edges file (both in the GeoJSON format) and loads them using roadmaptools.
    """
    nodes_collection = inout.load_geojson(nodes_filepath)
    edges_collection = inout.load_geojson(edges_filepath)
    return nodes_collection, edges_collection


def create_xengraph_file(nodes_feature_collection, edges_feature_collection, output_filepath: str):
    """
    Generates the actual graph file (suffix .xeng). Basically, the edges are processed one after each other and are
    transformed into a simple plain-text representation. Weights are travel times in seconds.
    """
    mapping = {}
    i = 0
    for node in nodes_feature_collection['features']:
        mapping[node['properties']['node_id']] = i
        i = i + 1

    with open(output_filepath, 'w') as graph_file:
        print("XGF", str(len(nodes_feature_collection['features'])), str(len(edges_feature_collection['features'])),
              file=graph_file)
        for edge in edges_feature_collection['features']:
            if 'oneway' in edge['properties']:
                if edge['properties']['oneway'] == "yes":
                    flag = 1
                else:
                    flag = 0
            else:
                flag = 0
            print(mapping[edge['properties']['from_id']], mapping[edge['properties']['to_id']], str(
                edge['properties']['length'] / (edge['properties']['maxspeed'] / 3.60)), str(flag), file=graph_file)


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
    nodes_file = "Prague_nodes.geojson"
    edges_file = "Prague_edges.geojson"
    graph_output_file = "Prague_graph.xeng"
    indices_output_file = "Prague_graph.xeni"

    print("Loading graph...")
    nodes, edges = load_graph(nodes_file, edges_file)
    print("Graph loaded, creating xengraph files.")
    create_xengraph_file(nodes, edges, graph_output_file)
    print("Created graph file (" + graph_output_file + ").")
    create_xenindices_file(nodes, indices_output_file)
    print("Created auxiliary indices file (" + indices_output_file + ").")
