#ifndef SKELETONVIEW
#define SKELETONVIEW

#include "../deftypes.h"
#include "Model/skeletonmodel.h"

#include <iostream>
#include <QString>

class SkeletonView
{
public:
    SkeletonView();
    void show(Image *image);
    void setFilename(QString filename);

private:
    SkeletonModel *skeletonModel;
    QString filename;
};

#endif // SKELETONVIEW

