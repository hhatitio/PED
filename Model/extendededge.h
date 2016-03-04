#ifndef EDGEGRAPH
#define EDGEGRAPH

#include <lemon/list_graph.h>
#include "extendednode.h"

using namespace lemon;

class ExtendedEdge{
public:
    ExtendedEdge(ListGraph &g, int u, int v);
//    ExtendedEdge(ListGraph &g, ListGraph::Node u, ListGraph::Node v);
//    ExtendedEdge(ListGraph &g, ExtendedNode u, ExtendedNode v);
    
    bool isAdjacentVertex(int id);
    void addAdjacentNode(int id);
    int getSize();
    void setSize(int length);
    int getId();

private:
    int _id;
    int _length;
    std::vector<int> _adjacentNodes;
};

#endif // EDGEGRAPH

