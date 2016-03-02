#include "vertexgraph.h"

VertexGraph::VertexGraph(ListGraph &g, int x, int y, int z){
    node = g.addNode();
    coord.push_back(x);
    coord.push_back(y);
    coord.push_back(z);
}

bool VertexGraph::operator ==(VertexGraph *v){
    return (this->getX()==v->getX()&&this->getY()==v->getY()
            &&this->getZ()==v->getZ());
}

bool VertexGraph::operator ==(VertexGraph v){
    return (this->getX()==v.getX()&&this->getY()==v.getY()
            &&this->getZ()==v.getZ());
}

bool VertexGraph::isAdjacentNode(VertexGraph* v){
    for(int i=0; i<adjacentNodes.size(); i++){
        if(adjacentNodes.at(i)==v){return true;}
    }
    return false;
}

void VertexGraph::addAdjacentNode(VertexGraph *v){
    adjacentNodes.push_back(v);
}

void VertexGraph::deleteAdjacentNode(VertexGraph *v){
    int i = 0;
    while(adjacentNodes[i]!=v){i++;}
    adjacentNodes.erase(adjacentNodes.begin()+i);
}

int VertexGraph::getNbIncidentEdges(){
    return incidentEdges.size();
}

void VertexGraph::addIncidentEdges(EdgeGraph *e){
    incidentEdges.push_back(e);
}

int VertexGraph::getX(){
    return coord[0];
}

int VertexGraph::getY(){
    return coord[1];
}

int VertexGraph::getZ(){
    return coord[2];
}

void VertexGraph::setX(int x){
    coord[0]=x;
}

void VertexGraph::setY(int y){
    coord[1]=y;
}

void VertexGraph::setZ(int z){
    coord[2]=z;
}
