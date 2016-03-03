#include "extendededge.h"


ExtendedEdge::ExtendedEdge(ListGraph &g, int u, int v){
    _id = g.id(g.addEdge(g.nodeFromId(u), g.nodeFromId(v)));
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

void ExtendedEdge::setSize(int length){
    this->_length = length;
}

int ExtendedEdge::getId(){
    return _id;
}

int ExtendedEdge::getSize(){
    return _length;
}
