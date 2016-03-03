#! /bin/bash

PATH_TO_IMAGEJ="../../ImageJ"

### Skeletonize 3D (Fiji)
wget "http://maven.imagej.net/service/local/repositories/snapshots/content/sc/fiji/Skeletonize3D_/2.0.1-SNAPSHOT/Skeletonize3D_-2.0.1-20160111.150444-1.jar"
cp Skeletonize3D_-2.0.1-20160111.150444-1.jar $PATH_TO_IMAGEJ/plugins/
rm Skeletonize3D_-2.0.1-20160111.150444-1.jar

### DGtal-0.9.1
wget "http://dgtal.org/releases/DGtal-0.9.1-Source.tar.gz"
tar xvf DGtal-0.9.1-Source.tar.gz
rm DGtal-0.9.1-Source.tar.gz
mkdir DGtal-0.9.1/build
cd DGtal-0.9.1/build
cmake ..
make -j 8
sudo make install
cd -

### DGtalTools
wget "https://codeload.github.com/DGtal-team/DGtalTools/zip/master"
unzip master
rm master
mkdir DGtalTools-master/build
cd DGtalTools-master/build
cmake ..
make -j 8
sudo make install
cd -

#/usr/local/include/DGtal/io/viewers/Viewer3D.h
