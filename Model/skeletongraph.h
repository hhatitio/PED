#ifndef SKELETONGRAPH
#define SKELETONGRAPH

#include <QFile>
#include <lemon/list_graph.h>
#include <lemon/graph_to_eps.h>
#include <lemon/math.h>
#include <math.h>
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

    void setSkeletonIm(Image* im);
    void compute();
    Image* getSkeleton3DIm();
    Image* getGraphImage3D();
    void exportGraph(std::string name);

    bool isIntersectionNode(int pos);
    void initGraph();
    bool contractNodes(ExtendedNode *n);
    bool mergeNodes(ExtendedNode *n1,ExtendedNode *n2);
    bool eraseNodes(ExtendedNode *n);
    void initNodeEdges(int pos);
    void processIntersectionNode(int pos);
    void processNodeWith2neighboor(ExtendedNode *u);
    int getNodeWeight(ExtendedNode *u);
    bool hasBiggerWeiht(int pos);
    std::unordered_map<int, ExtendedNode*> getENodes();
    std::unordered_map<int, ExtendedEdge*> getEEdges();
    void computeAngles();
    void showfeatures();

private:
    ListGraph graph;
    std::vector<double> data;
    std::unordered_map<int, ExtendedNode*> nodes;
    std::unordered_map<int, ExtendedEdge*> edges;
    Image3D<short int> skeletonIm3D;
    Image3D<short int> skeletonImTmp;
    Image *neighboorMap;
    std::unordered_map<int, int> enhanceNeighboorMap;
    std::unordered_map<int, ExtendedNode*> nodesTmp;

    void computeEnhanceMapFromNeighboorMap();
    void computeNeighboorMap();
    ExtendedEdge* addEdge(ListGraph &graph, ExtendedNode* u, ExtendedNode* v,
                          int pos_u, int pos_v);

};





#endif // SKELETONGRAPH

