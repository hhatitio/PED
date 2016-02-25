#ifndef SKELETONMODEL
#define SKELETONMODEL

#include "../deftypes.h"
#include "pfSkel/pfSkel.h"

#include <QString>
#include <iostream>
#include <cstdlib>

class SkeletonModel {
public:
    SkeletonModel();
    void compute(Image *image, QString filename);
private:
    Skeleton *skeleton;
};

#endif // SKELETONMODEL

