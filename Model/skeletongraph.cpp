#include "skeletongraph.h"
#include <qtextstream.h>
#include <limits>
#include <string>

SkeletonGraph::SkeletonGraph() {

}

SkeletonGraph::SkeletonGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(skeletonIm3D);
}

SkeletonGraph::~SkeletonGraph() {
}

void SkeletonGraph::setGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(im_const);
}

void SkeletonGraph::addVisitedVoxel(int pos){
    if(visitedVoxel.size()==10){
        visitedVoxel.pop_back();
    }
    visitedVoxel.insert(visitedVoxel.begin(),pos);
}

bool SkeletonGraph::isVisited(int pos){
    for(unsigned int i = 0; i < visitedVoxel.size(); i++)
        if(visitedVoxel[i] == pos)
            return true;
    return false;
}

void SkeletonGraph::computeEnhanceMapFromNeighboorMap(){
    int nb_rows = neighboorMap->n_rows;
    int nb_cols = neighboorMap->n_cols;
    int nb_slices = neighboorMap->n_slices;
    enhanceNeighboorMap.clear();
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

void SkeletonGraph::computeNeighboorMap(){
    neighboorMap = new Image3D<short int>(skeletonIm3D);
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

                            if (skeletonIm3D.at(n_pos) == 255) {
                                neighboorMap->at(pos) += 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

int SkeletonGraph::nextVoxelPosition(int x, int y, int z){
    int nb_cols = neighboorMap->n_cols;
    int nb_rows = neighboorMap->n_rows;
    int nb_slices = neighboorMap->n_slices;
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
                        if(!isVisited(n_posTmp)){
                            int val = neighboorMap->at(n_posTmp);
                            if(val>val_n){
                                val_n = val;
                                n_pos = n_posTmp;
                            }
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

std::vector<ExtendedNode*> SkeletonGraph::getNeighboorhoodList(ExtendedNode* n){
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

bool isSpatialNeighboor(ExtendedNode *u,ExtendedNode *n1){
    int n1x = n1->getX(); int n1y = n1->getY(); int n1z= n1->getZ();
    int dist_ux_n1x = ((n1x - u->getX())*(n1x - u->getX()));
    int dist_uy_n1y = ((n1y - u->getY())*(n1y - u->getY()));
    int dist_uz_n1z = ((n1z - u->getZ())*(n1z - u->getZ()));
    if(dist_ux_n1x < 2 && dist_uy_n1y <2 && dist_uz_n1z < 2){
        if((dist_ux_n1x != 0 || dist_uy_n1y !=0 || dist_uz_n1z !=0)){
            return true;
        }
    }
    return false;
}

int SkeletonGraph::getNodeWeight(ExtendedNode *u){
    int weight =0;
    std::vector<int> adnodes = u->getAdjacentNodesPos();
    for (int i = 0; i < adnodes.size() ; ++i) {
        ExtendedNode *n = nodes.at(adnodes.at(i));
        weight += n->getAdjacentNodesPos().size();
    }
    return weight;
}

void SkeletonGraph::processNodeWith2neighboor(ExtendedNode *u){
    std::vector<int> adnodes = u->getAdjacentNodesPos();
    int pos =u->getIndex(skeletonImTmp.n_cols,skeletonImTmp.n_rows,
                         skeletonImTmp.n_slices);
    if(nodes.find(adnodes.at(0))!=nodes.end() && nodes.find(adnodes.at(1))!=nodes.end()){
        ExtendedNode *n1 = nodes.at(adnodes[0]);
        ExtendedNode *n2 = nodes.at(adnodes[1]);
        if(isSpatialNeighboor(n1,n2)){
            bool mergeSucceeded = mergeNodes(n1,n2);
            if(mergeSucceeded){
                skeletonImTmp.at(adnodes[1]) = 255;
            }
        }
        else if(isSpatialNeighboor(u,n1) && isSpatialNeighboor(u,n2) &&
                (n1->getAdjacentNodesPos().size()>1 && n2->getAdjacentNodesPos().size()>1) &&
                (!isIntersectionNode(adnodes[0]) && !isIntersectionNode(adnodes[1]))){
            if(mergeNodes(u,n1) && mergeNodes(u,n2)){
                skeletonImTmp.at(adnodes[0]) = 255;
                skeletonImTmp.at(adnodes[1]) = 255;
            }
        }else{
            if(contractNodes(u)){
                skeletonImTmp.at(pos) = 255;
            }
        }

    }
}

void SkeletonGraph::processIntersectionNode(int pos){
    ExtendedNode *u = nodes.at(pos);
    std::cout << "ici" << std::endl;
    std::vector<int> adnodes = u->getAdjacentNodesPos();
    if(adnodes.size() > 2){
        for(int i = 0; i<adnodes.size(); ++i){
            if(nodes.find(adnodes.at(i))!=nodes.end()){
                ExtendedNode *v = nodes.at(adnodes.at(i));
                std::cout << "ici1" << std::endl;
                if(!isIntersectionNode(adnodes.at(i))){
                    std::cout << "ici2" << std::endl;
                    if(v->getAdjacentNodesPos().size()>1){
                        if(mergeNodes(u,v)){
                            skeletonImTmp.at(adnodes.at(i)) = 255;
                        }
                    }else if(isSpatialNeighboor(u,v)){
                        if(mergeNodes(u,v)){
                            skeletonImTmp.at(adnodes.at(i)) = 255;
                        }
                    }
                }
                else{
                    if(isIntersectionNode(pos) && isSpatialNeighboor(u,v)){
                        if(mergeNodes(u,v)){
                            skeletonImTmp.at(adnodes.at(i)) = 255;
                            //return;
                        }
                    }
                }
            }
        }
    }
    std::cout << "ici3" << std::endl;
}

void SkeletonGraph::compute() {
    initGraph();
    computeNeighboorMap();
    computeEnhanceMapFromNeighboorMap();
    std::cout << "noeuds :" << nodes.size() << std::endl;
    int size_graph;
    do{
        size_graph = nodes.size();
        for (auto it = voxels.begin(); (it!= voxels.end()); ++it){
            int pos = it->first;
            ExtendedNode *u = it->second;

            if(nodes.find(pos)!=nodes.end() && (hasBiggerWeiht(pos))){
                processIntersectionNode(pos);
            }
            if(nodes.find(pos)!=nodes.end() && u->getAdjacentNodesPos().size()==2){
                processNodeWith2neighboor(u);
            }

        }
        voxels.clear();
        for (auto it = nodes.begin(); (it!= nodes.end()); ++it){
            voxels.insert({it->first,it->second});
        }
    }while(size_graph != nodes.size());
    std::cout << "noeuds :" << nodes.size() << std::endl;

    delete neighboorMap;
}

void SkeletonGraph::initGraph(){

    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
    nodes.clear();
    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_rows)+(z*nb_rows*nb_cols);
                if(skeletonImTmp.at(pos) == 200) {
                    initNodeEdges(pos);
                    skeletonImTmp.at(pos) = 120;
                }
                if( skeletonImTmp.at(pos) == 255) {
                    ExtendedNode *d = new ExtendedNode(graph, x, y, z);
                    nodes.insert({pos ,d});
                    initNodeEdges(pos);
                    skeletonImTmp.at(pos) = 120;
                }
            }
        }
    }
    voxels.clear();
    for (auto it = nodes.begin(); (it!= nodes.end()); ++it){
        voxels.insert({it->first,it->second});
    }

}
ExtendedEdge* addEdge(ListGraph &graph, ExtendedNode* u, ExtendedNode* v,
                      int pos_u, int pos_v){
    ListGraph::Node u1, v1;
    u1 = u->getNode();
    v1 = v->getNode();
    ExtendedEdge *e = new ExtendedEdge(graph,u1,v1,pos_u,pos_v);
    int idu1 = graph.id(u1);
    int idv1 = graph.id(v1);

    u->addAdjacentNode(idv1);
    u->addAdjacentNodePos(pos_v);
    u->addIncidentEdges(e);
    v->addAdjacentNode(idu1);
    v->addAdjacentNodePos(pos_u);
    v->addIncidentEdges(e);
    return e;
}

void SkeletonGraph::initNodeEdges(int pos){
    ExtendedNode *u;
    if((nodes.find(pos))==nodes.end()){
        std::cout << "node doesn't exist" << std::endl;
        return;
    }
    u = nodes.at(pos);
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
                            ExtendedNode *n = nodes.at(n_pos);
                            if(!(u->isAdjacentNode(n->getId()))){
                                addEdge(graph,u,n,pos,n_pos);
                            }
                        }
                        if (skeletonImTmp.at(n_pos) == 255) {
                            ExtendedNode *v = new ExtendedNode(graph,x1,y1,z1);
                            if(!(u->isAdjacentNode(v->getId()))){
                                addEdge(graph,u,v,pos,n_pos);
                            }
                            nodes.insert({n_pos,v});
                            skeletonImTmp.at(n_pos) = 200;
                        }
                    }
                }
            }
        }
    }
}

