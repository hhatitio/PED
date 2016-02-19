#ifndef ALGORITHMITKDISCRETEGAUSSIAN_H
#define ALGORITHMITKDISCRETEGAUSSIAN_H

#include "algorithmitk.h"

#include <itkDiscreteGaussianImageFilter.h>

template <typename InputImageType, typename OutputImageType>
class AlgorithmITKDiscreteGaussian : public AlgorithmITK<InputImageType, OutputImageType>
{
    // Dimension de l'image en entrée

    static const unsigned int Dimension = AlgorithmITK<InputImageType, InputImageType>::Dimension;

    // Défintion des types en rapport avec l'image d'entrée

    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputPixelType       InputPixelType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputITKImageType    InputITKImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputImagePointer    InputImagePointer;

    // Défintion des types en rapport avec l'image de sortie

    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputPixelType      OutputPixelType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputITKImageType   OutputITKImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputImagePointer   OutputImagePointer;

    // Définition des filtres

    typedef typename itk::DiscreteGaussianImageFilter<InputITKImageType, OutputITKImageType>                    Filter;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceFilterType                       SliceBySliceFilterType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceInputImageType                   SliceBySliceInputImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceOutputImageType                  SliceBySliceOutputImageType;
    typedef typename itk::DiscreteGaussianImageFilter<SliceBySliceInputImageType, SliceBySliceOutputImageType>  SliceBySliceFilter;

public:

    AlgorithmITKDiscreteGaussian();
    ~AlgorithmITKDiscreteGaussian();

    void setVariance(const double &variance);
    void setDimensionality(const int &dimensionality);

    double getVariance();
    int getDimensionality();

    Algorithm<InputImageType, OutputImageType> * copy();

    void executeInPlace(InputImagePointer image);
    void executeSliceBySliceInPlace(InputImagePointer image);

    OutputImagePointer executeOutPlace(InputImagePointer image);
    OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image);

private:

    typename Filter::Pointer filter;
    typename SliceBySliceFilterType::Pointer sliceFilter;
    typename SliceBySliceFilter::Pointer discreteGaussianSliceFilter;

    double variance;
    int dimensionality;

};


// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType, typename OutputImageType>
AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::AlgorithmITKDiscreteGaussian() :
    AlgorithmITK<InputImageType, OutputImageType>("Gaussien discret"), variance(0), dimensionality(Dimension)
{
    filter = Filter::New();
    sliceFilter = SliceBySliceFilterType::New();
    discreteGaussianSliceFilter = SliceBySliceFilter::New();
}

template <typename InputImageType, typename OutputImageType>
AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::~AlgorithmITKDiscreteGaussian()
{
    filter->Delete();
    sliceFilter->Delete();
    discreteGaussianSliceFilter->Delete();
}

// ------------------------- Getter et seter ---------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::setVariance(const double &variance)
{
    if (variance > 0)
        this->variance = variance;
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::setDimensionality(const int &dimensionality)
{
    if (dimensionality > 0 && dimensionality <= Dimension)
        this->dimensionality = dimensionality;
}

template <typename InputImageType, typename OutputImageType>
double AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::getVariance()
{
    return variance;
}

template <typename InputImageType, typename OutputImageType>
int AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::getDimensionality()
{
    return dimensionality;
}

// ------------------------------- Copy ---------------------------------------


template <typename InputImageType, typename OutputImageType>
Algorithm<InputImageType, OutputImageType> * AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::copy()
{
    AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType> *copy = new AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>();
    copy->setVariance(variance);
    copy->setDimensionality(dimensionality);
    return copy;
}

// ---------------------- Fonctions d'éxecution ------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::executeInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    filter->SetInput(itkImage);
    filter->SetVariance(variance);
    filter->SetFilterDimensionality(dimensionality);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    discreteGaussianSliceFilter->SetVariance(variance);
    discreteGaussianSliceFilter->SetFilterDimensionality(dimensionality);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(discreteGaussianSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::executeOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    filter->SetInput(itkImage);
    filter->SetVariance(variance);
    filter->SetFilterDimensionality(dimensionality);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKDiscreteGaussian<InputImageType, OutputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    discreteGaussianSliceFilter->SetVariance(variance);
    discreteGaussianSliceFilter->SetFilterDimensionality(dimensionality);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(discreteGaussianSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

#endif // ALGORITHMITKDISCRETEGAUSSIAN_H
