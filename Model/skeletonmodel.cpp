#include "skeletonmodel.h"

SkeletonModel::SkeletonModel() {
    skeletonIm3D = new Image3D<short int>();
}

SkeletonModel::~SkeletonModel() {
    if (skeletonIm3D != NULL) delete skeletonIm3D;
}

void SkeletonModel::setFilename(QString filename) {
    this->filename = filename;
}

/***
        SKELETONIZE3D (Fiji)
        Author : Ignacio Arganda-Carreras
        http://imagej.net/Skeletonize3D

        from Lee et al. "Building skeleton models via 3-D medial surface/axis thinning algorithms.
            Computer Vision, Graphics, and Image Processing, 56(6):462-478, 1994."
  ***/

void SkeletonModel::compute() {

    this->width = (int)this->skeletonIm3D->n_rows;
    this->height = (int)this->skeletonIm3D->n_cols;
    this->depth = (int)this->skeletonIm3D->n_slices;

    for(int x = 0; x < this->width; x++)
        for(int y = 0; y < this->height; y++)
            for(int z = 0; z < this->depth; z++) {
                int pos = x+y*this->width+z*this->height*this->width;
                if (this->skeletonIm3D->at(pos) != 0)
                    this->skeletonIm3D->at(pos) = 1;
            }

    // compute thin image

    // prepare Euler LUT
    int eulerLUT[256];
    for(int i = 0; i < 256; i++)
        eulerLUT[i] = 0;
    fillEulerLUT(eulerLUT);

    // prepare number of points LUT
    int pointsLUT[256];
    fillnumOfPointsLUT(pointsLUT);

    int iter = 1;
    // Loop through the image several times until there is no change
    int unchangedBorders = 0;
    while (unchangedBorders < 6) { // loop until no change for all the six border types

        unchangedBorders = 0;
        for(int currentBorder = 1; currentBorder <= 6; currentBorder++) {
            std::cout << "Thinning iteration " << iter << " (" << currentBorder << "/6 borders) ..." << std::endl;
            bool noChange = true;
            std::vector<std::array<int,3>> simpleBorderPoints;

            // Loop through the image
            for(int z = 0; z < this->depth; z++) {
                for(int y = 0; y < this->height; y++) {
                    for(int x = 0; x < this->width; x++) {
                        // check if point is foreground
                        if (getPixelNoCheck(x, y, z) != 1) {
                            continue; // current point is already background
                        }
                        // check 6-neighbors if points is a border point of type currentBorder
                        bool isBorderPoint = false;
                        // north
                        if (currentBorder == 1 && N(x, y, z) <= 0) {
                            isBorderPoint = true;
                        }
                        // south
                        if (currentBorder == 2 && S(x, y, z) <= 0) {
                            isBorderPoint = true;
                        }
                        // east
                        if (currentBorder == 3 && E(x, y, z) <= 0) {
                            isBorderPoint = true;
                        }
                        // west
                        if (currentBorder == 4 && W(x, y, z) <= 0) {
                            isBorderPoint = true;
                        }
                        if (this->depth > 1) {
                            // up
                            if (currentBorder == 5 && U(x, y, z) <= 0) {
                                isBorderPoint = true;
                            }
                            // bottom
                            if (currentBorder == 6 && B(x, y, z) <= 0) {
                                isBorderPoint = true;
                            }
                        }
                        if (!isBorderPoint) {
                            continue; // current point is not deletable
                        }
                        if (isEndPoint(x, y, z)) {
                            continue;
                        }
                        int neighborhood[27];
                        getNeighborhood(neighborhood, x, y ,z);
                        // Check if point is Euler invariant
                        if (!isEulerInvariant(neighborhood, eulerLUT)) {
                            continue; // current point is not deletable
                        }
                        // Check if point is simple (deletion does not change connectivity in the 3x3x3 neighborhood)
                        if (!isSimplePoint(neighborhood)) {
                            continue; // current point is not deletable
                        }
                        // add all simple border points to a list for sequential re-checking
                        std::array<int,3> index;
                        index[0] = x;
                        index[1] = y;
                        index[2] = z;
                        simpleBorderPoints.push_back(index);
                    }
                }
            }
            // sequential re-checking to preserve connectivity when deleting in a parallel way
            for(unsigned long i = 0; i < simpleBorderPoints.size(); i++) {
                std::array<int,3> index = simpleBorderPoints.at(i);
                // check if border points is simple
                int neighboors[27];
                getNeighborhood(neighboors, index[0], index[1], index[2]);
                if (isSimplePoint(neighboors)) {
                    // we can delete the current point
                    int pos = index[0]+index[1]*this->width+index[2]*this->height*this->width;
                    this->skeletonIm3D->at(pos) = 0;
                    noChange = false;
                }
            }
            if (noChange) unchangedBorders++;
        }
        iter++;
    }

    std::cout << "Skeletonization done." << std::endl;

    for(int x = 0; x < this->width; x++)
        for(int y = 0; y < this->height; y++)
            for(int z = 0; z < this->depth; z++) {
                int pos = x+y*this->width+z*this->height*this->width;
                this->skeletonIm3D->at(pos) *= 255;
            }
}

