#include "skeletonmodel.h"

SkeletonModel::SkeletonModel() {
    AllocateSkeleton(&skeleton, 50000, 5000);
}

void SkeletonModel::compute(Image *image, QString filename, int dc, int fs, int p) {
    int dimX = image->n_cols;
    int dimY = image->n_rows;
    int dimZ = image->n_slices;

    int distCharges = dc; /** TODO **/ // distance from object boundary where to place the charges (>=0)
    int fieldStrenght = fs; /** TODO **/ // potential field strenght (1 .. 10)
    int percHDPts = p; /** TODO **/ // percentage of highest divergence points to use in skeleton construction (0 .. 100)

    char *volFilename, *vectorFieldInputFile, *vectorFieldOutputFile;

    volFilename = (char *)malloc(filename.length()*sizeof(char));
    for(int i = 0; i < filename.length(); i++) {
        volFilename[i] = filename.toStdString().at(i);
        //std::cout << "i = " << i << " - at = " << filename.toStdString().at(i) << std::endl;
    }
    volFilename[filename.length()] = '\0';
    vectorFieldInputFile = NULL;
    vectorFieldOutputFile = NULL;

    cbChangeParameters chgParamsFunction = NULL;
    void* chgParamsArg = NULL;

    //std::cout << "filename (String) = " << filename.toStdString() << std::endl;
    //std::cout << "filename (Char) = " << volFilename << std::endl;

    pfSkel(volFilename, dimX, dimY, dimZ, distCharges, fieldStrenght, percHDPts,
            skeleton,
            chgParamsFunction, chgParamsArg,
            vectorFieldInputFile, vectorFieldOutputFile);

    std::cout << "numPoints = " << skeleton->numPoints << std::endl;
    std::cout << "numSegments = " << skeleton->numSegments << std::endl;

    SaveSkeleton(skeleton, "skeleton", 1);

    FreeSkeleton(&skeleton);
}
