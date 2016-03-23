#ifndef VERTEXGRAPH
#define VERTEXGRAPH

#include <lemon/list_graph.h>
#include <Model/extendededge.h>

using namespace lemon;

class ExtendedNode{
public:
    ExtendedNode(ListGraph &g, int x, int y, int z);
    ~ExtendedNode();

    bool isAdjacentNode(int id);
    bool isIncidentEdge(int id);
    void addAdjacentNode(int id);
    void addAdjacentNodePos(int pos);
    std::vector<int> getAdjacentNodes();
    std::vector<int> getAdjacentNodesPos();
    void deleteAdjacentNode(int id);
    void deleteAdjacentNodePos(int pos);
    void deleteIncidentEdge(ExtendedEdge* e);
    int getNbIncidentEdges();
    std::vector<ExtendedEdge*> getIncidentEdges();
    ExtendedEdge* getIncidentEdge(ExtendedNode *v);
    void addIncidentEdge(ExtendedEdge* e);
    int getX();
    int getY();
    int getZ();
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    bool operator ==(ExtendedNode v);
    ListGraph::Node getNode();
    int getId();
    int getIndex(int n_cols, int n_rows, int n_slices);

private:
    int _id;
    ListGraph::Node node;
    std::vector<int> _adjacentNodes;
    std::vector<int> _adjacentNodesPos;
    std::vector<ExtendedEdge*> _incidentEdges;
    std::vector<int> _coord;
};

#endif // VERTEXGRAPH

