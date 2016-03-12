#include "skeletongraph.h"
#include <qtextstream.h>
#include <limits>
#include <string>

Image *neighboorMap;
std::unordered_map<int, int> enhanceNeighboorMap;
std::unordered_map<int, ExtendedNode*> voxels;

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
                        int val = ImTmp.at(n_posTmp);
                        if(val!=254 && val>n_pos){
                            n_pos = val;
                        }
                        /*if (ImTmp->at(n_pos) == 120 && ImTmp->at(pos)==120) {
                            ImTmp->at(n_pos) = 254;
                        }*/
                    }
                }
            }
        }
    }
    return n_pos;
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
        if((isInnerNode(pos) && (neighboorMap->at(pos)!=2)) || (neighboorMap->at(pos)==1)){
            skeletonImTmp.at(pos) = 120;
            nodesList.insert({pos,it->second});
        }
    }

    /*for (auto it = nodesList.begin(); (it!= nodesList.end()); ++it){
        int pos = it->first;
        ExtendedNode *u = it->second;
        int x = u->getX();
        int y = u->getY();
        int z = u->getZ();
        do{
            pos = nextVoxelPosition(x,y,z,skeletonImTmp);
            auto nextVoxel = nodesList.find(pos);
            if(nextVoxel != nodesList.end()){
                ExtendedNode *v = nextVoxel->second;
                u->addAdjacentNode(nextVoxel->first);
                v->addAdjacentNode(it->first);
                pos = it->first;
                x = u->getX(); y=u->getY(); z=u->getZ();
            }
            else if(pos != -1){
                std::vector<int> coord = getCoordOutOfIndex(skeletonImTmp.n_cols,
                                                            skeletonImTmp.n_rows,
                                                            skeletonImTmp.n_slices,pos);
                skeletonImTmp.at(pos) = 254;
                x = coord[0]; y=coord[1]; z=coord[2];
            }
        }while(pos != -1);
        std::vector<int> adjNodesList = u->getAdjacentNode();
        if(adjNodesList.size()==2){
            skeletonImTmp.at(it->first)= 254;
            ExtendedNode *adjNode1 = nodesList.at(adjNodesList[0]);
            ExtendedNode *adjNode2 = nodesList.at(adjNodesList[1]);
            adjNode1->deleteAdjacentNode(it->first);
            adjNode2->deleteAdjacentNode(it->first);
        }
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
                if(skeletonImTmp.at(pos) == 255) {
                    ExtendedNode *d = new ExtendedNode(graph, x, y, z);
                    voxels.insert({pos,d});
                    nodes.insert({pos ,d});
                }
            }
        }
    }
    
    /*for (auto it = nodes.begin(); it!= nodes.end(); ++it){
        ExtendedNode *n = it->second;
        arcToNeighboors(n);
        skeletonImTmp.at(it->first) = 200;
        //std::cout << "node | " << it->first << ":" << n.getId() << std::endl;
    }
    
    ListGraph::NodeMap<Point> coords(graph);
    ListGraph::NodeMap<double> sizes(graph);
    ListGraph::NodeMap<int> colors(graph);
    ListGraph::NodeMap<int> shapes(graph);
    ListGraph::ArcMap<int> acolors(graph);
    ListGraph::ArcMap<int> widths(graph);
    
    bool process = true;
    
    int nb = 0;
    while(process) {
        process = false;
        for (auto it = nodes.begin(); (it!= nodes.end() && !process); ++it){
            ExtendedNode *n = it->second;
            process = process || contractNodes(n);
            
            std::cout << "contract " << n->getId() << " : " << n->getAdjacentNode().size() << std::endl;
        }
        std::cout << "while(process) : " << nb++
                  << (process?"true":"false") << std::endl;
    }
*/
    /* process = true;
    while(process) {
        process = false;
        for (auto it = nodes.begin(); (it!= nodes.end() && !process); ++it){
            ExtendedNode *n = it->second;
            process = process || transformIntoArc(n);

            std::cout << "transform " << n->getId() << " : " << n->getAdjacentNode().size() << std::endl;
        }
        //std::cout << "while(process) : " << nb++
        //          << (process?"true":"false") << std::endl;
    }*/
    /* for (auto it = nodes.begin(); it!= nodes.end(); ++it){
        ExtendedNode *n = it->second;
        int x = n->getX();
        int y = n->getY();
        int z = n->getZ();
        
        ListGraph::Node gn = graph.nodeFromId(n->getId());
        
        coords[gn] = Point(x+z,y+z);
        sizes[gn]  = .1;
        colors[gn] = 1;
        shapes[gn] = 0;
    }
    
    
    graphToEps(graph,"graph.eps").scale(10).coords(coords)
            .nodeScale(.1).nodeSizes(sizes)
            .arcWidthScale(.005).run();*/
}

void SkeletonGraph::arcToNeighboors(ExtendedNode *n0){
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
    //    std::vector<VertexGraph *> lnodes;
    
    int x = n0->getX();
    int y = n0->getY();
    int z = n0->getZ();
    
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
                        
                        if (skeletonImTmp.at(n_pos) == 255) {
                            arcToSingleNeighboor(n0, n_pos);
                            n0->addAdjacentNode(nodes.at(n_pos)->getId());
                        }
                        
                        if (skeletonImTmp.at(n_pos) == 200) {
                            n0->addAdjacentNode(nodes.at(n_pos)->getId());
                        }
                    }
                }
            }
        }
    }
    //return lnodes;
}

