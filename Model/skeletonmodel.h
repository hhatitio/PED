#ifndef SKELETONMODEL
#define SKELETONMODEL 

#include "../deftypes.h"

#include <QString>
#include <QProcess>
#include <QTextStream>
#include <iostream>
#include <cstdlib>
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
    void setDataFromFile();
    void generate3DImFromData();
    void extractMinAndMaxFromData(double &xmin, double &xmax, double &ymin,
                                  double &ymax, double &zmin, double &zmax);

    int normalize(double xmin, double xmax, double ymin,
                  double ymax, double zmin, double zmax);
    Image* getSkeleton3DIm();

private:
    QString filename;
    std::vector<double> data;
    std::vector<bool> boolMap;
    std::vector<bool> isNodeTab;
    Image3D<short int> *skeletonIm3D;
};

#endif // SKELETONMODEL

