#include "extendednode.h"

ExtendedNode::ExtendedNode(ListGraph &g, int x, int y, int z){
    node = g.addNode();
    _id = g.id(node);
    _coord.push_back(x);
    _coord.push_back(y);
    _coord.push_back(z);
}

bool ExtendedNode::operator ==(ExtendedNode v){
    return (this->node == v.getNode());
}

int ExtendedNode::getIndex(int n_cols, int n_rows, int n_slices){

    int i = getX();
    int j = getY();
    int k = getZ();
    // IF Coordinate is out of boundaries associate intensity with -1
    if(i < 0||i>=n_rows){
        return -1;
    }
    if(j < 0||j>=n_cols){
        return -1;
    }
    if(k < 0||k>=n_slices){
        return -1;
    }

    // Formula to calculate IDX out of 3D Dimensional coordinate
    int sizeLayer = n_cols*n_rows;
    return (k*sizeLayer)+(j*n_rows)+i;
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

void ExtendedNode::addAdjacentNodePos(int pos){
    _adjacentNodes.push_back(pos);
}

std::vector<int> ExtendedNode::getAdjacentNode(){
    return _adjacentNodes;
}

std::vector<int> ExtendedNode::getAdjacentNodePos(){
    return _adjacentNodesPos;
}

void ExtendedNode::deleteAdjacentNode(int id){
    for(unsigned int i = 0; i < _adjacentNodes.size(); i++){
        if(_adjacentNodes[i] == id){
            _adjacentNodes.erase(_adjacentNodes.begin()+i);
            return;
        }
    }
}

void ExtendedNode::deleteAdjacentNodePos(int pos){
    for(unsigned int i = 0; i < _adjacentNodes.size(); i++){
        if(_adjacentNodes[i] == pos){
            _adjacentNodes.erase(_adjacentNodes.begin()+i);
            return;
        }
    }
}

int ExtendedNode::getNbIncidentEdges(){
    return _incidentEdges.size();
}

std::vector<ExtendedEdge*> ExtendedNode::getIncidentEdges(){
    return _incidentEdges;
}

void ExtendedNode::addIncidentEdges(ExtendedEdge* e){
    _incidentEdges.push_back(e);
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
