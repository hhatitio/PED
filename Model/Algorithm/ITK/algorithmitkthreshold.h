#ifndef ALGORITHMITKTHRESHOLD_H
#define ALGORITHMITKTHRESHOLD_H

#include "algorithmitk.h"

#include <itkThresholdImageFilter.h>

template <typename InputImageType>
class AlgorithmITKThreshold : public AlgorithmITK<InputImageType, InputImageType>
{
    // Dimension de l'image en entrée

    static const unsigned int Dimension = AlgorithmITK<InputImageType, InputImageType>::Dimension;

    // Défintion des types en rapport avec l'image d'entrée

    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputPixelType       InputPixelType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputITKImageType    InputITKImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::InputImagePointer    InputImagePointer;

    // Défintion des types en rapport avec l'image de sortie

    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputPixelType      OutputPixelType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputImagePointer   OutputImagePointer;

    // Définition des filtres

    typedef typename itk::ThresholdImageFilter<InputITKImageType>                               Filter;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceFilterType       SliceBySliceFilterType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceInputImageType   SliceBySliceInputImageType;
    typedef typename itk::ThresholdImageFilter<SliceBySliceInputImageType>                      SliceBySliceFilter;

public:

    AlgorithmITKThreshold();
    ~AlgorithmITKThreshold();

    void setLower(const double &lower);
    void setUpper(const double &upper);
    void setOutsideValue(const OutputPixelType &outsideValue);

    InputPixelType getLower();
    InputPixelType getUpper();
    OutputPixelType getOutsideValue();

    Algorithm<InputImageType, InputImageType> * copy();

    void executeInPlace(InputImagePointer image);
    void executeSliceBySliceInPlace(InputImagePointer image);

    OutputImagePointer executeOutPlace(InputImagePointer image);
    OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image);

private:

    typename Filter::Pointer filter;
    typename SliceBySliceFilterType::Pointer sliceFilter;
    typename SliceBySliceFilter::Pointer thresholdSliceFilter;

    InputPixelType lower;
    InputPixelType upper;
    OutputPixelType outsideValue;

};


// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType>
AlgorithmITKThreshold<InputImageType>::AlgorithmITKThreshold() :
    AlgorithmITK<InputImageType, InputImageType>("Seuillage"), lower(0), upper(0)
{
    filter = Filter::New();
    sliceFilter = SliceBySliceFilterType::New();
    thresholdSliceFilter = SliceBySliceFilter::New();
}

template <typename InputImageType>
AlgorithmITKThreshold<InputImageType>::~AlgorithmITKThreshold()
{
    filter->Delete();
    sliceFilter->Delete();
    thresholdSliceFilter->Delete();
}

// ------------------------- Getter et seter ---------------------------------

template <typename InputImageType>
void AlgorithmITKThreshold<InputImageType>::setLower(const double &lower)
{
    this->lower = lower;
}

template <typename InputImageType>
void AlgorithmITKThreshold<InputImageType>::setUpper(const double &upper)
{
    this->upper = upper;
}

template <typename InputImageType>
void AlgorithmITKThreshold<InputImageType>::setOutsideValue(const OutputPixelType &outsideValue)
{
    this->outsideValue = outsideValue;
}

template <typename InputImageType>
typename AlgorithmITKThreshold<InputImageType>::InputPixelType
AlgorithmITKThreshold<InputImageType>::getLower()
{
    return lower;
}

template <typename InputImageType>
typename AlgorithmITKThreshold<InputImageType>::InputPixelType
AlgorithmITKThreshold<InputImageType>::getUpper()
{
    return upper;
}

template <typename InputImageType>
typename AlgorithmITKThreshold<InputImageType>::OutputPixelType
AlgorithmITKThreshold<InputImageType>::getOutsideValue()
{
    return outsideValue;
}

// ------------------------------- Copy ---------------------------------------


template <typename InputImageType>
Algorithm<InputImageType, InputImageType> * AlgorithmITKThreshold<InputImageType>::copy()
{
    AlgorithmITKThreshold<InputImageType> *copy = new AlgorithmITKThreshold<InputImageType>();
    copy->setLower(lower);
    copy->setUpper(upper);
    copy->setOutsideValue(outsideValue);
    return copy;
}

// ---------------------- Fonctions d'éxecution ------------------------------

template <typename InputImageType>
void AlgorithmITKThreshold<InputImageType>::executeInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    filter->SetInput(itkImage);
    filter->SetLower(lower);
    filter->SetUpper(upper);
    filter->SetOutsideValue(outsideValue);
    filter->InPlaceOn();
    filter->UpdateLargestPossibleRegion();
    filter->Update();
}

template <typename InputImageType>
void AlgorithmITKThreshold<InputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    thresholdSliceFilter->SetLower(lower);
    thresholdSliceFilter->SetUpper(upper);
    thresholdSliceFilter->SetOutsideValue(outsideValue);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(thresholdSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();
}

template <typename InputImageType>
typename AlgorithmITKThreshold<InputImageType>::OutputImagePointer
AlgorithmITKThreshold<InputImageType>::executeOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    filter->SetInput(itkImage);
    filter->SetLower(lower);
    filter->SetUpper(upper);
    filter->SetOutsideValue(outsideValue);
    filter->InPlaceOff();
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

template <typename InputImageType>
typename AlgorithmITKThreshold<InputImageType>::OutputImagePointer
AlgorithmITKThreshold<InputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    thresholdSliceFilter->SetLower(lower);
    thresholdSliceFilter->SetUpper(upper);
    thresholdSliceFilter->SetOutsideValue(outsideValue);
    thresholdSliceFilter->InPlaceOff();
    sliceFilter->SetInput(itkImage);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->SetFilter(thresholdSliceFilter);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

#endif // ALGORITHMITKTHRESHOLD_H
