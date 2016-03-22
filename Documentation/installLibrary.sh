#! /bin/bash

### Armadillo v. 6.200.4
wget "http://downloads.sourceforge.net/project/arma/armadillo-6.200.4.tar.gz"
tar xvf armadillo-6.200.4.tar.gz
cd armadillo-6.200.4/
./configure
make
sudo make install
cd -

### InsightToolkit v. 4.8.2
wget "http://sourceforge.net/projects/itk/files/itk/4.8/InsightToolkit-4.8.2.tar.gz"
tar xvf InsightToolkit-4.8.2.tar.gz
mkdir InsightToolkit-4.8.2/build
cd InsightToolkit-4.8.2/build
cmake .. -DCMAKE_CXX_FLAGS="-std=c++11" -DCMAKE_BUILD_TYPE="Release" -DBUILD_EXAMPLES=false -DBUILD_TESTING=false -DITK_BUILD_DEFAULT_MODULES=false -DITKGroup_Core=true -DITKGroup_IO=true -DITKGroup_Numerics=true
make
sudo make install
cd -

### DGtal 0.9
wget "http://dgtal.org/releases/DGtal-0.9-Source.tar.gz"
tar xvf DGtal-0.9-Source.tar.gz
mkdir DGtal-0.9/build
cd DGtal-0.9/build
cmake ..
make
sudo make install
cd -

## libQGLViewer
sudo apt-get install libqglviewer-dev
