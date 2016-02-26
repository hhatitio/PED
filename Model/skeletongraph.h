#ifndef SKELETONGRAPH
#define SKELETONGRAPH

#include <QFile>
#include <lemon/list_graph.h>

using namespace lemon;

class SkeletonGraph
{
public:
    SkeletonGraph();
    ~SkeletonGraph();
    void compute();

private:
    ListDigraph graph;
}

#endif // SKELETONGRAPH

