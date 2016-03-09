#ifndef SKELETONGRAPH
#define SKELETONGRAPH

#include <QFile>
#include <lemon/list_graph.h>
#include <lemon/graph_to_eps.h>
#include <lemon/math.h>
#include <unordered_map>

//#include "Model/image3D.h"
#include "extendededge.h"
#include "extendednode.h"
#include "deftypes.h"

using namespace lemon;

typedef dim2::Point<int> Point;

class SkeletonGraph
{
public:
    SkeletonGraph();
    SkeletonGraph(Image* im);
    ~SkeletonGraph();
    
    void setGraph(Image* im);
    void compute();
    void setDataFromFile(QString filename);
    void generate3DImFromData();
    
    void extractMinAndMaxFromData(double &xmin, double &xmax, double &ymin,
                                  double &ymax, double &zmin, double &zmax);
    int normalize(double xmin, double xmax, double ymin,
                  double ymax, double zmin, double zmax);
    
    int computeNbNode();
    bool isNode(int x, int y, int z);
    Image* getSkeleton3DIm();
    Image* getGraphImage3D();
    void initGraph();
    bool contractNodes(ExtendedNode *n);
    bool transformIntoArc(ExtendedNode *n);
    bool eraseNodes(ExtendedNode *n);
    bool spotHighDivergentNodes(ExtendedNode *n);
    void arcToNeighboors(ExtendedNode *n);
    void arcToSingleNeighboor(ExtendedNode *n, int n_pos);
    std::vector<ExtendedNode*> getNeighboorhood(int x,
                                                     int y,
                                                     int z);
    

private:
    ListGraph graph;
    std::vector<double> data;
    std::vector<bool> isVoxelChecked;
    std::vector<bool> isNodeTab;
    std::unordered_map<int, ExtendedNode*> nodes;
    //std::unordered_map<int, bool> doesEdgeExist;
    Image3D<short int> skeletonIm3D;
    Image3D<short int> skeletonImTmp;
};





#endif // SKELETONGRAPH

