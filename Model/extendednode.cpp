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

int ExtendedNode::getIndex(int n_cols, int n_rows, int n_slices){
    
    int i = getX();
    int j = getY();
    int k = getZ();
    // IF Coordinate is out of boundaries associate intensity with -1
    if(i < 0||i>=n_cols){
        return -1;
    }
    if(j < 0||j>=n_rows){
        return -1;
    }
    if(k < 0||k>=n_slices){
        return -1;
    }
    
    // Formula to calculate IDX out of 3D Dimensional coordinate
    int sizeLayer = n_cols*n_rows;
    return (k*sizeLayer)+(j*n_cols)+i;
}

/*
 * getCoordOutOfIndex(int idx)
 * INPUT:
 * Scalar INDEX of the Voxel to access intensity
 * in "std::vector<int> data;"
 * OUTPUT:
 * 3D Dimensional coordinate of voxel point
 */

std::vector<int> ExtendedNode::getCoordOutOfIndex(int n_cols, int n_rows, int n_slices,int idx){
    
    // Mathematical formulas to calculate 3D Coordinte of voxel
    int sizeLayer = n_cols*n_rows;
    int z =(int)idx/sizeLayer;
    int idx2D = idx-z*sizeLayer;
    int y =(int) idx2D/n_cols;
    int x = idx2D%n_cols;
    // Store the calculated coordinates
    std::vector<int> coord;
    coord.push_back(x);
    coord.push_back(y);
    coord.push_back(z);
    return coord;
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

std::vector<int> ExtendedNode::getAdjacentNode(){
    return _adjacentNodes;
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
