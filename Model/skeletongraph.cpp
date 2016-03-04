#include "skeletongraph.h"
#include <qtextstream.h>
#include <limits>
#include <string>


SkeletonGraph::SkeletonGraph() {

}

SkeletonGraph::SkeletonGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    skeletonIm3D = new Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(im_const);
}

SkeletonGraph::~SkeletonGraph() {}

void SkeletonGraph::setGraph(Image* im) {
    const Image3D<short int> im_const = *im;
    //skeletonIm3D = new Image3D<short int>(im_const);
    skeletonImTmp = Image3D<short int>(im_const);
}

void SkeletonGraph::compute() {
    //setDataFromFile("/Users/andrearuffino/Downloads/cow-fs6-hd0.txt");
    //generate3DImFromData();
    initGraph();
    //std::cout << "nb_node : " << initGraph() << std::endl;
}

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

void SkeletonGraph::initGraph(){
    
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
    
    for (int x = 0; x < nb_rows ; ++x){
        for (int y = 0; y < nb_cols ; ++y){
            for (int z = 0; z < nb_slices ; ++z){
                int pos = x+(y*nb_cols)+(z*nb_rows*nb_cols);
                if(skeletonImTmp.at(pos) == 255) {
                    ExtendedNode d = ExtendedNode(graph, x, y, z);
                    nodes.insert({pos ,d});
                }
            }
        }
    }
    
    for (auto it = nodes.begin(); it!= nodes.end(); ++it){
        int pos_node = it->first; // TODO convert to xyz.
        ExtendedNode n = it->second;
        std::cout << "node | " << pos_node
                  << ":" << n.getId() << std::endl;
    }
}

void SkeletonGraph::addNeighboor(int x, int y, int z, ListGraph::Node){
    int nb_rows = skeletonImTmp.n_rows;
    int nb_cols = skeletonImTmp.n_cols;
    int nb_slices = skeletonImTmp.n_slices;
//    std::vector<VertexGraph *> lnodes;
    for (int i = -1; i <= 1 ; ++i) {
        for (int j = -1; j <= 1 ; ++j) {
            for (int k = -1; k <= 1 ; ++k) {
                if(i!=0 || j!=0 || k!=0){
                    int x1 = x+i;
                    int y1 = y+j;
                    int z1 = z+k;
                    if(isNode(x1,y1,z1)){
                        
//                      lnodes.push_back(new VertexGraph(graph,x,y,z));
                    }
                    if(skeletonImTmp.at(x1+(y1*nb_cols)+(z1*nb_rows*nb_cols)) == 255){
                        //skeletonImTmp.at(x1+(y1*nb_cols)+(z1*nb_rows*nb_cols)) == 125;
                        //return getNodeNeighboor(x1,y1,z1);
                    }
                }
            }
        }
    }
    //return lnodes;
}

bool SkeletonGraph::isNode(int x, int y, int z){
    int nb_rows = skeletonIm3D->n_rows;
    int nb_cols = skeletonIm3D->n_cols;
    int nb_slices = skeletonIm3D->n_slices;
    int nb_neighboor = 0;
    
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
                        if(!isNodeTab[pos] && skeletonIm3D->at(x1+(y1*nb_cols)+(z1*nb_rows*nb_cols)) == 255){
                            nb_neighboor++;
                        }
                    }
                }
            }
        }
    }
    std::cout <<"at: " << x << " "<< y << " " << z << " "
    << "neighboor : " << nb_neighboor << " ("
    << skeletonIm3D->at(x+(y*nb_cols)+(z*nb_rows*nb_cols)) << ")" << std::endl;
    
    return ((nb_neighboor >= 3)||(nb_neighboor == 1));
}

Image* SkeletonGraph::getSkeleton3DIm(){
//    const Image3D<short int> im = *skeletonIm3D;
    return new Image3D<short int>(skeletonImTmp);
}
