#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace johnson {
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
            boost::no_property, boost::property<boost::edge_weight_t, int> > Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef std::pair<int,int> Edge;

    typedef struct graph {
        int V;
        int E;
        Edge *edge_array;
        int* weights;
    } graph_t;

    graph_t* johnson_init(std::vector<int>);

    graph_t* johnson_init2(const int n, const double p, const unsigned long seed);

    typedef struct edge {
        int u;
        int v;
    } edge_t;

    void free_graph(graph_t* g);
    void johnson_parallel(graph_t *gr, int* output);
}
