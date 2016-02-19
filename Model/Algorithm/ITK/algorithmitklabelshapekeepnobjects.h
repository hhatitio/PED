#ifndef ALGORITHMITKLABELSHAPEKEEPNOBJECTS_H
#define ALGORITHMITKLABELSHAPEKEEPNOBJECTS_H

#include "algorithmitk.h"

#include <itkLabelShapeKeepNObjectsImageFilter.h>

template <typename InputImageType>
class AlgorithmITKLabelShapeKeepNObjects : public AlgorithmITK<InputImageType, InputImageType>
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

    typedef typename itk::LabelShapeKeepNObjectsImageFilter<InputITKImageType>                  Filter;
    typedef typename Filter::AttributeType                                                      AttributeFilterType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceFilterType       SliceBySliceFilterType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceInputImageType   SliceBySliceInputImageType;
    typedef typename itk::LabelShapeKeepNObjectsImageFilter<SliceBySliceInputImageType>         SliceBySliceFilter;

public:

    // Label utiliser pour les attributs
    typedef typename Filter::LabelObjectType LabelObjectFilterType;

    AlgorithmITKLabelShapeKeepNObjects();
    ~AlgorithmITKLabelShapeKeepNObjects();

    void setBackgroundValue(const OutputPixelType &backgroundValue);
    void setNumberOfObjects(const unsigned int &numberOfObjects);
    void setAttribute(const AttributeFilterType &attribute);

    OutputPixelType getBackgroundValue();
    unsigned int getNumberOfObjects();
    AttributeFilterType getAttribute();

    Algorithm<InputImageType, InputImageType> * copy();

    void executeInPlace(InputImagePointer image);
    void executeSliceBySliceInPlace(InputImagePointer image);

    OutputImagePointer executeOutPlace(InputImagePointer image);
    OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image);

private:

    typename Filter::Pointer filter;
    typename SliceBySliceFilterType::Pointer sliceFilter;
    typename SliceBySliceFilter::Pointer labelShapeSliceFilter;

    OutputPixelType backgroundValue;
    unsigned int numberOfObjects;
    AttributeFilterType attribute;

};


// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType>
AlgorithmITKLabelShapeKeepNObjects<InputImageType>::AlgorithmITKLabelShapeKeepNObjects() :
    AlgorithmITK<InputImageType, InputImageType>("Sélection de composantes connexes")
{
    filter = Filter::New();
    sliceFilter = SliceBySliceFilterType::New();
    labelShapeSliceFilter = SliceBySliceFilter::New();
}

template <typename InputImageType>
AlgorithmITKLabelShapeKeepNObjects<InputImageType>::~AlgorithmITKLabelShapeKeepNObjects()
{
    filter->Delete();
    sliceFilter->Delete();
    labelShapeSliceFilter->Delete();
}

// ------------------------- Getter et seter ---------------------------------

template <typename InputImageType>
void AlgorithmITKLabelShapeKeepNObjects<InputImageType>::setBackgroundValue(const OutputPixelType &backgroundValue)
{
    this->backgroundValue = backgroundValue;
}

template <typename InputImageType>
void AlgorithmITKLabelShapeKeepNObjects<InputImageType>::setNumberOfObjects(const unsigned int &numberOfObjects)
{
    this->numberOfObjects = numberOfObjects;
}

template <typename InputImageType>
void AlgorithmITKLabelShapeKeepNObjects<InputImageType>::setAttribute(const AttributeFilterType &attribute)
{
    this->attribute = attribute;
}

template <typename InputImageType>
typename AlgorithmITKLabelShapeKeepNObjects<InputImageType>::OutputPixelType
AlgorithmITKLabelShapeKeepNObjects<InputImageType>::getBackgroundValue()
{
    return backgroundValue;
}

template <typename InputImageType>
unsigned int AlgorithmITKLabelShapeKeepNObjects<InputImageType>::getNumberOfObjects()
{
    return numberOfObjects;
}

template <typename InputImageType>
typename AlgorithmITKLabelShapeKeepNObjects<InputImageType>::AttributeFilterType
AlgorithmITKLabelShapeKeepNObjects<InputImageType>::getAttribute()
{
    return attribute;
}

// ------------------------------- Copy ---------------------------------------


template <typename InputImageType>
Algorithm<InputImageType, InputImageType> * AlgorithmITKLabelShapeKeepNObjects<InputImageType>::copy()
{
    AlgorithmITKLabelShapeKeepNObjects<InputImageType> *copy = new AlgorithmITKLabelShapeKeepNObjects<InputImageType>();
    copy->setBackgroundValue(backgroundValue);
    copy->setNumberOfObjects(numberOfObjects);
    copy->setAttribute(attribute);
    return copy;
}

// ---------------------- Fonctions d'éxecution ------------------------------

template <typename InputImageType>
void AlgorithmITKLabelShapeKeepNObjects<InputImageType>::executeInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    filter->SetInput(itkImage);
    filter->SetBackgroundValue(backgroundValue);
    filter->SetNumberOfObjects(numberOfObjects);
    filter->SetAttribute(attribute);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();
}

template <typename InputImageType>
void AlgorithmITKLabelShapeKeepNObjects<InputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    labelShapeSliceFilter->SetBackgroundValue(backgroundValue);
    labelShapeSliceFilter->SetNumberOfObjects(numberOfObjects);
    labelShapeSliceFilter->SetAttribute(attribute);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(labelShapeSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();
}

template <typename InputImageType>
typename AlgorithmITKLabelShapeKeepNObjects<InputImageType>::OutputImagePointer
AlgorithmITKLabelShapeKeepNObjects<InputImageType>::executeOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    filter->SetInput(itkImage);
    filter->SetBackgroundValue(backgroundValue);
    filter->SetNumberOfObjects(numberOfObjects);
    filter->SetAttribute(attribute);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

template <typename InputImageType>
typename AlgorithmITKLabelShapeKeepNObjects<InputImageType>::OutputImagePointer
AlgorithmITKLabelShapeKeepNObjects<InputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    labelShapeSliceFilter->SetBackgroundValue(backgroundValue);
    labelShapeSliceFilter->SetNumberOfObjects(numberOfObjects);
    labelShapeSliceFilter->SetAttribute(attribute);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(labelShapeSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

#endif // ALGORITHMITKLABELSHAPEKEEPNOBJECTS_H
