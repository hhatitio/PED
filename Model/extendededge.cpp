#include "extendededge.h"


ExtendedEdge::ExtendedEdge(ListGraph &g, ListGraph::Node u,
                           ListGraph::Node v, int pos_u, int pos_v){
    _adjacentNodes.push_back(g.id(u));
    _adjacentNodes.push_back(g.id(v));
    _adjacentNodes.push_back(pos_u);
    _adjacentNodes.push_back(pos_v);
    edge = g.addEdge(u,v);
    _id = g.id(edge);
    _length = 0;
}

bool ExtendedEdge::isAdjacentVertex(int id){
    for(unsigned int i = 0; i < _adjacentNodes.size(); i++)
        if(_adjacentNodes[i] == id)
            return true;
    return false;
}

void ExtendedEdge::addAdjacentNode(int id){
    _adjacentNodes.push_back(id);
}

void ExtendedEdge::addAdjacentNodePos(int pos){
    _adjacentNodesPos.push_back(pos);
}

void ExtendedEdge::setSize(int length){
    this->_length = length;
}

int ExtendedEdge::getId(){
    return _id;
}

int ExtendedEdge::getFirstNode(){
    return _adjacentNodes.at(0);
}

int ExtendedEdge::getFirstNodePos(){
    return _adjacentNodesPos.at(0);
}

int ExtendedEdge::getSecondNode(){
    return _adjacentNodes.at(1);
}

int ExtendedEdge::getSecondNodePos(){
    return _adjacentNodesPos.at(1);
}

int ExtendedEdge::getSize(){
    return _length;
}