bool SkeletonGraph::eraseNodes(ExtendedNode *n){
    std::vector<int> adNodes = n->getAdjacentNodesPos();
    int pos = n->getIndex(skeletonIm3D.n_cols,
                          skeletonIm3D.n_rows,
                          skeletonIm3D.n_slices);
    if(nodes.find(pos)==nodes.end() || adNodes.empty()){
        std::cout << "fonction erase: adNodes empty or node doesn't exist" << std::endl;
        return false;
    }

    for(int i=0; i< adNodes.size();i++){
        if(nodes.find(adNodes[i])!=nodes.end()){
            ExtendedNode *enode = nodes.at(adNodes[i]);
            enode->deleteAdjacentNode(n->getId());
            enode->deleteAdjacentNodePos(pos);
            //enode->deleteIncidentEdge(i);
        }
    }
    graph.erase(n->getNode());
    nodes.erase(pos);
    return true;
}

bool SkeletonGraph::mergeNodes(ExtendedNode *n1,ExtendedNode *n2){

    int nb_rows = skeletonIm3D.n_rows;
    int nb_cols = skeletonIm3D.n_cols;
    int nb_slices = skeletonIm3D.n_slices;
    int pos_n1 = n1->getIndex(nb_cols, nb_rows, nb_slices);
    int pos_n2 = n2->getIndex(nb_cols, nb_rows, nb_slices);
    std::vector<int> adNodes = n2->getAdjacentNodesPos();
    if(nodes.find(pos_n1)==nodes.end() || nodes.find(pos_n2)==nodes.end()
            || adNodes.empty()){
        std::cout << "fonction merge: adNodes empty or node doesn't exist" << std::endl;
        return false;
    }
    if(!eraseNodes(n2)){return false;}
    for(int i = 0; i < adNodes.size(); ++i){
        if(nodes.find(adNodes[i])!=nodes.end()){
            ExtendedNode *enode_u = nodes.at(adNodes[i]);
            int id_node_u = enode_u->getId();
            if(!n1->isAdjacentNode(id_node_u) && n1->getId()!=id_node_u){
                ExtendedEdge *e = addEdge(graph,n1,enode_u,pos_n1,adNodes[i]);
                //e->setSize(e->ge);
            }
        }
    }

    return true;
}

