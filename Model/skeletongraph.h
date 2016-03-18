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
    bool eraseNodes(ExtendedNode *n);
    void initNodeEdges(int pos);
    void updateNodeMap(std::unordered_map<int,ExtendedNode*> nodesList);
    void updateNodeMap();
    void processIntersectionNode(int pos);
    void processNodeWith2neighboor(ExtendedNode *u);
    int getNodeWeight(ExtendedNode *u);
    bool hasBiggerWeiht(int pos);
    void exportGraph(std::string name);

private:
    ListGraph graph;
    std::vector<double> data;
    std::unordered_map<int, ExtendedNode*> nodes;
    Image3D<short int> skeletonIm3D;
    Image3D<short int> skeletonImTmp;
    Image *neighboorMap;
    std::unordered_map<int, int> enhanceNeighboorMap;
    std::unordered_map<int, ExtendedNode*> voxels;
    std::vector<int> visitedVoxel;


    void addVisitedVoxel(int pos);
    bool isVisited(int pos);
    void computeEnhanceMapFromNeighboorMap();
    void computeNeighboorMap();
    int nextVoxelPosition(int x, int y, int z);
    std::vector<ExtendedNode*> getNeighboorhoodList(ExtendedNode* n);
};





#endif // SKELETONGRAPH

