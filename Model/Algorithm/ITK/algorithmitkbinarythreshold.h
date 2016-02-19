#ifndef ALGORITHMITKBINARYTHRESHOLD_H
#define ALGORITHMITKBINARYTHRESHOLD_H

#include "algorithmitk.h"

#include <itkBinaryThresholdImageFilter.h>

template <typename InputImageType, typename OutputImageType>
class AlgorithmITKBinaryThreshold : public AlgorithmITK<InputImageType, OutputImageType>
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

    typedef typename itk::BinaryThresholdImageFilter<InputITKImageType, OutputITKImageType>                     Filter;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceFilterType                       SliceBySliceFilterType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceInputImageType                   SliceBySliceInputImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceOutputImageType                  SliceBySliceOutputImageType;
    typedef typename itk::BinaryThresholdImageFilter<SliceBySliceInputImageType, SliceBySliceOutputImageType>   SliceBySliceFilter;

public:

    AlgorithmITKBinaryThreshold();
    ~AlgorithmITKBinaryThreshold();

    void setLower(const double &lower);
    void setUpper(const double &upper);
    void setInsideValue(const OutputPixelType &insideValue);
    void setOutsideValue(const OutputPixelType &outsideValue);

    InputPixelType getLower();
    InputPixelType getUpper();
    OutputPixelType getInsideValue();
    OutputPixelType getOutsideValue();

    Algorithm<InputImageType, OutputImageType> * copy();

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

    OutputPixelType insideValue;
    OutputPixelType outsideValue;

};


// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType, typename OutputImageType>
AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::AlgorithmITKBinaryThreshold() :
    AlgorithmITK<InputImageType, OutputImageType>("Seuillage binaire"), lower(0), upper(0)
{
    filter = Filter::New();
    sliceFilter = SliceBySliceFilterType::New();
    thresholdSliceFilter = SliceBySliceFilter::New();
}

template <typename InputImageType, typename OutputImageType>
AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::~AlgorithmITKBinaryThreshold()
{
    filter->Delete();
    sliceFilter->Delete();
    thresholdSliceFilter->Delete();
}

// ------------------------- Getter et seter ---------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::setLower(const double &lower)
{
    this->lower = lower;
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::setUpper(const double &upper)
{
    this->upper = upper;
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::setInsideValue(const OutputPixelType &insideValue)
{
    this->insideValue = insideValue;
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::setOutsideValue(const OutputPixelType &outsideValue)
{
    this->outsideValue = outsideValue;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::InputPixelType
AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::getLower()
{
    return lower;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::InputPixelType
AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::getUpper()
{
    return upper;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::OutputPixelType
AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::getInsideValue()
{
    return insideValue;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::OutputPixelType
AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::getOutsideValue()
{
    return outsideValue;
}

// ------------------------------- Copy ---------------------------------------


template <typename InputImageType, typename OutputImageType>
Algorithm<InputImageType, OutputImageType> * AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::copy()
{
    AlgorithmITKBinaryThreshold<InputImageType, OutputImageType> *copy = new AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>();
    copy->setLower(lower);
    copy->setUpper(upper);
    copy->setInsideValue(insideValue);
    copy->setOutsideValue(outsideValue);
    return copy;
}

// ---------------------- Fonctions d'éxecution ------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::executeInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    filter->SetInput(itkImage);
    filter->SetLowerThreshold(lower);
    filter->SetUpperThreshold(upper);
    filter->SetInsideValue(insideValue);
    filter->SetOutsideValue(outsideValue);
    filter->InPlaceOn();
    filter->UpdateLargestPossibleRegion();
    filter->Update();
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    thresholdSliceFilter->SetLowerThreshold(lower);
    thresholdSliceFilter->SetUpperThreshold(upper);
    thresholdSliceFilter->SetInsideValue(insideValue);
    thresholdSliceFilter->SetOutsideValue(outsideValue);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(thresholdSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::executeOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    filter->SetInput(itkImage);
    filter->SetLowerThreshold(lower);
    filter->SetUpperThreshold(upper);
    filter->SetInsideValue(insideValue);
    filter->SetOutsideValue(outsideValue);
    filter->GraftOutput(itkImage);
    filter->InPlaceOff();
    filter->UpdateLargestPossibleRegion();
    filter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKBinaryThreshold<InputImageType, OutputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    thresholdSliceFilter->SetLowerThreshold(lower);
    thresholdSliceFilter->SetUpperThreshold(upper);
    thresholdSliceFilter->SetInsideValue(insideValue);
    thresholdSliceFilter->SetOutsideValue(outsideValue);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(thresholdSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

#endif // ALGORITHMITKBINARYTHRESHOLD_H
