#ifndef EDGEGRAPH
#define EDGEGRAPH

#include <lemon/list_graph.h>

using namespace lemon;

class ExtendedEdge{
public:
    ExtendedEdge(ListGraph &g, ListGraph::Node u,
                 ListGraph::Node v, int pos_u, int pos_v);
//    ExtendedEdge(ListGraph &g, ListGraph::Node u, ListGraph::Node v);
//    ExtendedEdge(ListGraph &g, ExtendedNode u, ExtendedNode v);

    bool isAdjacentVertex(int id);
    void addAdjacentNode(int id);
    void addAdjacentNodePos(int pos);
    int getSize();
    void setSize(int length);
    int getId();
    int getFirstNode();
    int getSecondNode();
    int getFirstNodePos();
    int getSecondNodePos();

private:
    int _id;
    ListGraph::Edge edge;
    int _length;
    std::vector<int> _adjacentNodes;
    std::vector<int> _adjacentNodesPos;
};

#endif // EDGEGRAPH