bool SkeletonGraph::contractNodes(ExtendedNode *n){

    int nb_rows = skeletonIm3D.n_rows;
    int nb_cols = skeletonIm3D.n_cols;
    int nb_slices = skeletonIm3D.n_slices;

    std::vector<int> adNodes = n->getAdjacentNodesPos();
    if(nodes.find(n->getIndex(nb_cols, nb_rows, nb_slices))==nodes.end() || adNodes.empty()){
        std::cout << "fonction contract: adNodes empty or node doesn't exist" << std::endl;
        return false;
    }
    if(nodes.find(adNodes[0])!=nodes.end() && nodes.find(adNodes[1])!=nodes.end()){
        ExtendedNode *enode_u = nodes.at(adNodes[0]);
        ExtendedNode *enode_v = nodes.at(adNodes[1]);
        if(!enode_u->isAdjacentNode(enode_v->getId())){
            addEdge(graph,enode_u,enode_v,adNodes[0],adNodes[1]);
        }
        return eraseNodes(n);
    }else{
        return false;
    }
}


bool SkeletonGraph::isIntersectionNode(int pos){
    int nb_rows = skeletonIm3D.n_rows;
    int nb_cols = skeletonIm3D.n_cols;
    int nb_slices = skeletonIm3D.n_slices;
    ExtendedNode *n = nodes.at(pos);
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

bool SkeletonGraph::hasBiggerWeiht(int pos){
    ExtendedNode *n = nodes.at(pos);
    std::vector<int> adNodes = n->getAdjacentNodesPos();
    for(int i =0; i<adNodes.size();++i){
        ExtendedNode *n1 = nodes.at(adNodes[i]);
        if(getNodeWeight(n) < getNodeWeight(n1)){
            return false;
        }
    }
    return true;
}
void SkeletonGraph::updateNodeMap(){
    std::cout << "taille nodeMap : " << nodes.size() << std::endl;
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_rows)+(z*nb_rows*nb_cols);
                if(skeletonImTmp.at(pos) == 255) {
                    nodes.erase(pos);
                }
            }
        }
    }
    std::cout << "taille nodeMap : " << nodes.size() << std::endl;
}

