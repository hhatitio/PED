#include "skeletongraph.h"
#include <qtextstream.h>
#include <limits>
#include <string>

Image *neighboorMap;
std::unordered_map<int, int> enhanceNeighboorMap;
std::unordered_map<int, ExtendedNode*> voxels;
std::vector<int> visitedVoxel;

SkeletonGraph::SkeletonGraph() {

}

SkeletonGraph::SkeletonGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(skeletonIm3D);
}

SkeletonGraph::~SkeletonGraph() {}

void SkeletonGraph::setGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    //skeletonIm3D = new Image3D<short int>(im_const);
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(im_const);
}

void addVisitedVoxel(int pos){
    if(visitedVoxel.size()==10){
        visitedVoxel.pop_back();
    }
    visitedVoxel.insert(visitedVoxel.begin(),pos);
}

bool isVisited(int pos){
    for(unsigned int i = 0; i < visitedVoxel.size(); i++)
        if(visitedVoxel[i] == pos)
            return true;
    return false;
}

/*
 * getCoordOutOfIndex(int idx)
 * INPUT:
 * Scalar INDEX of the Voxel to access intensity
 * in "std::vector<int> data;"
 * OUTPUT:
 * 3D Dimensional coordinate of voxel point
 */

std::vector<int> getCoordOutOfIndex(int n_cols, int n_rows, int n_slices,int idx){

    // Mathematical formulas to calculate 3D Coordinte of voxel
    int sizeLayer = n_cols*n_rows;
    int z =(int)idx/sizeLayer;
    int idx2D = idx-z*sizeLayer;
    int y =(int) idx2D/n_rows;
    int x = idx2D%n_rows;
    // Store the calculated coordinates
    std::vector<int> coord;
    coord.push_back(x);
    coord.push_back(y);
    coord.push_back(z);
    return coord;
}

void computeEnhanceMapFromNeighboorMap(){
    int nb_rows = neighboorMap->n_rows;
    int nb_cols = neighboorMap->n_cols;
    int nb_slices = neighboorMap->n_slices;
    for (auto it = voxels.begin(); it!= voxels.end(); ++it){
        ExtendedNode *n = it->second;
        int x = n->getX();
        int y = n->getY();
        int z = n->getZ();
        int pos = x+(y*nb_rows)+(z*nb_cols*nb_rows);
        int neighboorHoodSum = 0;
        for (int i = -1; i <= 1 ; ++i) {
            for (int j = -1; j <= 1 ; ++j) {
                for (int k = -1; k <= 1 ; ++k) {
                    if(i!=0 || j!=0 || k!=0){

                        int x1 = x+i;
                        int y1 = y+j;
                        int z1 = z+k;

                        int n_pos = x1+(y1*nb_rows)+(z1*nb_cols*nb_rows);

                        if(x1 >= 0   && y1 >= 0   && z1 >= 0   &&
                                x1 < nb_rows && y1 < nb_cols && z1 < nb_slices) {

                            neighboorHoodSum += neighboorMap->at(n_pos);
                        }
                    }
                }
            }
        }
        enhanceNeighboorMap.insert({pos,neighboorHoodSum});
    }
}