void SkeletonModel::fillEulerLUT(int LUT[256]) {
    LUT[1]  =  1;
    LUT[3]  = -1;
    LUT[5]  = -1;
    LUT[7]  =  1;
    LUT[9]  = -3;
    LUT[11] = -1;
    LUT[13] = -1;
    LUT[15] =  1;
    LUT[17] = -1;
    LUT[19] =  1;
    LUT[21] =  1;
    LUT[23] = -1;
    LUT[25] =  3;
    LUT[27] =  1;
    LUT[29] =  1;
    LUT[31] = -1;
    LUT[33] = -3;
    LUT[35] = -1;
    LUT[37] =  3;
    LUT[39] =  1;
    LUT[41] =  1;
    LUT[43] = -1;
    LUT[45] =  3;
    LUT[47] =  1;
    LUT[49] = -1;
    LUT[51] =  1;

    LUT[53] =  1;
    LUT[55] = -1;
    LUT[57] =  3;
    LUT[59] =  1;
    LUT[61] =  1;
    LUT[63] = -1;
    LUT[65] = -3;
    LUT[67] =  3;
    LUT[69] = -1;
    LUT[71] =  1;
    LUT[73] =  1;
    LUT[75] =  3;
    LUT[77] = -1;
    LUT[79] =  1;
    LUT[81] = -1;
    LUT[83] =  1;
    LUT[85] =  1;
    LUT[87] = -1;
    LUT[89] =  3;
    LUT[91] =  1;
    LUT[93] =  1;
    LUT[95] = -1;
    LUT[97] =  1;
    LUT[99] =  3;
    LUT[101] =  3;
    LUT[103] =  1;

    LUT[105] =  5;
    LUT[107] =  3;
    LUT[109] =  3;
    LUT[111] =  1;
    LUT[113] = -1;
    LUT[115] =  1;
    LUT[117] =  1;
    LUT[119] = -1;
    LUT[121] =  3;
    LUT[123] =  1;
    LUT[125] =  1;
    LUT[127] = -1;
    LUT[129] = -7;
    LUT[131] = -1;
    LUT[133] = -1;
    LUT[135] =  1;
    LUT[137] = -3;
    LUT[139] = -1;
    LUT[141] = -1;
    LUT[143] =  1;
    LUT[145] = -1;
    LUT[147] =  1;
    LUT[149] =  1;
    LUT[151] = -1;
    LUT[153] =  3;
    LUT[155] =  1;

    LUT[157] =  1;
    LUT[159] = -1;
    LUT[161] = -3;
    LUT[163] = -1;
    LUT[165] =  3;
    LUT[167] =  1;
    LUT[169] =  1;
    LUT[171] = -1;
    LUT[173] =  3;
    LUT[175] =  1;
    LUT[177] = -1;
    LUT[179] =  1;
    LUT[181] =  1;
    LUT[183] = -1;
    LUT[185] =  3;
    LUT[187] =  1;
    LUT[189] =  1;
    LUT[191] = -1;
    LUT[193] = -3;
    LUT[195] =  3;
    LUT[197] = -1;
    LUT[199] =  1;
    LUT[201] =  1;
    LUT[203] =  3;
    LUT[205] = -1;
    LUT[207] =  1;

    LUT[209] = -1;
    LUT[211] =  1;
    LUT[213] =  1;
    LUT[215] = -1;
    LUT[217] =  3;
    LUT[219] =  1;
    LUT[221] =  1;
    LUT[223] = -1;
    LUT[225] =  1;
    LUT[227] =  3;
    LUT[229] =  3;
    LUT[231] =  1;
    LUT[233] =  5;
    LUT[235] =  3;
    LUT[237] =  3;
    LUT[239] =  1;
    LUT[241] = -1;
    LUT[243] =  1;
    LUT[245] =  1;
    LUT[247] = -1;
    LUT[249] =  3;
    LUT[251] =  1;
    LUT[253] =  1;
    LUT[255] = -1;
}

