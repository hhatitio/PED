#ifndef SKELETONMODEL
#define SKELETONMODEL 

#include "../deftypes.h"

#include <QString>
#include <QProcess>
#include <iostream>
#include <cstdlib>
#include <jni.h>
#include <string>

#define PATH_TO_IMAGEJ       "../ImageJ/ImageJ"
#define MAKE_BINARY          " -run \"Make Binary\""
#define SKELETONIZE          " -run \"Skeletonize (2D/3D)\""
#define SAVE_AS              " -run \"Raw Data...\""

class SkeletonModel {
public:
    SkeletonModel();
    void setFilename(QString filename);
    void compute();
private:
    QString filename;
};

#endif // SKELETONMODEL

