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
    int computeNbNode();
    bool isIntersectionNode(int pos);
    Image* getSkeleton3DIm();
    Image* getGraphImage3D();
    void initGraph();
    bool contractNodes(ExtendedNode *n);
    bool mergeNodes(ExtendedNode *n1,ExtendedNode *n2);
    bool transformIntoArc(ExtendedNode *n);
    bool eraseNodes(ExtendedNode *n);
    void initNodeEdges(int pos);
    void arcToSingleNeighboor(ExtendedNode *n, int n_pos);
    void updateNodeMap(std::unordered_map<int,ExtendedNode*> nodesList);
    void updateNodeMap();
    void processIntersectionNode(ExtendedNode *u);
    void processNodeWith2neighboor(ExtendedNode *u);
    int getNodeWeight(ExtendedNode *u);
    bool hasBiggerWeiht(int pos);

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