void SkeletonModel::fillnumOfPointsLUT(int LUT[256]) {
    for(int i = 0; i < 256; i++) {
        int value = i, count = 0;
        while (value > 0) {
            if ((value & 1) == 1)
                count++;
            value >>= 1;
        }
        LUT[i] = count;
    }
}

// Get pixel in 3D image (no border checking)
int SkeletonModel::getPixelNoCheck(int x, int y, int z) {
    int pos = x+y*this->width+z*this->height*this->width;
    return this->skeletonIm3D->at(pos);
}

// North neighborhood
int SkeletonModel::N(int x, int y, int z) {
    return getPixel(x, y-1, z);
}

// South neighborhood
int SkeletonModel::S(int x, int y, int z) {
    return getPixel(x, y+1, z);
}

// East neighborhood
int SkeletonModel::E(int x, int y, int z) {
    return getPixel(x+1, y, z);
}

// West neighborhood
int SkeletonModel::W(int x, int y, int z) {
    return getPixel(x-1, y, z);
}

// Up neighborhood
int SkeletonModel::U(int x, int y, int z) {
    return getPixel(x, y, z+1);
}

// Botton neighborhood
int SkeletonModel::B(int x, int y, int z) {
    return getPixel(x, y, z-1);
}

// Check if a point in the given stack is at the end of an arc
bool SkeletonModel::isEndPoint(int x, int y, int z) {
    int numberOfNeighbors = -1; // -1 and not 0 because the center pixel will be counted as well
    int neighbor[27];
    getNeighborhood(neighbor, x, y, z);
    for(int i = 0; i < 27; i++) {
        if (neighbor[i] == 1)
            numberOfNeighbors++;
    }
    return (numberOfNeighbors == 1);
}

void SkeletonModel::getNeighborhood(int neighborhood[27], int x, int y, int z) {
    neighborhood[0]  = getPixel(x-1, y-1, z-1);
    neighborhood[1]  = getPixel(x,   y-1, z-1);
    neighborhood[2]  = getPixel(x+1, y-1, z-1);

    neighborhood[3]  = getPixel(x-1, y, z-1);
    neighborhood[4]  = getPixel(x,   y, z-1);
    neighborhood[5]  = getPixel(x+1, y, z-1);

    neighborhood[6]  = getPixel(x-1, y+1, z-1);
    neighborhood[7]  = getPixel(x,   y+1, z-1);
    neighborhood[8]  = getPixel(x+1, y+1, z-1);

    neighborhood[9]  = getPixel(x-1, y-1, z);
    neighborhood[10] = getPixel(x,   y-1, z);
    neighborhood[11] = getPixel(x+1, y-1, z);

    neighborhood[12] = getPixel(x-1, y, z);
    neighborhood[13] = getPixel(x,   y, z);
    neighborhood[14] = getPixel(x+1, y, z);

    neighborhood[15] = getPixel(x-1, y+1, z);
    neighborhood[16] = getPixel(x,   y+1, z);
    neighborhood[17] = getPixel(x+1, y+1, z);

    neighborhood[18] = getPixel(x-1, y-1, z+1);
    neighborhood[19] = getPixel(x,   y-1, z+1);
    neighborhood[20] = getPixel(x+1, y-1, z+1);

    neighborhood[21] = getPixel(x-1, y, z+1);
    neighborhood[22] = getPixel(x,   y, z+1);
    neighborhood[23] = getPixel(x+1, y, z+1);

    neighborhood[24] = getPixel(x-1, y+1, z+1);
    neighborhood[25] = getPixel(x,   y+1, z+1);
    neighborhood[26] = getPixel(x+1, y+1, z+1);
}

