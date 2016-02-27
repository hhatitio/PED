#ifndef SKELETONGRAPH
#define SKELETONGRAPH

#include <QFile>
#include <lemon/list_graph.h>
//#include "Model/image3D.h"
#include "deftypes.h"

using namespace lemon;

class SkeletonGraph
{
public:
    SkeletonGraph();
    ~SkeletonGraph();
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

private:
    ListDigraph graph;
    std::vector<double> data;
    std::vector<bool> boolMap;
    std::vector<bool> isNodeTab;
    Image3D<short int> *skeletonIm3D;
};

#endif // SKELETONGRAPH

