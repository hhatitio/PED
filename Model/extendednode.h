#ifndef VERTEXGRAPH
#define VERTEXGRAPH

#include <lemon/list_graph.h>

using namespace lemon;

class ExtendedNode{
public:
    ExtendedNode(ListGraph &g, int x, int y, int z);

    bool isAdjacentNode(int id);
    void addAdjacentNode(int id);
    std::vector<int> getAdjacentNode();
    void deleteAdjacentNode(int id);
    int getNbIncidentEdges();
    void addIncidentEdges(int id);
    int getX();
    int getY();
    int getZ();
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    bool operator ==(ExtendedNode v);
    int getId();
    std::vector<int> getCoordOutOfIndex(int n_cols, int n_rows, int n_slices,int idx);
    int getIndex(int n_cols, int n_rows, int n_slices);

private:
    int _id;
    std::vector<int> _adjacentNodes;
    std::vector<int> _incidentEdges;
    std::vector<int> _coord;
};

#endif // VERTEXGRAPH