int SkeletonModel::getPixel(int x, int y, int z) {
    if (x >= 0 && x < this->width && y >= 0 && y < this->height && z >= 0 && z < this->depth) {
        int pos = x+y*this->width+z*this->height*this->width;
        return this->skeletonIm3D->at(pos);
    }
    else return 0;
}

// Check if a point is Euler invariant
bool SkeletonModel::isEulerInvariant(int neighbors[27], int LUT[256]) {
    // Calculate Euler characteristic for each octant and sum up
    int eulerChar = 0;
    uint8 n;
    // Octant SWU
    n = indexOctantSWU(neighbors);
    eulerChar += LUT[n];
    // Octant SEU
    n = indexOctantSEU(neighbors);
    eulerChar += LUT[n];
    // Octant NWU
    n = indexOctantNWU(neighbors);
    eulerChar += LUT[n];
    // Octant NEU
    n = indexOctantNEU(neighbors);
    eulerChar += LUT[n];
    // Octant SWB
    n = indexOctantSWB(neighbors);
    eulerChar += LUT[n];
    // Octant SEB
    n = indexOctantSEB(neighbors);
    eulerChar += LUT[n];
    // Octant NWB
    n = indexOctantNWB(neighbors);
    eulerChar += LUT[n];
    // Octant NEB
    n = indexOctantNEB(neighbors);
    eulerChar += LUT[n];

    return (eulerChar == 0);
}

uint8 SkeletonModel::indexOctantSWU(int neighbors[27]) {
    uint8 n;
    n = 1;
    if (neighbors[24] == 1) n |= 128;
    if (neighbors[25] == 1) n |= 64;
    if (neighbors[15] == 1) n |= 32;
    if (neighbors[16] == 1) n |= 16;
    if (neighbors[21] == 1) n |= 8;
    if (neighbors[22] == 1) n |= 4;
    if (neighbors[12] == 1) n |= 2;
    return n;
}

uint8 SkeletonModel::indexOctantSEU(int neighbors[27]) {
    uint8 n;
    n = 1;
    if (neighbors[26] == 1) n |= 128;
    if (neighbors[23] == 1) n |= 64;
    if (neighbors[17] == 1) n |= 32;
    if (neighbors[14] == 1) n |= 16;
    if (neighbors[25] == 1) n |= 8;
    if (neighbors[22] == 1) n |= 4;
    if (neighbors[16] == 1) n |= 2;
    return n;
}

uint8 SkeletonModel::indexOctantNWU(int neighbors[27]) {
    uint8 n;
    n = 1;
    if (neighbors[18] == 1) n |= 128;
    if (neighbors[21] == 1) n |= 64;
    if (neighbors[9] == 1)  n |= 32;
    if (neighbors[12] == 1) n |= 16;
    if (neighbors[19] == 1) n |= 8;
    if (neighbors[22] == 1) n |= 4;
    if (neighbors[10] == 1) n |= 2;
    return n;
}

uint8 SkeletonModel::indexOctantNEU(int neighbors[27]) {
    uint8 n;
    n = 1;
    if (neighbors[20] == 1) n |= 128;
    if (neighbors[23] == 1) n |= 64;
    if (neighbors[19] == 1) n |= 32;
    if (neighbors[22] == 1) n |= 16;
    if (neighbors[11] == 1) n |= 8;
    if (neighbors[14] == 1) n |= 4;
    if (neighbors[10] == 1) n |= 2;
    return n;
}

uint8 SkeletonModel::indexOctantSWB(int neighbors[27]) {
    uint8 n;
    n = 1;
    if (neighbors[6] == 1)  n |= 128;
    if (neighbors[15] == 1) n |= 64;
    if (neighbors[7] == 1)  n |= 32;
    if (neighbors[16] == 1) n |= 16;
    if (neighbors[3] == 1)  n |= 8;
    if (neighbors[12] == 1) n |= 4;
    if (neighbors[4] == 1)  n |= 2;
    return n;
}

