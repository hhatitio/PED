#ifndef ALGORITHMITKBINARYTHINNING3D
#define ALGORITHMITKBINARYTHINNING3D

#include "algorithmitk.h"

#include <itkBinaryThinningImageFilter3D.h>

#include <iostream>

template <typename InputImageType, typename OutputImageType>
class AlgorithmITKBinaryThinning3D: public AlgorithmITK<InputImageType, OutputImageType>
{
    // Dimension de l'image en entrée

    static const unsigned int Dimension = AlgorithmITK<InputImageType, InputImageType>::Dimension;

    // Définition des types en rapport avec l'image d'entrée

    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputPixelType       InputPixelType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputITKImageType    InputITKImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputImagePointer    InputImagePointer;

    // Définition des types en rapport avec l'image de sortie

    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputPixelType      OutputPixelType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputITKImageType   OutputITKImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputImagePointer   OutputImagePointer;

    // Définition des filtres

    typedef typename itk::BinaryThinningImageFilter3D<InputITKImageType, OutputITKImageType>                         Filter;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceFilterType                          SliceBySliceFilterType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceInputImageType                      SliceBySliceInputImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceOutputImageType                     SliceBySliceOutputImageType;
    typedef typename itk::BinaryThinningImageFilter3D<SliceBySliceInputImageType, SliceBySliceOutputImageType>       SliceBySliceFilter;

public:

    AlgorithmITKBinaryThinning3D();
    ~AlgorithmITKBinaryThinning3D();

    Algorithm<InputImageType, OutputImageType> * copy();

    void executeInPlace(InputImagePointer image);
    void executeSliceBySliceInPlace(InputImagePointer image);

    OutputImagePointer executeOutPlace(InputImagePointer image);
    OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image);

private:

    // Filtres

    typename Filter::Pointer filter;
    typename SliceBySliceFilterType::Pointer sliceFilter;
    typename SliceBySliceFilter::Pointer skeletonSliceFilter;
};

// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType, typename OutputImageType>
AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::AlgorithmITKBinaryThinning3D():
    AlgorithmITK<InputImageType, OutputImageType>("Squelettisation")
{
    filter = Filter::New();
    sliceFilter = SliceBySliceFilterType::New();
    skeletonSliceFilter = SliceBySliceFilter::New();
}

template <typename InputImageType, typename OutputImageType>
AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::~AlgorithmITKBinaryThinning3D()
{
    filter->Delete();
    sliceFilter->Delete();
    skeletonSliceFilter->Delete();
}

// ------------------- Copy ---------------------------

template <typename InputImageType, typename OutputImageType>
Algorithm<InputImageType, OutputImageType> * AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::copy()
{
    AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType> *copy = new AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>();

    return copy;
}

// ------------------- Fonctions d'exécution ---------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::executeInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    filter->SetInput(itkImage);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(skeletonSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::executeOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    filter->SetInput(itkImage);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKBinaryThinning3D<InputImageType, OutputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(skeletonSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

#endif // ALGORITHMITKBINARYTHINNING3D

