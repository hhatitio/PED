#include "skeletonmodel.h"

SkeletonModel::SkeletonModel() {

}

void SkeletonModel::setFilename(QString filename) {
    this->filename = filename;
}

void SkeletonModel::compute() {
    /*std::cout << "Execute process" << std::endl;
    QString cmdImageJ(PATH_TO_IMAGEJ);
    cmdImageJ += " \""+filename+"\""+MAKE_BINARY+SKELETONIZE+SAVE_AS;
    std::cout << "cmdImageJ = " << cmdImageJ.toStdString() << std::endl;
    QString cmdRaw2Vol("raw2vol");
    QProcess::execute(cmdImageJ);*/

    int width = this->skeletonIm3D->n_rows;
    int height = this->skeletonIm3D->n_cols;
    int depth = this->skeletonIm3D->n_slices;

    Image3D<short int> *skeletonImTmp = this->skeletonIm3D;

}

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

    skeletonIm3D = new Image3D<short int>((int)width, (int)height, slices);

    int j = 0;
    do {
        uint32 size = (width*height);
        uint32 *tiff = (uint32 *)_TIFFmalloc(size*sizeof(uint32));
        int read = TIFFReadRGBAImage(file, width, height, tiff, 0);
        if (read) {
            for(uint32 i = 0; i < size/4; i++) {
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

    std::cout << "max   : " << xmax <<" "<< ymax <<" "<< zmax << std::endl;
    std::cout << "min   : " << xmin <<" "<< ymin <<" "<< zmin << std::endl;
    std::cout << "delta : " << delta_x <<" "<< delta_y <<" "<< delta_z << std::endl;
    std::cout << "D     : " << delta << std::endl;

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

        std::cout << x <<" "<< y <<" "<< z;
        std::cout << " : " << data[i] <<" "<< data[i+1] <<" "<< data[i+2] << std::endl;
    }

    std::cout << "data : " << data.size() << "/" << 372*3 << std::endl;
}

Image3D<short int>* SkeletonModel::getSkeleton3DIm(){
    Image3D<short int> im = *skeletonIm3D;
    return new Image3D<short int>(im);
}

void SkeletonModel::setSkeleton3DIm(Image *image) {
    this->skeletonIm3D = image;
}