void computeNeighboorMap(Image3D<short int> *im){
    neighboorMap = new Image3D<short int>(*im);
    int nb_cols = neighboorMap->n_cols;
    int nb_rows = neighboorMap->n_rows;
    int nb_slices = neighboorMap->n_slices;
    for (auto it = voxels.begin(); it!= voxels.end(); ++it){
        ExtendedNode *n = it->second;
        int x = n->getX();
        int y = n->getY();
        int z = n->getZ();
        int pos = x+(y*nb_rows)+(z*nb_cols*nb_rows);
        neighboorMap->at(pos) = 0;
        for (int i = -1; i <= 1 ; ++i) {
            for (int j = -1; j <= 1 ; ++j) {
                for (int k = -1; k <= 1 ; ++k) {
                    if(i!=0 || j!=0 || k!=0){

                        int x1 = x+i;
                        int y1 = y+j;
                        int z1 = z+k;

                        int n_pos = x1+(y1*nb_rows)+(z1*nb_cols*nb_rows);

                        if(x1 >= 0   && y1 >= 0   && z1 >= 0   &&
                                x1 < nb_rows && y1 < nb_cols && z1 < nb_slices) {

                            if (im->at(n_pos) == 255) {
                                neighboorMap->at(pos) += 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

int nextVoxelPosition(int x, int y, int z, Image3D<short int>& ImTmp){
    int nb_cols = ImTmp.n_cols;
    int nb_rows = ImTmp.n_rows;
    int nb_slices = ImTmp.n_slices;
    int pos = x+(y*nb_rows)+(z*nb_cols*nb_rows);
    int n_pos = -1;
    int val_n = 0;
    for (int i = -1; i <= 1 ; ++i) {
        for (int j = -1; j <= 1 ; ++j) {
            for (int k = -1; k <= 1 ; ++k) {
                if(i!=0 || j!=0 || k!=0){

                    int x1 = x+i;
                    int y1 = y+j;
                    int z1 = z+k;
                    int n_posTmp = x1+(y1*nb_rows)+(z1*nb_cols*nb_rows);
                    if(x1 >= 0   && y1 >= 0   && z1 >= 0   &&
                            x1 < nb_rows && y1 < nb_cols && z1 < nb_slices) {
                        //int val = ImTmp.at(n_posTmp);
                        if(!isVisited(n_posTmp)){
                            int val = neighboorMap->at(n_posTmp);
                            //int val_nTmp = enhanceNeighboorMap.at(n_posTmp);
                            //if(val!=254 && val!=150 && val_nTmp>val_n){
                            if(val>val_n){
                                //val_n = val_nTmp;
                                //std::cout << "nb_neighboor node_n :" << val << std::endl;

                                val_n = val;
                                n_pos = n_posTmp;
                                //std::cout << "coord nextVoxel :" << x1 << "," << y1 << "," << z1 << std::endl;
                                //std::cout << "pos nextVoxel :" << n_pos << std::endl;
                            }
                            /*if (ImTmp->at(n_pos) == 120 && ImTmp->at(pos)==120) {
                            ImTmp->at(n_pos) = 254;
                        }*/
                        }
                    }
                }
            }
        }
    }
    if(n_pos != -1){
        neighboorMap->at(pos) -= 1;
    }
    return n_pos;
}

std::vector<ExtendedNode*> getNeighboorhoodList(ExtendedNode* n,
                                                std::unordered_map<int,ExtendedNode*> nodesList,
                                                Image3D<short int>& skeletonIm3D){
    int nb_rows = ((int)skeletonIm3D.n_rows);
    int nb_cols = ((int)skeletonIm3D.n_cols);
    int nb_slices = ((int)skeletonIm3D.n_slices);

    int x = n->getX();
    int y = n->getY();
    int z = n->getZ();

    std::vector<ExtendedNode*> neighboorhood;

    for (int i = -1; i <= 1 ; ++i) {
        for (int j = -1; j <= 1 ; ++j) {
            for (int k = -1; k <= 1 ; ++k) {
                if(i!=0 || j!=0 || k!=0){
                    int x1 = x+i;
                    int y1 = y+j;
                    int z1 = z+k;
                    if((x1>=0 && y1>=0 && z1>=0) &&
                            (x1<nb_rows && y1<nb_cols && z1<nb_slices)){
                        int pos = x1+(y1*nb_rows)+(z1*nb_rows*nb_cols);

                        std::unordered_map<int,ExtendedNode*>::const_iterator n = nodesList.find(pos);

                        if ( n != nodesList.end() )
                            neighboorhood.push_back(n->second);
                    }
                }
            }
        }
    }
    return neighboorhood;
}


void SkeletonGraph::compute() {
    //setDataFromFile("/Users/andrearuffino/Downloads/cow-fs6-hd0.txt");
    //generate3DImFromData();
    initGraph();
    computeNeighboorMap(&skeletonIm3D);
    computeEnhanceMapFromNeighboorMap();
    std::unordered_map<int, ExtendedNode*> nodesList;
    for (auto it = voxels.begin(); (it!= voxels.end()); ++it){
        int pos = it->first;
        if((isIntersectionNode(pos) && (neighboorMap->at(pos)!=2)) || (neighboorMap->at(pos)==1)){
            skeletonImTmp.at(pos) = 120;
            nodesList.insert({pos,it->second});
        }else{
            skeletonImTmp.at(pos) = 255;
            nodes.erase(pos);
        }
        //std::cout << "nb_neighboor node :" << neighboorMap->at(pos) << std::endl;

    }
    //voxels.clear();
    /*for (auto it = nodesList.begin(); (it!= nodesList.end()); ++it){
        ExtendedNode *u = it->second;
        int x = u->getX();
        int y = u->getY();
        int z = u->getZ();
        std::vector<ExtendedNode*> neighboorList = getNeighboorhoodList(u,
                                                                        nodesList,
                                                                        skeletonIm3D);
        if(!neighboorList.empty()){
            int pos = it->first;
            int val = enhanceNeighboorMap.at(pos);
            if(skeletonImTmp.at(pos) != 255){
                for (int i = 0; i < neighboorList.size(); ++i) {
                    int posTmp = neighboorList[i]->getIndex(skeletonIm3D.n_cols,
                                                            skeletonIm3D.n_rows,
                                                            skeletonIm3D.n_slices);

                    if(val < enhanceNeighboorMap.at(posTmp)){
                        //voxels.erase(pos);
                        pos = posTmp;
                        u = neighboorList[i];
                        skeletonImTmp.at(pos)= 255;
                    }else{
                        //voxels.erase(posTmp);
                        skeletonImTmp.at(posTmp) = 255;
                    }

                }
            }
            //if(skeletonImTmp.at(pos) != 255 && skeletonImTmp.at(pos) != 150){
            voxels.insert({pos,u});
            //skeletonImTmp.at(pos) = 150;
            //}
        }
    }

    for (auto it = voxels.begin(); (it!= voxels.end()); ++it){
        int pos = it->first;
        ExtendedNode *u = it->second;
        int x = u->getX();
        int y = u->getY();
        int z = u->getZ();
        do{
            //std::cout << "coord node :" << x << "," << y << "," << z << std::endl;
            //std::cout << "pos node :" << pos << std::endl;
            //std::cout << "nb_neighboor node :" << neighboorMap->at(pos) << std::endl;
            if(!isVisited(pos)){
                addVisitedVoxel(pos);
            }
            pos = nextVoxelPosition(x,y,z,skeletonImTmp);
            auto nextVoxel = voxels.find(pos);
            if(nextVoxel != voxels.end()){
                ExtendedNode *v = nextVoxel->second;
                //skeletonImTmp.at(nextVoxel->first) = 150;
                if(!u->isAdjacentNode(nextVoxel->first)){
                    u->addAdjacentNode(nextVoxel->first);
                }
                if(!v->isAdjacentNode(it->first)){
                    v->addAdjacentNode(it->first);
                }
                pos = it->first;
                x = u->getX(); y=u->getY(); z=u->getZ();
                //std::cout << "coord1 node :" << x << "," << y << "," << z << std::endl;
                //std::cout << "pos1 node :" << pos << std::endl;
            }
            else if(pos != -1){
                std::vector<int> coord = getCoordOutOfIndex(skeletonImTmp.n_cols,
                                                            skeletonImTmp.n_rows,
                                                            skeletonImTmp.n_slices,pos);
                //skeletonImTmp.at(pos) = 254;
                x = coord[0]; y=coord[1]; z=coord[2];
                //std::cout << "coord node1 :" << x << "," << y << "," << z << std::endl;
                //std::cout << "pos node1 :" << pos << std::endl;
            }
        }while((neighboorMap->at(pos) > 0) && (pos != -1));
        std::vector<int> adjNodesList = u->getAdjacentNode();
        std::cout << "nodePos :" << it->first << std::endl;
        std::cout << "nbAdjnode :" << adjNodesList.size() << std::endl;
        if(adjNodesList.size()==2){
            skeletonImTmp.at(it->first)= 255;
            ExtendedNode *adjNode1 = voxels.at(adjNodesList[0]);
            ExtendedNode *adjNode2 = voxels.at(adjNodesList[1]);
            adjNode1->deleteAdjacentNode(it->first);
            adjNode2->deleteAdjacentNode(it->first);
        }
        visitedVoxel.clear();
    }*/

}

void SkeletonGraph::initGraph(){

    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;

    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_rows)+(z*nb_rows*nb_cols);
                if(skeletonImTmp.at(pos) == 255){
                    ExtendedNode *n = new ExtendedNode(graph,x,y,z);
                    nodes.insert({pos,n});
                }
                /*if(skeletonImTmp.at(pos) == 200) {
                    initNodeEdges(pos);
                    skeletonImTmp.at(pos) = 120;
                }
                if( skeletonImTmp.at(pos) == 255) {
                    ExtendedNode *d = new ExtendedNode(graph, x, y, z);
                    nodes.insert({pos ,d});
                    initNodeEdges(pos);
                    skeletonImTmp.at(pos) = 120;
                }*/
            }
        }
    }
    voxels = (const std::unordered_map<int,ExtendedNode*>) nodes;
}
void addEdge(ListGraph &graph, ListGraph::Node u, ListGraph::Node v,
             int pos_u, int pos_v){
    ExtendedEdge *e = new ExtendedEdge(graph,u,v,pos_u,pos_v);
    u->addAdjacentNode(graph.id(v));
    u->addAdjacentNodePos(pos_u);
    u->addIncidentEdges(e);
    v->addAdjacentNode(graph.id(u));
    v->addAdjacentNodePos(pos_v);
    v->addIncidentEdges(e);
}

void SkeletonGraph::initNodeEdges(int pos){
    ExtendedNode *u;
    if((u=nodes.find(pos))==nodes.end()){
        std::cout << "node doesn't exist" << std::endl;
        return false;
    }
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;

    int x = u->getX();
    int y = u->getY();
    int z = u->getZ();

    for (int i = -1; i <= 1 ; ++i) {
        for (int j = -1; j <= 1 ; ++j) {
            for (int k = -1; k <= 1 ; ++k) {
                if(i!=0 || j!=0 || k!=0){

                    int x1 = x+i;
                    int y1 = y+j;
                    int z1 = z+k;
                    int n_pos = x1+(y1*nb_rows)+(z1*nb_cols*nb_rows);

                    if(x1 >= 0   && y1 >= 0   && z1 >= 0   &&
                            x1 < nb_rows && y1 < nb_cols && z1 < nb_slices) {

                        if (skeletonImTmp.at(n_pos) == 200) {
                            u->addAdjacentNode(n_pos);
                        }
                        if (skeletonImTmp.at(n_pos) == 255 || skeletonImTmp.at(n_pos) == 200) {
                            ListGraph::Node U1 = u->getNode();
                            ListGraph::Node V1 = v->getNode();
                            ExtendedNode *v = new ExtendedNode(graph,x1,y1,z1);
                            addEdge(graph,U1,V1,pos,n_pos);
                            nodes.insert({n_pos,v});
                            skeletonImTmp.at(n_pos) = 200;
                        }
                    }
                }
            }
        }
    }
}

void SkeletonGraph::arcToSingleNeighboor(ExtendedNode *n0, int n_pos){
    int id_n2 = nodes.at(n_pos)->getId();
    ExtendedEdge e = ExtendedEdge(graph, n0->getId(), id_n2);
    //doesEdgeExist[e.getId()] = 1;
    //std::cout << "edge | " << e.getId() << std::endl;
}


bool SkeletonGraph::transformIntoArc(ExtendedNode *n){
    //bool res = false;
    std::vector<int> adNodes = n->getAdjacentNode();
    std::vector<ExtendedNode*> neighboors;

    neighboors = getNeighboorhood(n->getX(), n->getY(), n->getZ());

    for(int i=0; i< neighboors.size(); ++i){
        ExtendedNode *enode = neighboors.at(i);
        if(adNodes.size() != enode->getAdjacentNode().size()){
            std::cout << "transformIntoArc false 1" << std::endl;
            return false;
        }
    }

    for(int i=0; i< neighboors.size(); ++i){
        ExtendedNode *enode = neighboors.at(i);
        if(!eraseNodes(enode)){
            std::cout << "transformIntoArc false 2" << std::endl;
            return false;
        }
    }
    std::cout << "transformIntoArc true" << std::endl;
    return true;
}

bool SkeletonGraph::eraseNodes(ExtendedNode *n){
    std::vector<int> adNodes = n->getAdjacentNodesPos();
    //std::vector<ExtendedNode*> neighboors;
    int pos = n->getIndex(skeletonIm3D.n_cols,
                          skeletonIm3D.n_rows,
                          skeletonIm3D.n_slices);
    if(adNodes.empty() || nodes.find(pos)==nodes.end()){
        std::cout << "eraseNodes empty or node doesn't exist" << std::endl;
        return false;
    }
    //neighboors = getNeighboorhoodList(n,skeletonImTmp,nodes);

    for(int i=0; i< adNodes.size();i++){
        ExtendedNode *enode = nodes.at(adNodes[i]);
        enode->deleteAdjacentNode(n->getId());
        enode->deleteAdjacentNodePos(pos);
    }
    graph.erase(n->getNode());
    nodes.erase(pos);
    return true;
}

bool SkeletonGraph::contractNodes(ExtendedNode *n){

    std::vector<int> adNodes = n->getAdjacentNodesPos();
    if(adNodes.empty() || nodes.find(pos)==nodes.end()){
        std::cout << "eraseNodes empty or node doesn't exist" << std::endl;
        return false;
    }

    ExtendedNode *enode_u = nodes.at(adNodes[0]);
    ExtendedNode *enode_v = nodes.at(adNodes[1]);

    enode_u->addAdjacentNode(enode_v->getId());
    enode_v->addAdjacentNode(enode_u->getId());
    enode_u->addAdjacentNodePos(enode_v->getIndex(nb_cols,nb_rows,nb_slices));
    enode_v->addAdjacentNodePos(enode_u->getIndex(nb_cols,nb_rows,nb_slices));

    return eraseNodes(n);
}


bool SkeletonGraph::isIntersectionNode(int pos){
    int nb_rows = skeletonIm3D.n_rows;
    int nb_cols = skeletonIm3D.n_cols;
    int nb_slices = skeletonIm3D.n_slices;
    ExtendedNode *n = voxels.at(pos);
    int x = n->getX();
    int y = n->getY();
    int z = n->getZ();

    for (int i = -1; i <= 1 ; ++i) {
        for (int j = -1; j <= 1 ; ++j) {
            for (int k = -1; k <= 1 ; ++k) {
                if(i!=0 || j!=0 || k!=0){
                    int x1 = x+i;
                    int y1 = y+j;
                    int z1 = z+k;
                    if((x1>=0 && y1>=0 && z1>=0) &&
                            (x1<nb_rows && y1<nb_cols && z1<nb_slices)){
                        int pos_n = x1+(y1*nb_rows)+(z1*nb_rows*nb_cols);
                        if(skeletonIm3D.at(pos_n)==255){
                            if(enhanceNeighboorMap.at(pos)<enhanceNeighboorMap.at(pos_n)){
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

void GraphTest::updateNodeMap(){
    std::cout << "taille nodeMap : " << nodes.size() << std::endl;
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
    //std::unordered_map<int,ExtendedNode*> nodesList;
    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_rows)+(z*nb_rows*nb_cols);
                /*if(skeletonImTmp.at(pos) == 120) {
                    nodesList.insert({pos,nodes.at(pos)});
                }*/
                if(skeletonImTmp.at(pos) == 255) {
                    nodes.erase(pos);
                }
            }
        }
    }
    //nodes.realloc();
    /*nodes.clear();
    nodes = nodesList;*/
    std::cout << "taille nodeMap : " << nodes.size() << std::endl;
}

void GraphTest::updateNodeMap(std::unordered_map<int,ExtendedNode*> nodesList){
    std::cout << "taille nodeMap : " << nodes.size() << std::endl;
    nodes.clear();
    for(auto it = nodesList.begin(); it!=nodesList.end(); ++it){
        nodes.insert({it->first,it->second});
    }
    std::cout << "taille nodeMap : " << nodes.size() << std::endl;
}

Image* SkeletonGraph::getSkeleton3DIm(){
    //    const Image3D<short int> im = *skeletonIm3D;
    return new Image3D<short int>(skeletonImTmp);
}


Image* SkeletonGraph::getGraphImage3D(){
    //    const Image3D<short int> im = *skeletonIm3D;
    Image3D<short int> *im = new Image3D<short int>(skeletonIm3D);

    for (auto it = nodes.begin(); it!= nodes.end(); ++it){
        im->at(it->first) = 120;
    }

    return im;
}
