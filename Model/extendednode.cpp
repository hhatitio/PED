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

ListGraph::Node ExtendedNode::getNode() {
    return node;
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
    if(!isAdjacentNode(id)){
    _adjacentNodes.push_back(id);
    }
}

void ExtendedNode::addAdjacentNodePos(int pos){
    for(unsigned int i = 0; i < _adjacentNodesPos.size(); i++){
        if(_adjacentNodesPos[i] == pos){
            return;
        }
    }
    _adjacentNodesPos.push_back(pos);
}

std::vector<int> ExtendedNode::getAdjacentNodes(){
    return _adjacentNodes;
}

std::vector<int> ExtendedNode::getAdjacentNodesPos(){
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
    for(unsigned int i = 0; i < _adjacentNodesPos.size(); i++){
        if(_adjacentNodesPos[i] == pos){
            _adjacentNodesPos.erase(_adjacentNodesPos.begin()+i);
            return;
        }
    }
}

void ExtendedNode::deleteIncidentEdge(ExtendedEdge *e){
    for(unsigned int i = 0; i < _incidentEdges.size(); i++){
        ExtendedEdge *edge = _incidentEdges[i];
        if(edge->getId()==e->getId()){
            _incidentEdges.erase(_incidentEdges.begin()+i);
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

ExtendedEdge* ExtendedNode::getIncidentEdge(ExtendedNode *v){
    for(unsigned int i = 0; i < _adjacentNodes.size(); i++){
        if(_adjacentNodes[i] == v->getId()){
            return _incidentEdges.at(i);
        }
    }
    return NULL;
}
bool ExtendedNode::isIncidentEdge(int id){
    for(unsigned int i = 0; i < _incidentEdges.size(); i++){
        ExtendedEdge *edge = _incidentEdges[i];
        if(edge->getId() == id){
            return true;
        }
    }
    return false;
}

void ExtendedNode::addIncidentEdge(ExtendedEdge* e){
    if(!isIncidentEdge(e->getId())){
     _incidentEdges.push_back(e);
    }
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
