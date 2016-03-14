#ifndef GRAPHTEST_H
#define GRAPHTEST_H

#include <QFile>
#include <lemon/list_graph.h>
#include <lemon/graph_to_eps.h>
#include <lemon/math.h>
#include <unordered_map>

//#include "Model/image3D.h"
#include "deftypes.h"

using namespace lemon;

class GraphTest
{
public:
    GraphTest();
    GraphTest(Image* im);
    ~GraphTest();
    void compute();
    void initGraph();
    void setGraph(Image* im);
    bool contractNodes(int pos);
    void arcToNeighboors(int pos,int x, int y, int z/*ExtendedNode *n0*/);
    void updateNodeMap();
    Image* getSkeleton3DIm();
    Image* getGraphImage3D();
signals:

public slots:

private:
    ListGraph graph;
    std::vector<double> data;
    std::unordered_map<int, int> nodes;
    Image3D<short int> skeletonIm3D;
    Image3D<short int> skeletonImTmp;
};

#endif // GRAPHTEST_H
