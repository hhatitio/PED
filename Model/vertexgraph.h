#ifndef VERTEXGRAPH
#define VERTEXGRAPH

#include <lemon/list_graph.h>
#include "edgegraph.h"

using namespace lemon;

class VertexGraph{
public:
    VertexGraph(ListGraph &g, int x, int y, int z);

    bool isAdjacentNode(VertexGraph* v);
    void addAdjacentNode(VertexGraph *v);
    void deleteAdjacentNode(VertexGraph *v);
    int getNbIncidentEdges();
    void addIncidentEdges(EdgeGraph *e);
    int getX();
    int getY();
    int getZ();
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    bool operator ==(VertexGraph *v);
    bool operator ==(VertexGraph v);

private:
    ListGraph::Node node;
    std::vector<VertexGraph*> adjacentNodes;
    std::vector<EdgeGraph*> incidentEdges;
    std::vector<int> coord;
};

#endif // VERTEXGRAPH

