#ifndef EDGEGRAPH
#define EDGEGRAPH

#include <unordered_map>
#include <lemon/list_graph.h>

using namespace lemon;

class ExtendedEdge{
public:
    ExtendedEdge(ListGraph &g, ListGraph::Node u,
                 ListGraph::Node v, int pos_u, int pos_v);
    ~ExtendedEdge();

    bool isAdjacentVertex(int id);
    void addAdjacentNode(int id);
    void addAdjacentNodePos(int pos);
    void addAngle(int idEdge, double val);
    void deleteAngle(int idEdge);
    bool containAngle(int idEdge);
    int getSize();
    void setSize(int length);
    int getId();
    int getFirstNode();
    int getSecondNode();
    int getFirstNodePos();
    int getSecondNodePos();
    int getOppositeNode(int id);
    int getOppositeNodePos(int id);
    double getAngle(int idEdge);

    //return the list of adjacent node position
    std::vector<int> getAdjacentNodesPos();


    
private:
    int _id;
    ListGraph::Edge edge;
    int _length;
    std::unordered_map<int, double> angles;

    //contain the id of adjacent nodes
    std::vector<int> _adjacentNodes;

    //contain the position of adjacent nodes
    std::vector<int> _adjacentNodesPos;

};

#endif // EDGEGRAPH