uint8 SkeletonModel::indexOctantSEB(int neighbors[27]) {
    uint8 n;
    n = 1;
    if (neighbors[8] == 1)  n |= 128;
    if (neighbors[7] == 1)  n |= 64;
    if (neighbors[17] == 1) n |= 32;
    if (neighbors[16] == 1) n |= 16;
    if (neighbors[5] == 1)  n |= 8;
    if (neighbors[4] == 1)  n |= 4;
    if (neighbors[14] == 1) n |= 2;
    return n;
}

uint8 SkeletonModel::indexOctantNWB(int neighbors[27]) {
    uint8 n;
    n = 1;
    if (neighbors[0] == 1)  n |= 128;
    if (neighbors[9] == 1)  n |= 64;
    if (neighbors[3] == 1)  n |= 32;
    if (neighbors[12] == 1) n |= 16;
    if (neighbors[1] == 1)  n |= 8;
    if (neighbors[10] == 1) n |= 4;
    if (neighbors[4] == 1)  n |= 2;
    return n;
}

uint8 SkeletonModel::indexOctantNEB(int neighbors[27]) {
    uint8 n;
    n = 1;
    if (neighbors[2] == 1)  n |= 128;
    if (neighbors[1] == 1)  n |= 64;
    if (neighbors[11] == 1) n |= 32;
    if (neighbors[10] == 1) n |= 16;
    if (neighbors[5] == 1)  n |= 8;
    if (neighbors[4] == 1)  n |= 4;
    if (neighbors[14] == 1) n |= 2;
    return n;
}

bool SkeletonModel::isSimplePoint(int neighbors[27]) {
    // copy neighbors for labeling
    int cube[26];
    for(int i = 0; i < 13; i++)
        cube[i] = neighbors[i];
    // i != 13 : ignore center pixel when counting
    for(int i = 14; i < 27; i++)
        cube[i-1] = neighbors[i];
    // set initial label
    int label = 2;
    // for all points in the neighborhood
    for(int i = 0; i < 26; i++) {
        if (cube[i] == 1) { // voxel has not been labeled yet
            // start recursion with any octant that contains the point i
            switch(i) {
                case 0:
                case 1:
                case 3:
                case 4:
                case 9:
                case 10:
                case 12:
                    octreeLabeling(1, label, cube);
                    break;
                case 2:
                case 5:
                case 11:
                case 13:
                    octreeLabeling(2, label, cube);
                    break;
                case 6:
                case 7:
                case 14:
                case 15:
                    octreeLabeling(3, label, cube);
                    break;
                case 8:
                case 16:
                    octreeLabeling(4, label, cube);
                    break;
                case 17:
                case 18:
                case 20:
                case 21:
                    octreeLabeling(5, label, cube);
                    break;
                case 19:
                case 22:
                    octreeLabeling(6, label, cube);
                    break;
                case 23:
                case 24:
                    octreeLabeling(7, label, cube);
                    break;
                case 25:
                    octreeLabeling(8, label, cube);
                    break;
            }
            label++;
            if (label-2 >= 2) return false;
        }
    }
    return true;
}

