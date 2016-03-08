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

SkeletonGraph::~SkeletonGraph() {}

void SkeletonGraph::setGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    //skeletonIm3D = new Image3D<short int>(im_const);
    skeletonIm3D = Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(im_const);
}

void SkeletonGraph::compute() {
    //setDataFromFile("/Users/andrearuffino/Downloads/cow-fs6-hd0.txt");
    //generate3DImFromData();
    initGraph();
    //std::cout << "nb_node : " << initGraph() << std::endl;
}

/*
 void SkeletonGraph::setDataFromFile(QString filename) {
 //    QFile file(filename);
 //    if(!file.open(QIODevice::ReadOnly)){
 //
 //    }else{
 //        QTextStream fileStream(&file);
 //        while(!fileStream.atEnd()){
 //            QString line = fileStream.readLine();
 //            QStringList sp_line = line.split(" ");
 //            data.push_back(sp_line.at(0).toDouble());
 //            data.push_back(sp_line.at(1).toDouble());
 //            data.push_back(sp_line.at(2).toDouble());
 //        }
 //    }
 }
 
 void SkeletonGraph::extractMinAndMaxFromData(double &xmin, double &xmax,
 double &ymin, double &ymax,
 double &zmin, double &zmax)
 {
 //    xmin = xmax = data[0];
 //    ymin = ymax = data[1];
 //    zmin = zmax = data[2];
 //
 //    for (unsigned long i = 3; i < data.size(); i+=3) {
 //
 //        if(data[i+0]< xmin) xmin = data[i+0];
 //        if(data[i+0]> xmax) xmax = data[i+0];
 //
 //        if(data[i+1]< ymin) ymin = data[i+1];
 //        if(data[i+1]> ymax) ymax = data[i+1];
 //
 //        if(data[i+2]< zmin) zmin = data[i+2];
 //        if(data[i+2]> zmax) zmax = data[i+2];
 //    }
 }
 
 int SkeletonGraph::normalize(double xmin, double xmax,
 double ymin, double ymax,
 double zmin, double zmax)
 {
 //    double delta_x = xmax - xmin;
 //    double delta_y = ymax - ymin;
 //    double delta_z = zmax - zmin;
 //
 //    double delta = (delta_x > delta_y) ?
 //    ( (delta_x > delta_z) ? delta_x : delta_z):
 //    ( (delta_y > delta_z) ? delta_y : delta_z);
 //
 //    for(unsigned long i = 0; i < data.size(); i+= 3){
 //        data[i] = (data[i]-xmin) / delta;
 //        data[i+1] = (data[i+1]-ymin) / delta;
 //        data[i+2] = (data[i+2]-zmin) / delta;
 //    }
 //
 //    std::cout << "max   : " << xmax <<" "<< ymax <<" "<< zmax << std::endl;
 //    std::cout << "min   : " << xmin <<" "<< ymin <<" "<< zmin << std::endl;
 //    std::cout << "delta : " << delta_x <<" "<< delta_y <<" "<< delta_z << std::endl;
 //    std::cout << "D     : " << delta << std::endl;
 //
 //    return (floor(delta));
 }
 
 void SkeletonGraph::generate3DImFromData() {
 
 //    double xmin,xmax;
 //    double ymin,ymax;
 //    double zmin,zmax;
 //
 //    extractMinAndMaxFromData(xmin,xmax,ymin,ymax,zmin,zmax);
 //    int delta = normalize(xmin,xmax,ymin,ymax,zmin,zmax);
 //    int size = floor((double) delta*1.0);
 //    int row, col, slice;
 //    row = col = slice = size;
 //
 //    skeletonIm3D = new Image3D<short int>(row,col,slice);
 //
 //    for(int x = 0; x < row; x++){
 //        for(int y = 0; y < col; y++){
 //            for(int z = 0; z < slice; z++){
 //                skeletonIm3D->at(x+(y*col)+(z*row*col)) = 0;
 //                isVoxelChecked.push_back(false);
 //                isNodeTab.push_back(false);
 //                //nodes.push_back(NULL);
 //            }
 //        }
 //    }
 //    for(unsigned long i = 0; i < data.size(); i+= 3){
 //
 //        int x = floor(data[i]*row);
 //        int y = floor(data[i+1]*col);
 //        int z = floor(data[i+2]*slice);
 //
 //        skeletonIm3D->at(x+(y*col)+(z*row*slice)) = 255;
 //
 ////        std::cout << x <<" "<< y <<" "<< z;
 ////        std::cout << " : " << data[i]
 ////                  << " " << data[i+1]
 ////                  << " " << data[i+2]
 ////                  << std::endl;
 //    }
 //
 //    std::cout << "data : " << data.size() << "/" << 372*3 << std::endl;
 }
 
 int SkeletonGraph::computeNbNode(){
 //    int nb_rows = skeletonIm3D->n_rows;
 //    int nb_cols = skeletonIm3D->n_cols;
 //    int nb_slices = skeletonIm3D->n_slices;
 //
 //    std::cout << "rows : " << nb_rows <<  std::endl;
 //    std::cout << "cols : " << nb_cols <<  std::endl;
 //    std::cout << "slices : " << nb_slices << std::endl;
 //
 //    int nb_node = 0;
 //
 //    for(unsigned long i = 0; i < data.size(); i+= 3){
 //
 //        int x = floor(data[i+0]*nb_cols);
 //        int y = floor(data[i+1]*nb_rows);
 //        int z = floor(data[i+2]*nb_slices);
 //
 //        int pos = x+(y*nb_cols)+(z*nb_rows*nb_cols);
 //
 //        if(!isVoxelChecked[pos]){
 //            ExtendedNode n = ExtendedNode(graph, x, y, z);
 //
 //            if(isNode(x,y,z)){
 //                nb_node++;
 //                isNodeTab[pos] = true;
 //            }
 //            isVoxelChecked[pos] = true;
 //        }
 //    }
 //    return nb_node;
 }
 */

void SkeletonGraph::initGraph(){
    
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
    
    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_cols)+(z*nb_rows*nb_cols);
                if(skeletonImTmp.at(pos) == 255) {
                    ExtendedNode *d = new ExtendedNode(graph, x, y, z);
                    nodes.insert({pos ,d});
                }
            }
        }
    }
    
    for (auto it = nodes.begin(); it!= nodes.end(); ++it){
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
    
    for (auto it = nodes.begin(); it!= nodes.end(); ++it){
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
    .arcWidthScale(.005).run();
}

void SkeletonGraph::arcToNeighboors(ExtendedNode *n0){
    int nb_x = skeletonImTmp.n_rows;
    int nb_y = skeletonImTmp.n_cols;
    int nb_z = skeletonImTmp.n_slices;
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
                    int n_pos = x1+(y1*nb_y)+(z1*nb_x*nb_y);
                    
                    if(x1 >= 0   && y1 >= 0   && z1 >= 0   &&
                       x1 < nb_x && y1 < nb_y && z1 < nb_z) {
                        
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
                       (x1<nb_cols && y1<nb_rows && z1<nb_slices)){
                        int pos = x1+(y1*nb_cols)+(z1*nb_rows*nb_cols);
                        
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
