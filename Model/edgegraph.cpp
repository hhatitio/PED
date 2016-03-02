#include "edgegraph.h"


EdgeGraph::EdgeGraph(ListGraph &g){
    edge = g.addEdge();
    length = 0;
}

bool EdgeGraph::isAdjacentVertex(VertexGraph* v){
    for(int i=0; i<adjacentNodes.size(); i++){
        if(adjacentNodes.at(i)==v){return true;}
    }
    return false;
}

void EdgeGraph::addAdjacentNode(VertexGraph *v){
    adjacentNodes.push_back(v);
}

void EdgeGraph::setSize(int length){
    this->length = length;
}

int EdgeGraph::getSize(){
    return length;
}
