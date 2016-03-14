#include "graphtest.h"

GraphTest::GraphTest()
{

}
GraphTest::GraphTest(Image* im) {
    const Image3D<short int> im_const = *im;
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(skeletonIm3D);
}

GraphTest::~GraphTest() {}

void GraphTest::compute() {

    for (auto it = nodes.begin(); (it!= nodes.end()); ++it){
        int pos = it->first;
        if(contractNodes(pos)){
            skeletonImTmp.at(pos)=255;
        }
    }
    updateNodeMap();
}

void GraphTest::initGraph(){

    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;

    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_rows)+(z*nb_rows*nb_cols);
                if(skeletonImTmp.at(pos) == 200) {
                    arcToNeighboors(pos,x,y,z);
                    skeletonImTmp.at(pos) = 120;
                }
                if( skeletonImTmp.at(pos) == 255) {
                    //ExtendedNode *d = new ExtendedNode(graph, x, y, z);
                    int id = graph.id(graph.addNode());
                    //voxels.insert({pos,id});
                    nodes.insert({pos ,id});
                    arcToNeighboors(pos,x,y,z);
                    skeletonImTmp.at(pos) = 120;
                }
            }
        }
    }
}

void GraphTest::updateNodeMap(){
    std::cout << "taille nodeMap : " << nodes.size() << std::endl;
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_rows)+(z*nb_rows*nb_cols);
                if(skeletonImTmp.at(pos) == 255) {
                    if(nodes.find(pos) != nodes.end()){
                        nodes.erase(pos);
                    }
                }
            }
        }
    }
    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_rows)+(z*nb_rows*nb_cols);
                if(skeletonImTmp.at(pos) == 120) {
                    ListGraph::Node u = graph.nodeFromId(nodes.at(pos));
                    std::vector<ListGraph::Node> neighboors;
                    for(ListGraph::OutArcIt e(graph, u); e!=INVALID; ++e){
                        neighboors.push_back(graph.oppositeNode(u,e));
                    }
                    //if (neighboors.size() == 3) {
                        std::cout << "x = " << x << " y = " << y << " z = " << z << std::endl;
                        //std::cout << "\t pos = " << pos << std::endl;
                        //std::cout << "\t color = " << skeletonImTmp.at(pos) << std::endl;
                        //std::cout << "\t node id = " << nodes.at(pos) << std::endl;
                        std::cout << "\t neighboor size = " << neighboors.size() << std::endl;
                        for(int i = 0; i < neighboors.size(); i++)
                            std::cout << "\t\t neighboor id = " << graph.id(neighboors.at(i)) << std::endl;
                    //}
                }
            }
        }
    }
    std::cout << "taille nodeMap : " << nodes.size() << std::endl;
}

void GraphTest::setGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    //skeletonIm3D = new Image3D<short int>(im_const);
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(im_const);
}

void GraphTest::arcToNeighboors(int pos,int x, int y, int z/*ExtendedNode *n0*/){
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
    //    std::vector<VertexGraph *> lnodes;

    //int x = n0->getX();
    //int y = n0->getY();
    //int z = n0->getZ();

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

                        /*if (skeletonImTmp.at(n_pos) == 200) {
                            n0->addAdjacentNode(nodes.at(n_pos)->getId());
                        }*/

                        if (skeletonImTmp.at(n_pos) == 255 || skeletonImTmp.at(n_pos) == 200) {
                            //arcToSingleNeighboor(n0, n_pos);
                            int id = graph.id(graph.addNode());
                            nodes.insert({n_pos,id});
                            //n0->addAdjacentNode(nodes.at(n_pos)->getId());
                            graph.addEdge(graph.nodeFromId(nodes.at(pos)),
                                          graph.nodeFromId(nodes.at(n_pos)));
                            skeletonImTmp.at(n_pos) = 200;
                        }
                    }
                }
            }
        }
    }
    //return lnodes;
}

bool GraphTest::contractNodes(int pos){
    bool res = false;
    ListGraph::Node u = graph.nodeFromId(nodes.at(pos));
    std::vector<ListGraph::Node> neighboors;
    for(ListGraph::OutArcIt e(graph, u); e!=INVALID; ++e){
        //ListGraph::Node u1 = graph.u(e);
        //ListGraph::Node v = graph.v(e);
        //if(u1 == u){
        neighboors.push_back(graph.oppositeNode(u,e));
        //}else{
        //  neighboors.push_back(u1);
        //}
    }
    if (neighboors.size()==2) {
        res = true;
        ListGraph::Node v = neighboors.at(0);
        //ListGraph::Node v1 = neighboors.at(1);
        //graph.addEdge(v, v1);
        //graph.erase(u);
        graph.contract(v,u);
        //skeletonImTmp.at(pos)=255;
    }
    return res;
}

Image* GraphTest::getSkeleton3DIm(){
    //    const Image3D<short int> im = *skeletonIm3D;
    return new Image3D<short int>(skeletonImTmp);
}


Image* GraphTest::getGraphImage3D(){
    //    const Image3D<short int> im = *skeletonIm3D;
    Image3D<short int> *im = new Image3D<short int>(skeletonIm3D);

    for (auto it = nodes.begin(); it!= nodes.end(); ++it){
        im->at(it->first) = 120;
    }

    return im;
}