void SkeletonModel::octreeLabeling(int octant, int label, int cube[26]) {
    // check if there are points in the octant with value 1
    if (octant == 1) {
        // set points in this octant to current label
        // and recursive labeling of adjacent octants
        if (cube[0] == 1) cube[0] = label;
        if (cube[1] == 1) {
            cube[1] = label;
            octreeLabeling(2, label, cube);
        }
        if (cube[3] == 1) {
            cube[3] = label;
            octreeLabeling(3, label, cube);
        }
        if (cube[4] == 1) {
            cube[4] = label;
            octreeLabeling(2, label, cube);
            octreeLabeling(3, label, cube);
            octreeLabeling(4, label, cube);
        }
        if (cube[9] == 1) {
            cube[9] = label;
            octreeLabeling(5, label, cube);
        }
        if (cube[10] == 1) {
            cube[10] = label;
            octreeLabeling(2, label, cube);
            octreeLabeling(5, label, cube);
            octreeLabeling(6, label, cube);
        }
        if (cube[12] == 1) {
            cube[12] = label;
            octreeLabeling(3, label, cube);
            octreeLabeling(5, label, cube);
            octreeLabeling(7, label, cube);
        }
    }
    if (octant == 2) {
        if (cube[1] == 1) {
            cube[1] = label;
            octreeLabeling(1, label, cube);
        }
        if (cube[4] == 1) {
            cube[4] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(3, label, cube);
            octreeLabeling(4, label, cube);
        }
        if (cube[10] == 1) {
            cube[10] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(5, label, cube);
            octreeLabeling(6, label, cube);
        }
        if (cube[2] == 1) cube[2] = label;
        if (cube[5] == 1) {
            cube[5] = label;
            octreeLabeling(4, label, cube);
        }
        if (cube[11] == 1) {
            cube[11] = label;
            octreeLabeling(6, label, cube);
        }
        if (cube[13] == 1) {
            cube[13] = label;
            octreeLabeling(4, label, cube);
            octreeLabeling(6, label, cube);
            octreeLabeling(8, label, cube);
        }
    }
    if (octant == 3) {
        if (cube[3] == 1) {
            cube[3] = label;
            octreeLabeling(1, label, cube);
        }
        if (cube[4] == 1) {
            cube[4] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(2, label, cube);
            octreeLabeling(4, label, cube);
        }
        if (cube[12] == 1) {
            cube[12] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(5, label, cube);
            octreeLabeling(7, label, cube);
        }
        if (cube[6] == 1) cube[6] = label;
        if (cube[7] == 1) {
            cube[7] = label;
            octreeLabeling(4, label, cube);
        }
        if (cube[14] == 1) {
            cube[14] = label;
            octreeLabeling(7, label, cube);
        }
        if (cube[15] == 1) {
            cube[15] = label;
            octreeLabeling(4, label, cube);
            octreeLabeling(7, label, cube);
            octreeLabeling(8, label, cube);
        }
    }
    if (octant == 4) {
        if (cube[4] == 1) {
            cube[4] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(2, label, cube);
            octreeLabeling(3, label, cube);
        }
        if (cube[5] == 1 ) {
            cube[5] = label;
            octreeLabeling(2, label, cube);
        }
        if (cube[13] == 1) {
            cube[13] = label;
            octreeLabeling(2, label, cube);
            octreeLabeling(6, label, cube);
            octreeLabeling(8, label, cube);
        }
        if (cube[7] == 1) {
            cube[7] = label;
            octreeLabeling(3, label, cube);
        }
        if (cube[15] == 1) {
            cube[15] = label;
            octreeLabeling(3, label, cube);
            octreeLabeling(7, label, cube);
            octreeLabeling(8, label, cube);
        }
        if (cube[8] == 1) cube[8] = label;
        if (cube[16] == 1) {
            cube[16] = label;
            octreeLabeling(8, label, cube);
        }
    }
    if (octant == 5) {
        if (cube[9] == 1) {
            cube[9] = label;
            octreeLabeling(1, label, cube);
        }
        if (cube[10] == 1) {
            cube[10] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(2, label, cube);
            octreeLabeling(6, label, cube);
        }
        if (cube[12] == 1) {
            cube[12] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(3, label, cube);
            octreeLabeling(7, label, cube);
        }
        if (cube[17] == 1) cube[17] = label;
        if (cube[18] == 1) {
            cube[18] = label;
            octreeLabeling(6, label, cube);
        }
        if (cube[20] == 1) {
            cube[20] = label;
            octreeLabeling( 7, label, cube);
        }
        if (cube[21] == 1) {
            cube[21] = label;
            octreeLabeling(6, label, cube);
            octreeLabeling(7, label, cube);
            octreeLabeling(8, label, cube);
        }
    }
    if (octant == 6) {
        if (cube[10] == 1) {
            cube[10] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(2, label, cube);
            octreeLabeling(5, label, cube);
        }
        if (cube[11] == 1) {
            cube[11] = label;
            octreeLabeling(2, label, cube);
        }
        if (cube[13] == 1) {
            cube[13] = label;
            octreeLabeling(2, label, cube);
            octreeLabeling(4, label, cube);
            octreeLabeling(8, label, cube);
        }
        if( cube[18] == 1 ) {
            cube[18] = label;
            octreeLabeling(5, label, cube);
        }
        if( cube[21] == 1 ) {
            cube[21] = label;
            octreeLabeling(5, label, cube);
            octreeLabeling(7, label, cube);
            octreeLabeling(8, label, cube);
        }
        if( cube[19] == 1 ) cube[19] = label;
        if( cube[22] == 1 ) {
            cube[22] = label;
            octreeLabeling(8, label, cube);
        }
    }
    if( octant==7 ) {
        if( cube[12] == 1 )
        {
            cube[12] = label;
            octreeLabeling(1, label, cube);
            octreeLabeling(3, label, cube);
            octreeLabeling(5, label, cube);
        }
        if( cube[14] == 1)
        {
            cube[14] = label;
            octreeLabeling(3, label, cube);
        }
        if( cube[15] == 1 )
        {
            cube[15] = label;
            octreeLabeling(3, label, cube);
            octreeLabeling(4, label, cube);
            octreeLabeling(8, label, cube);
        }
        if( cube[20] == 1 )
        {
            cube[20] = label;
            octreeLabeling(5, label, cube);
        }
        if( cube[21] == 1 )
        {
            cube[21] = label;
            octreeLabeling(5, label, cube);
            octreeLabeling(6, label, cube);
            octreeLabeling(8, label, cube);
        }
        if( cube[23] == 1)
            cube[23] = label;
        if( cube[24] == 1)
        {
            cube[24] = label;
            octreeLabeling(8, label, cube);
        }
    }
    if( octant==8 ) {
        if( cube[13] == 1 )
        {
            cube[13] = label;
            octreeLabeling(2, label, cube);
            octreeLabeling(4, label, cube);
            octreeLabeling(6, label, cube);
        }
        if( cube[15] == 1 )
        {
            cube[15] = label;
            octreeLabeling(3, label, cube);
            octreeLabeling(4, label, cube);
            octreeLabeling(7, label, cube);
        }
        if( cube[16] == 1 )
        {
            cube[16] = label;
            octreeLabeling(4, label, cube);
        }
        if( cube[21] == 1 )
        {
            cube[21] = label;
            octreeLabeling(5, label, cube);
            octreeLabeling(6, label, cube);
            octreeLabeling(7, label, cube);
        }
        if( cube[22] == 1 )
        {
            cube[22] = label;
            octreeLabeling(6, label, cube);
        }
        if (cube[24] == 1) {
            cube[24] = label;
            octreeLabeling(7, label, cube);
        }
        if (cube[25] == 1) cube[25] = label;
    }
}