void SkeletonGraph::arcToSingleNeighboor(ExtendedNode *n0, int n_pos){
    int id_n2 = nodes.at(n_pos)->getId();
    ExtendedEdge e = ExtendedEdge(graph, n0->getId(), id_n2);
    //doesEdgeExist[e.getId()] = 1;
    //std::cout << "edge | " << e.getId() << std::endl;
}

bool SkeletonGraph::spotHighDivergentNodes(ExtendedNode *n){
    return false;
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
    std::vector<int> adNodes = n->getAdjacentNode();
    std::vector<ExtendedNode*> neighboors;

    if(adNodes.empty()){
        std::cout << "eraseNodes empty" << std::endl;
        return false;
    }
    if(adNodes.size()>=2){
        neighboors = getNeighboorhood(n->getX(), n->getY(), n->getZ());

        ListGraph::Node u = graph.nodeFromId(adNodes.at(0));
        ExtendedNode *enode_u = neighboors[0];
        enode_u->deleteAdjacentNode(n->getId());

        for(int i=1; i< adNodes.size();i++){
            ListGraph::Node v = graph.nodeFromId(adNodes.at(i));
            ExtendedEdge e = ExtendedEdge(graph, graph.id(u), graph.id(v));


            ExtendedNode *enode_v = neighboors[i];

            enode_u->addAdjacentNode(enode_v->getId());
            enode_v->addAdjacentNode(enode_u->getId());

            enode_v->deleteAdjacentNode(n->getId());
        }

        std::cout << "eraseNodes size >=2" << std::endl;

    }
    std::cout << "eraseNodes graph erase" << std::endl;
    graph.erase(graph.nodeFromId(n->getId()));
    std::cout << "eraseNodes nodes erase" << std::endl;
    nodes.erase(n->getIndex(skeletonIm3D.n_cols, skeletonIm3D.n_rows, skeletonIm3D.n_slices));
    std::cout << "eraseNodes true" << std::endl;
    return true;
}

bool SkeletonGraph::contractNodes(ExtendedNode *n){
    bool res = false;
    //supprimer dans graph g
    //suppirmer arc du graph
    //hashmap
    //mettre a jour adjacentNodes dans extendedNode.
    //connecter les deux voisins.
    std::vector<int> adNodes = n->getAdjacentNode();
    std::vector<ExtendedNode*> neighboors;
    
    if (adNodes.size()==2) {
        res = true;
        ListGraph::Node u = graph.nodeFromId(adNodes.at(0));
        ListGraph::Node v = graph.nodeFromId(adNodes.at(1));
        ExtendedEdge e = ExtendedEdge(graph, graph.id(u), graph.id(v));
        
        neighboors = getNeighboorhood(n->getX(), n->getY(), n->getZ());
        
        if(neighboors.size() == 2){
            ExtendedNode *enode_u = neighboors[0];
            ExtendedNode *enode_v = neighboors[1];
            
            enode_u->addAdjacentNode(enode_v->getId());
            enode_v->addAdjacentNode(enode_u->getId());
            
            enode_u->deleteAdjacentNode(n->getId());
            enode_v->deleteAdjacentNode(n->getId());
        }
        graph.erase(graph.nodeFromId(n->getId()));
        nodes.erase(n->getIndex(skeletonIm3D.n_cols, skeletonIm3D.n_rows, skeletonIm3D.n_slices));
        //getextendednodefromneighboor
        //addadjacent u -> v
        //addadjacent v -> u
    }
    return res;
}

std::vector<ExtendedNode*> SkeletonGraph::getNeighboorhood(int x,
                                                           int y,
                                                           int z){
    int nb_rows = ((int)skeletonIm3D.n_rows);
    int nb_cols = ((int)skeletonIm3D.n_cols);
    int nb_slices = ((int)skeletonIm3D.n_slices);
    
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
                        
                        std::unordered_map<int,ExtendedNode*>::const_iterator n = nodes.find(pos);
                        
                        if ( n != nodes.end() )
                            neighboorhood.push_back(n->second);
                    }
                }
            }
        }
    }
    return neighboorhood;
}

bool SkeletonGraph::isInnerNode(int pos){
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

bool SkeletonGraph::isNode(int x, int y, int z){
    //    int nb_rows = skeletonIm3D.n_rows;
    //    int nb_cols = skeletonIm3D.n_cols;
    //    int nb_slices = skeletonIm3D.n_slices;
    int nb_neighboor = 0;
    //
    //    for (int i = -1; i <= 1 ; ++i) {
    //        for (int j = -1; j <= 1 ; ++j) {
    //            for (int k = -1; k <= 1 ; ++k) {
    //                if(i!=0 || j!=0 || k!=0){
    //                    int x1 = x+i;
    //                    int y1 = y+j;
    //                    int z1 = z+k;
    //                    if((x1>=0 && y1>=0 && z1>=0) &&
    //                       (x1<nb_cols && y1<nb_rows && z1<nb_slices)){
    //                        int pos = x1+(y1*nb_cols)+(z1*nb_rows*nb_cols);
    //                        if(!isNodeTab[pos] && skeletonIm3D.at(x1+(y1*nb_cols)+(z1*nb_rows*nb_cols)) == 255){
    //                            nb_neighboor++;
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //    std::cout <<"at: " << x << " "<< y << " " << z << " "
    //    << "neighboor : " << nb_neighboor << " ("
    //    << skeletonIm3D.at(x+(y*nb_cols)+(z*nb_rows*nb_cols)) << ")" << std::endl;
    //
    return ((nb_neighboor >= 3)||(nb_neighboor == 1));
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
