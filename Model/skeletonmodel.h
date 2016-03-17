#ifndef SKELETONMODEL
#define SKELETONMODEL 

#include "../deftypes.h"

#include <QString>
#include <QTextStream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <tiffio.h>

// ftp://ftp.remotesensing.org/pub/libtiff/tiff-4.0.6.tar.gz

class SkeletonModel {
public:
    SkeletonModel();

    void setFilename(QString filename);

    void compute();
    void fillEulerLUT(int LUT[256]);
    void fillnumOfPointsLUT(int LUT[256]);
    int getPixelNoCheck(int x, int y, int z);
    int N(int x, int y, int z);
    int S(int x, int y, int z);
    int E(int x, int y, int z);
    int W(int x, int y, int z);
    int U(int x, int y, int z);
    int B(int x, int y, int z);
    bool isEndPoint(int x, int y, int z);
    void getNeighborhood(int neighborhood[27], int x, int y, int z);
    int getPixel(int x, int y, int z);
    bool isEulerInvariant(int neighbors[27], int LUT[256]);
    uint8 indexOctantSWU(int neighbors[27]);
    uint8 indexOctantSEU(int neighbors[27]);
    uint8 indexOctantNWU(int neighbors[27]);
    uint8 indexOctantNEU(int neighbors[27]);
    uint8 indexOctantSWB(int neighbors[27]);
    uint8 indexOctantSEB(int neighbors[27]);
    uint8 indexOctantNWB(int neighbors[27]);
    uint8 indexOctantNEB(int neighbors[27]);
    bool isSimplePoint(int neighbors[27]);
    void octreeLabeling(int octant, int label, int cube[26]);

    void loadTIFFFile();

    void setDataFromFile();
    void extractMinAndMaxFromData(double &xmin, double &xmax, double &ymin,
                                  double &ymax, double &zmin, double &zmax);
    int normalize(double xmin, double xmax, double ymin,
                  double ymax, double zmin, double zmax);
    void generate3DImFromData();

    Image3D<short int>* getSkeleton3DIm();
    void setSkeleton3DIm(Image *image);


private:
    QString filename;
    std::vector<double> data;
    std::vector<bool> boolMap;
    std::vector<bool> isNodeTab;
    Image3D<short int> *skeletonIm3D;
    int width, height, depth;
};

#endif // SKELETONMODEL