/***                        END SKELETONIZE3D (Fiji)                        ***/

void SkeletonModel::loadTIFFFile() {
    TIFF *file = TIFFOpen(filename.toStdString().c_str(), "r");

    uint32 width, height;

    TIFFGetField(file, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(file, TIFFTAG_IMAGELENGTH, &height);

    int slices = 0;
    do {
        slices++;
    } while (TIFFReadDirectory(file));

    TIFFClose(file);

    file = TIFFOpen(filename.toStdString().c_str(), "r");
    if (skeletonIm3D != NULL) delete skeletonIm3D;
    skeletonIm3D = new Image3D<short int>((int)width, (int)height, slices);

    int j = 0;
    do {
        uint32 size = (width*height);
        uint32 *tiff = (uint32 *)_TIFFmalloc(size*sizeof(uint32));
        int read = TIFFReadRGBAImage(file, width, height, tiff, 0);
        if (read) {
            for(uint32 i = 0; i < size; i++) {
                int r = TIFFGetR(*(tiff+i));
                int g = TIFFGetG(*(tiff+i));
                int b = TIFFGetB(*(tiff+i));
                if (r == 0 || g == 0 || b == 0) skeletonIm3D->at(j) = 0;
                else skeletonIm3D->at(j) = r;
                j++;
            }
        }
        _TIFFfree(tiff);
    } while (TIFFReadDirectory(file));

    TIFFClose(file);
}

void SkeletonModel::setDataFromFile() {
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)){

    }else{
        QTextStream fileStream(&file);
        while(!fileStream.atEnd()){
            QString line = fileStream.readLine();
            QStringList sp_line = line.split(" ");
            data.push_back(sp_line.at(0).toDouble());
            data.push_back(sp_line.at(1).toDouble());
            data.push_back(sp_line.at(2).toDouble());
        }

    }
}

