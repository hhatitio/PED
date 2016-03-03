#include "extendednode.h"

ExtendedNode::ExtendedNode(ListGraph &g, int x, int y, int z){
    _id = g.id(g.addNode());
    
    _coord.push_back(x);
    _coord.push_back(y);
    _coord.push_back(z);
}

bool ExtendedNode::operator ==(ExtendedNode v){
    return (this->_id == v._id);
}

bool ExtendedNode::isAdjacentNode(int id){
    for(unsigned int i = 0; i < _adjacentNodes.size(); i++)
        if(_adjacentNodes[i] == id)
            return true;
    return false;
}

void ExtendedNode::addAdjacentNode(int id){
    _adjacentNodes.push_back(id);
}

void ExtendedNode::deleteAdjacentNode(int id){
    for(unsigned int i = 0; i < _adjacentNodes.size(); i++){
        if(_adjacentNodes[i] == id){
            _adjacentNodes.erase(_adjacentNodes.begin()+i);
            return;
        }
    }
}

int ExtendedNode::getNbIncidentEdges(){
    return _incidentEdges.size();
}

void ExtendedNode::addIncidentEdges(int id){
    _incidentEdges.push_back(id);
}

int ExtendedNode::getId(){
    return _id;
}

int ExtendedNode::getX(){
    return _coord[0];
}

int ExtendedNode::getY(){
    return _coord[1];
}

int ExtendedNode::getZ(){
    return _coord[2];
}

void ExtendedNode::setX(int x){
    _coord[0]=x;
}

void ExtendedNode::setY(int y){
    _coord[1]=y;
}

void ExtendedNode::setZ(int z){
    _coord[2]=z;
}
