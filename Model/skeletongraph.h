#ifndef SKELETONGRAPH
#define SKELETONGRAPH

#include <QFile>
#include <lemon/list_graph.h>
#include <unordered_map>

//#include "Model/image3D.h"
#include "extendededge.h"
#include "extendednode.h"
#include "deftypes.h"

using namespace lemon;

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
    void initGraph();
    void addNeighboor(int x, int y, int z, ListGraph::Node node);

private:
    ListGraph graph;
    std::vector<double> data;
    std::vector<bool> isVoxelChecked;
    std::vector<bool> isNodeTab;
    std::unordered_map<int, ExtendedNode> nodes;
    Image3D<short int> *skeletonIm3D;
    Image3D<short int> skeletonImTmp;
};





#endif // SKELETONGRAPH