void SkeletonModel::extractMinAndMaxFromData(double &xmin, double &xmax, double &ymin,
                                             double &ymax, double &zmin, double &zmax)
{
    xmin = xmax = data[0];
    ymin = ymax = data[1];
    zmin = zmax = data[2];

    for (unsigned long i = 3; i < data.size(); i+=3) {

        if(data[i]< xmin)
            xmin = data[i];
        if(data[i]> xmax)
            xmax = data[i];

        if(data[i+1]< ymin)
            ymin = data[i+1];
        if(data[i+1]> ymax)
            ymax = data[i+1];

        if(data[i+2]< zmin)
            zmin = data[i+2];
        if(data[i+2]> zmax)
            zmax = data[i+2];
    }
}

int SkeletonModel::normalize(double xmin, double xmax, double ymin,
                             double ymax, double zmin, double zmax)
{
    double delta_x = xmax - xmin;
    double delta_y = ymax - ymin;
    double delta_z = zmax - zmin;

    double delta = (delta_x > delta_y) ?
                ( (delta_x > delta_z) ? delta_x : delta_z):
                ( (delta_y > delta_z) ? delta_y : delta_z);

    for(unsigned long i = 0; i < data.size(); i+= 3){
        data[i] = (data[i]-xmin) / delta;
        data[i+1] = (data[i+1]-ymin) / delta;
        data[i+2] = (data[i+2]-zmin) / delta;
    }

    //std::cout << "max   : " << xmax <<" "<< ymax <<" "<< zmax << std::endl;
    //std::cout << "min   : " << xmin <<" "<< ymin <<" "<< zmin << std::endl;
    //std::cout << "delta : " << delta_x <<" "<< delta_y <<" "<< delta_z << std::endl;
    //std::cout << "D     : " << delta << std::endl;

    return (floor(delta));
}

void SkeletonModel::generate3DImFromData() {
    double xmin,xmax;
    double ymin,ymax;
    double zmin,zmax;

    extractMinAndMaxFromData(xmin,xmax,ymin,ymax,zmin,zmax);
    int delta = normalize(xmin,xmax,ymin,ymax,zmin,zmax);
    int size = floor((double) delta*1.0);
    int row, col, slice;
    row = col = slice = size;

    if(skeletonIm3D != NULL) delete skeletonIm3D;
    skeletonIm3D = new Image3D<short int>(row,col,slice);

    for(int x = 0; x < row; x++){
        for(int y = 0; y < col; y++){
            for(int z = 0; z < slice; z++){
                skeletonIm3D->at(x+(y*row)+(z*row*col)) = 0;
                boolMap.push_back(false);
                isNodeTab.push_back(false);
            }
        }
    }

    for(unsigned long i = 0; i < data.size(); i+= 3){

        int x = floor(data[i]*row);
        int y = floor(data[i+1]*col);
        int z = floor(data[i+2]*slice);

        skeletonIm3D->at(x+(y*row)+(z*row*slice)) = 255;

        //std::cout << x <<" "<< y <<" "<< z;
        //std::cout << " : " << data[i] <<" "<< data[i+1] <<" "<< data[i+2] << std::endl;
    }

    //std::cout << "data : " << data.size() << "/" << 372*3 << std::endl;
}

Image3D<short int>* SkeletonModel::getSkeleton3DIm(){
    Image3D<short int> im = *skeletonIm3D;
    return new Image3D<short int>(im);
}

void SkeletonModel::setSkeleton3DIm(Image *image) {
    if (skeletonIm3D != NULL) delete skeletonIm3D;
    this->skeletonIm3D = new Image3D<short int>(*image);
}
