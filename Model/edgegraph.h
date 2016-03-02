#ifndef EDGEGRAPH
#define EDGEGRAPH

#include <lemon/list_graph.h>
#include "vertexgraph.h"

using namespace lemon;

class EdgeGraph{
public:
    EdgeGraph(ListGraph &g);
    ~EdgeGraph();
    bool isAdjacentVertex(VertexGraph* v);
    void addAdjacentNode(VertexGraph* v);
    int getSize();
    void setSize(int length);

private:
    ListGraph::Edge edge;
    std::vector<VertexGraph&> adjacentNodes;
    int length;
};

#endif // EDGEGRAPH

