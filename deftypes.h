#ifndef DEFTYPES_H
#define DEFTYPES_H

#include "Model/image3D.h"

#include <itkImage.h>

#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerByITKImage.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

#define VOXEL_VOLM 1
#define VOXEL_SKEL 2
#define VOXEL_NODE 3

const unsigned int Dimension = 3;

typedef short PixelType;

typedef Image3D<PixelType>                                                  Image;
typedef typename itk::Image<PixelType, Dimension>                           ITKImage;
typedef typename itk::Image<PixelType, 2>                                   ITKImage2D;
typedef DGtal::ImageContainerByITKImage<DGtal::Z3i::Domain, PixelType>      DGtalITKImage;
typedef DGtal::ImageContainerByITKImage<DGtal::Z2i::Domain, PixelType>      DGtalITKImage2D;

#endif // DEFTYPES_H