void SkeletonGraph::updateNodeMap(std::unordered_map<int,ExtendedNode*> nodesList){
    nodes.clear();
    for(auto it = nodesList.begin(); it!=nodesList.end(); ++it){
        nodes.insert({it->first,it->second});
    }
}

Image* SkeletonGraph::getSkeleton3DIm(){
    return new Image3D<short int>(skeletonImTmp);
}


Image* SkeletonGraph::getGraphImage3D(){
    Image3D<short int> *im = new Image3D<short int>(skeletonIm3D);

    for (auto it = nodes.begin(); it!= nodes.end(); ++it){
        ExtendedNode *u = it->second;
        std::vector<int> adnodes = u->getAdjacentNodes();
        //        std::cout << "node( " << u->getX() << "," << u->getY() << "," << u->getZ() << ") "<< u->getId() << " :";
        //        for(int i= 0; i<adnodes.size(); ++i){
        //            std::cout << " " << adnodes.at(i);
        //        }
        //        std::cout << " " << std::endl;
        im->at(it->first) = 120;
    }

    return im;
}

void SkeletonGraph::exportGraph(std::string name){
    
    Palette palette;
    Palette paletteW(true);
    
    ListGraph::NodeMap<dim2::Point<int> > coords(graph);
    ListGraph::NodeMap<double> sizes(graph);
    ListGraph::NodeMap<int> colors(graph);
    ListGraph::NodeMap<int> shapes(graph);
    ListGraph::ArcMap<int> acolors(graph);
    ListGraph::ArcMap<int> widths(graph);
    
    IdMap<ListGraph,ListGraph::Node> id(graph);
    
    int nbz = skeletonIm3D.n_slices;
    int nbx = 0;
    //int nby = 0;
    for(auto nit = nodes.begin(); nit!=nodes.end(); ++nit){
        ListGraph::Node n = graph.nodeFromId(nit->second->getId());
        
        int x = nit->second->getX();
        int y = nit->second->getY();
        int z = nit->second->getZ();
        
        std::cout << "exportGraph"<< nbx++ << " : "
                  << nit->second->getId() << " "
                  << nit->second->getX() << " "
                  << nit->second->getY() << " "
                  << nit->second->getZ() << std::endl;
        
        coords[n] = Point(x*10+(nbz*z),y*10+(nbz*z));
        sizes[n]  = 20;
        colors[n] = 0;
        shapes[n] = 0;
    }

    for (ListGraph::ArcIt ait(graph); ait != INVALID; ++ait){
        acolors[ait] = 0;
        widths[ait]  = 1;
    }
    
    std::cout << "Create " << name << std::endl;
    graphToEps(graph,name.data()).
            coords(coords).
            title("Sample .eps figure").
            copyright("(C) 2003-2009 LEMON Project").
            absoluteNodeSizes().absoluteArcWidths().
            nodeScale(2).nodeSizes(sizes).
            nodeShapes(shapes).
            nodeColors(composeMap(paletteW,colors)).
            arcColors(composeMap(palette,acolors)).
            arcWidthScale(.4).arcWidths(widths).
            nodeTexts(id).nodeTextSize(3*5).
            run();
}
