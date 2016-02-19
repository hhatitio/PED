#ifndef DGTALTOOLS_H
#define DGTALTOOLS_H

#include "../deftypes.h"
#include "itktools.h"

#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>

template <typename PixelType> class DGtalTools
{

public:

    // DGtalITKImage

    static DGtalITKImage convertInDGtalITKImage(Image *image, bool copy);
    static DGtalITKImage convertInDGtalITKImage(const typename ITKImage::Pointer &itkImage);
    static Image * convertInImage(const DGtalITKImage &dgtalITKImage, bool copy);

    static Image * loadImage3D(const std::string &filename);
    static void saveImage3D(const std::string &filename, Image *image);

};

template <typename PixelType> DGtalITKImage DGtalTools<PixelType>::convertInDGtalITKImage(Image *image, bool copy)
{
    ITKImage::Pointer itkImage;

    if (copy)
        itkImage = ITKTools<PixelType>::copyITKImage(ITKTools<PixelType>::convertInItkImage(image, false));
    else
        itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    return convertInDGtalITKImage(itkImage);
}

template <typename PixelType> DGtalITKImage DGtalTools<PixelType>::convertInDGtalITKImage(const typename ITKImage::Pointer &itkImage)
{
    typename DGtalITKImage::ITKImagePointer itkImagePointer = itkImage;
    return DGtalITKImage(itkImagePointer);
}

template <typename PixelType> Image * DGtalTools<PixelType>::convertInImage(const DGtalITKImage &dgtalITKImage, bool copy)
{
    return ITKTools<PixelType>::convertInImage(dgtalITKImage.getITKImagePointer(), copy);
}

template <typename PixelType> Image * DGtalTools<PixelType>::loadImage3D(const std::string &filename)
{
    DGtalITKImage dgtalITKImage = DGtal::GenericReader<DGtalITKImage>::import(filename);
    return ITKTools<PixelType>::convertInImage(dgtalITKImage.getITKImagePointer(), false);
}

template <typename PixelType> void DGtalTools<PixelType>::saveImage3D(const std::string &filename, Image *image)
{
    DGtalITKImage dgtalITKImage = convertInDGtalITKImage(image, false);

    typedef DGtal::ImageContainerBySTLVector<DGtal::Z3i::Domain, unsigned char> DGtalSTLVectorImage;

    DGtalSTLVectorImage dgtalSTLVectorImage (dgtalITKImage.domain());
    typename DGtalSTLVectorImage::Domain::ConstIterator itBegin = dgtalSTLVectorImage.domain().begin();
    typename DGtalSTLVectorImage::Domain::ConstIterator itEnd = dgtalSTLVectorImage.domain().end();
    typename DGtalITKImage::Domain::ConstIterator itValue = dgtalITKImage.domain().begin();

    for (typename DGtalSTLVectorImage::Domain::ConstIterator it=itBegin; it!=itEnd; ++it, ++itValue)
        dgtalSTLVectorImage.setValue(*it, dgtalITKImage(*itValue));

    dgtalSTLVectorImage >> filename;
}

#endif // DGTALTOOLS_H
