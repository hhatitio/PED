#include "skeletongraph.h"
#include <qtextstream.h>
#include <limits>
#include <string>

SkeletonGraph::SkeletonGraph() {
        neighboorMap = 0;
}

SkeletonGraph::SkeletonGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(skeletonIm3D);
    neighboorMap = new Image3D<short int>(skeletonIm3D);
}

SkeletonGraph::~SkeletonGraph() {
    if(!nodes.empty()){
       nodes.clear();
    }
    if(!edges.empty()){
        edges.clear();
    }
    if(!nodesTmp.empty()){
        nodesTmp.clear();
    }
    if(neighboorMap!=0){
        delete neighboorMap;
    }
}

void SkeletonGraph::setSkeletonIm(Image* im) {
    const Image3D<short int> im_const = *im;
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(im_const);
}

/**
 * @brief SkeletonGraph::computeEnhanceMapFromNeighboorMap
 * initialise the map enhanceNeighboorMap by inserting for each node
 * in voxels the couple {key,value}
 * key: the index of the nodes in the skeleton image
 * value: the sum of the current node neighboor's values in the image neihgboorMap
 */
void SkeletonGraph::computeEnhanceMapFromNeighboorMap(){
    int nb_rows = neighboorMap->n_rows;
    int nb_cols = neighboorMap->n_cols;
    int nb_slices = neighboorMap->n_slices;
    enhanceNeighboorMap.clear();
    for (auto it = nodesTmp.begin(); it!= nodesTmp.end(); ++it){
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

/**
 * @brief SkeletonGraph::computeNeighboorMap
 * for each pixel in skeletonIm3D with a value of 255
 * the number of neighboor pixels with the same value are
 * store at the current pixel index in neighboorMap
 */
void SkeletonGraph::computeNeighboorMap(){

    int nb_cols = neighboorMap->n_cols;
    int nb_rows = neighboorMap->n_rows;
    int nb_slices = neighboorMap->n_slices;
    for (auto it = nodesTmp.begin(); it!= nodesTmp.end(); ++it){
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


/**
 * @brief isSpatialNeighboor
 * @param u is a ExtendedNode
 * @param n1 is a ExtendedNode
 * @return true if the difference between u and n1 coordonates
 *         is inferior than 2;
 *         false otherwise
 */
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

/**
 * @brief SkeletonGraph::getNodeWeight
 * @param u is a ExtendedNode
 * @return the sum of the number of adjacent node
 *         of u adjacent nodes
 */
int SkeletonGraph::getNodeWeight(ExtendedNode *u){
    int weight =0;
    std::vector<int> adnodes = u->getAdjacentNodesPos();
    for (int i = 0; i < adnodes.size() ; ++i) {
        ExtendedNode *n = nodes.at(adnodes.at(i));
        weight += n->getAdjacentNodesPos().size();
    }
    return weight;
}

/**
 * @brief SkeletonGraph::processNodeWith2neighboor
 * @param u is a ExtendedNode
 * if u adjacent node are direct neighboor, one of them is erease
 * if u is between his adjacent nodes and they are his direct neighboors
 * then they are merge to u
 * otherwise u is erease by the fonction contratNodes
 */
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


/**
 * @brief SkeletonGraph::processIntersectionNode
 * @param pos the positon of the current node
 * this function merge a node's adjacent nodes with himself
 * if they have more than one neighboor,
 *    they are not considered as intersection node
 * or they are direct neighboor of the current node
 */
void SkeletonGraph::processIntersectionNode(int pos){
    ExtendedNode *u = nodes.at(pos);
    std::vector<int> adnodes = u->getAdjacentNodesPos();
    if(adnodes.size() > 2){
        for(int i = 0; i<adnodes.size(); ++i){
            if(nodes.find(adnodes.at(i))!=nodes.end()){
                ExtendedNode *v = nodes.at(adnodes.at(i));
                if(!isIntersectionNode(adnodes.at(i))){
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
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief SkeletonGraph::compute
 * generate the graph from skeletonIm3D.
 * start by initialising the nodes and the edges with initGraph
 * then compute the neighboorMap and the enhanceNeighboorMap
 * finally try to decrease the number of nodes and edges by using
 * processIntersectionNode() and processNodeWith2neighboor() to delete
 * in specific configuration
 */
void SkeletonGraph::compute() {
    initGraph();
    computeNeighboorMap();
    computeEnhanceMapFromNeighboorMap();
    std::cout << "nb_noeuds_before :" << nodes.size() << std::endl;
    int size_graph;
    do{
        size_graph = nodes.size();
        for (auto it = nodesTmp.begin(); (it!= nodesTmp.end()); ++it){
            int pos = it->first;
            ExtendedNode *u = it->second;

            if(nodes.find(pos)!=nodes.end() && (hasBiggerWeiht(pos))){
                processIntersectionNode(pos);
            }
            if(nodes.find(pos)!=nodes.end() && u->getAdjacentNodesPos().size()==2){
                processNodeWith2neighboor(u);
            }

        }
        nodesTmp.clear();
        for (auto it = nodes.begin(); (it!= nodes.end()); ++it){
            nodesTmp.insert({it->first,it->second});
        }
    }while(size_graph != nodes.size());
    std::cout << "nb_noeuds_after :" << nodes.size() << std::endl;
    computeAngles();
    //delete neighboorMap;
}

/**
 * @brief SkeletonGraph::initGraph
 * initialise the graph such as one node created in the graph
 * equals one pixels with the value 255 in the skeleton Image.
 * each node is link by an edge to his direct neighboors in the image.
 */
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
    nodesTmp.clear();
    for (auto it = nodes.begin(); (it!= nodes.end()); ++it){
        nodesTmp.insert({it->first,it->second});
    }

}

ExtendedEdge* SkeletonGraph::addEdge(ListGraph &graph, ExtendedNode* u, ExtendedNode* v,
                                     int pos_u, int pos_v){
    ListGraph::Node u1, v1;
    u1 = u->getNode();
    v1 = v->getNode();
    ExtendedEdge *e = new ExtendedEdge(graph,u1,v1,pos_u,pos_v);
    edges.insert({e->getId(),e});
    int idu1 = graph.id(u1);
    int idv1 = graph.id(v1);

    u->addAdjacentNode(idv1);
    u->addAdjacentNodePos(pos_v);
    u->addIncidentEdge(e);
    v->addAdjacentNode(idu1);
    v->addAdjacentNodePos(pos_u);
    v->addIncidentEdge(e);
    return e;
}

/**
 * @brief SkeletonGraph::initNodeEdges
 * @param pos is the position of the current node
 * create an edge between the node at the position pos and his
 * direct neighboors in the skeleton image
 */
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
            ExtendedEdge *edge = enode->getIncidentEdge(n);
            if(edges.find(edge->getId())!=edges.end()){
                edges.erase(edge->getId());
            }
            enode->deleteIncidentEdge(edge);
            enode->deleteAdjacentNode(n->getId());
            enode->deleteAdjacentNodePos(pos);
        }
    }
    graph.erase(n->getNode());
    nodes.erase(pos);
    return true;
}

/**
 * @brief SkeletonGraph::mergeNodes
 * @param n1
 * @param n2
 * try to merge n2 with n1.
 * link all n2 adjacent nodes to n1 and erase n2
 * @return true if succeeded. false otherwise
 */
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

    ExtendedEdge *edge_n1n2 = n1->getIncidentEdge(n2);
    for(int i = 0; i < adNodes.size(); ++i){
        if(nodes.find(adNodes[i])!=nodes.end()){
            ExtendedNode *enode_u = nodes.at(adNodes[i]);
            int id_node_u = enode_u->getId();
            if(!n1->isAdjacentNode(id_node_u) && n1->getId()!=id_node_u){
                ExtendedEdge *edge_n2N = n2->getIncidentEdge(enode_u);
                ExtendedEdge *e1 = addEdge(graph,n1,enode_u,pos_n1,adNodes[i]);
                e1->setSize(edge_n1n2->getSize()+edge_n2N->getSize()+1);
            }
        }
    }
    return eraseNodes(n2);
}

/**
 * @brief SkeletonGraph::contractNodes
 * @param n is a ExtendedNode with only 2 neighboor
 * erase n and create an edge between the adjacent nodes of n
 * @return true if succeeded. false otherwise
 */
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
        ExtendedEdge *edge_u = n->getIncidentEdge(enode_u);
        ExtendedEdge *edge_v = n->getIncidentEdge(enode_v);
        if(!enode_u->isAdjacentNode(enode_v->getId())){
            ExtendedEdge *e = addEdge(graph,enode_u,enode_v,adNodes[0],adNodes[1]);
            e->setSize(edge_u->getSize()+edge_v->getSize()+1);
        }
        return eraseNodes(n);
    }else{
        return false;
    }
}


/**
 * @brief SkeletonGraph::isIntersectionNode
 * @param pos is the position of the current node
 * @return true if the node's value in enhanceNeighboorMap is bigger than
 *         the value of his neighboorhood
 *         false otherwise
 */
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


Image* SkeletonGraph::getSkeleton3DIm(){
    return new Image3D<short int>(skeletonImTmp);
}


Image* SkeletonGraph::getGraphImage3D(){
    Image3D<short int> *im = new Image3D<short int>(skeletonIm3D);

    for (auto it = nodes.begin(); it!= nodes.end(); ++it){
        ExtendedNode *u = it->second;
        std::vector<ExtendedEdge *> incidentEdge = u->getIncidentEdges();
        im->at(it->first) = 120;
    }

    return im;
}

/**
 * @brief SkeletonGraph::computeAngles
 * for each node in the graph, compute the angle
 * between two consecutive edges around the node
 */
void SkeletonGraph::computeAngles(){
    for (auto it = edges.begin(); it!= edges.end(); ++it){
        ExtendedEdge *edge = it->second;
        std::vector<int> adjacentNodesPos = edge->getAdjacentNodesPos();
        for(int i = 0; i<adjacentNodesPos.size(); i++){
            ExtendedNode *n1 = nodes.at(adjacentNodesPos.at(i));
            std::vector<ExtendedEdge*> n1_incidentEdges = n1->getIncidentEdges();
            for(int j = 0; j<n1_incidentEdges.size(); j++){
                ExtendedEdge *edge2 = n1_incidentEdges.at(j);
                if(edge2->getId() != edge->getId() && !edge->containAngle(edge2->getId())){
                    ExtendedNode *n2 = nodes.at(edge2->getOppositeNodePos(n1->getId()));
                    ExtendedNode *n3 = nodes.at(edge->getOppositeNodePos(n1->getId()));
                    std::vector<int> vec_n1n2,vec_n1n3;
                    vec_n1n2.push_back(n2->getX()-n1->getX());
                    vec_n1n2.push_back(n2->getY()-n1->getY());
                    vec_n1n2.push_back(n2->getZ()-n1->getZ());

                    vec_n1n3.push_back(n3->getX()-n1->getX());
                    vec_n1n3.push_back(n3->getY()-n1->getY());
                    vec_n1n3.push_back(n3->getZ()-n1->getZ());

                    int produiScalaire = vec_n1n2.at(0)*vec_n1n3.at(0)+
                            vec_n1n2.at(1)*vec_n1n3.at(1)+
                            vec_n1n2.at(2)*vec_n1n3.at(2);
                    double norm_n1n2 = sqrt(vec_n1n2.at(0)*vec_n1n2.at(0)+
                                            vec_n1n2.at(1)*vec_n1n2.at(1)+
                                            vec_n1n2.at(2)*vec_n1n2.at(2));
                    double norm_n1n3 = sqrt(vec_n1n3.at(0)*vec_n1n3.at(0)+
                                            vec_n1n3.at(1)*vec_n1n3.at(1)+
                                            vec_n1n3.at(2)*vec_n1n3.at(2));
                    double val = (double)produiScalaire/(norm_n1n2*norm_n1n3);
                    double angle = acos(val)*180/PI;
                    edge->addAngle(edge2->getId(),angle);
                    edge2->addAngle(edge->getId(),angle);
//                   std::cout << "angle edges (" << edge->getId() << ","
//                              << edge2->getId() << ") :" << angle << std::endl;
                }
            }

        }

    }
}

void SkeletonGraph::showfeatures(){
    std::cout << std::endl;
    std::cout << "nb noeuds graph :" << nodes.size() << std::endl;
    std::cout << "nb edges graph :" << edges.size() << std::endl;
    std::cout << "=====================Angle edges===================" << std::endl;

    for (auto it = edges.begin(); it!= edges.end(); ++it){
        ExtendedEdge *edge = it->second;
        std::vector<int> adjacentNodesPos = edge->getAdjacentNodesPos();
        for(int i = 0; i<adjacentNodesPos.size(); i++){
            ExtendedNode *n1 = nodes.at(adjacentNodesPos.at(i));
            std::vector<ExtendedEdge*> n1_incidentEdges = n1->getIncidentEdges();
            for(int j = 0; j<n1_incidentEdges.size(); j++){
                ExtendedEdge *edge2 = n1_incidentEdges.at(j);
                if(edge2->getId() != edge->getId()){
                    std::cout << "edgesId(" << edge->getId() << ","
                              << edge2->getId() << ") angle: " << edge->getAngle(edge2->getId()) << std::endl;
                }
            }

        }

    }

    std::cout << "===================================================" << std::endl;
    std::cout << std::endl;
    std::cout << "=====================Node features===================" << std::endl;

    for (auto it = nodes.begin(); it!= nodes.end(); ++it){
        ExtendedNode *u = it->second;
        std::vector<int> adnodes = u->getAdjacentNodes();
        std::cout << "Current_nodeId(" << u->getId() << ") / Current_nodeCoord("
                  << u->getX() << "," << u->getY() << "," << u->getZ() << ") nb_adjacent_node("
                  << adnodes.size() << ") :";
        std::vector<ExtendedEdge *> incidentEdge = u->getIncidentEdges();
        for(int i= 0; i<incidentEdge.size(); ++i){
            std::cout << std::endl;
            std::cout << "\t";
            ExtendedEdge *e = incidentEdge.at(i);
            std::cout << " edgeId(" << e->getId() << ")-edgeSize(" << e->getSize()
                      << ") / Opposite_nodeId(" << e->getOppositeNode(u->getId()) << ") ";
        }
        std::cout << std::endl;
//        std::cout << " " << std::endl;
//        for(int i= 0; i<adnodes.size(); ++i){
//            std::cout << " " << adnodes.at(i);
//        }
//        std::cout << " " << std::endl;
    }
    std::cout << "=====================================================" << std::endl;
}

std::unordered_map<int, ExtendedNode*> SkeletonGraph::getENodes()
{
    return nodes;
}

std::unordered_map<int, ExtendedEdge*> SkeletonGraph::getEEdges()
{
    return edges;
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
        
        //std::cout << "exportGraph"<< nbx++ << " : "
        //<< nit->second->getId() << " "
        //<< nit->second->getX() << " "
        //<< nit->second->getY() << " "
        //<< nit->second->getZ() << std::endl;
        
        coords[n] = Point(x*10+(nbz*z),y*10+(nbz*z));
        sizes[n]  = 20;
        colors[n] = 0;
        shapes[n] = 0;
    }

    for (ListGraph::ArcIt ait(graph); ait != INVALID; ++ait){
        acolors[ait] = 0;
        widths[ait]  = 1;
    }
    
    //std::cout << "Create " << name << std::endl;
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
