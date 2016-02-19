#ifndef ALGORITHMITKCONNECTEDCOMPONENT_H
#define ALGORITHMITKCONNECTEDCOMPONENT_H

#include "algorithmitk.h"

#include <itkConnectedComponentImageFilter.h>

template <typename InputImageType, typename OutputImageType>
class AlgorithmITKConnectedComponent : public AlgorithmITK<InputImageType, OutputImageType>
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

    typedef typename itk::ConnectedComponentImageFilter<InputITKImageType, OutputITKImageType>                      Filter;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceFilterType                           SliceBySliceFilterType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceInputImageType                       SliceBySliceInputImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceOutputImageType                      SliceBySliceOutputImageType;
    typedef typename itk::ConnectedComponentImageFilter<SliceBySliceInputImageType, SliceBySliceOutputImageType>    SliceBySliceFilter;

public:

    AlgorithmITKConnectedComponent();
    ~AlgorithmITKConnectedComponent();

    void setFullyConnected(const bool &fullyConnected);

    bool getFullyConnected();
    int getObjectCount();

    Algorithm<InputImageType, OutputImageType> * copy();

    void executeInPlace(InputImagePointer image);
    void executeSliceBySliceInPlace(InputImagePointer image);

    OutputImagePointer executeOutPlace(InputImagePointer image);
    OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image);

private:

    typename Filter::Pointer filter;
    typename SliceBySliceFilterType::Pointer sliceFilter = SliceBySliceFilterType::New();
    typename SliceBySliceFilter::Pointer connectedComponentSliceFilter = SliceBySliceFilter::New();

    bool fullyConnected;

};


// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType, typename OutputImageType>
AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::AlgorithmITKConnectedComponent() :
    AlgorithmITK<InputImageType, OutputImageType>("Composantes connexes"), fullyConnected(false)
{
    filter = Filter::New();
    sliceFilter = SliceBySliceFilterType::New();
    connectedComponentSliceFilter = SliceBySliceFilter::New();
}

template <typename InputImageType, typename OutputImageType>
AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::~AlgorithmITKConnectedComponent()
{
    filter->Delete();
    sliceFilter->Delete();
    connectedComponentSliceFilter->Delete();
}

// ------------------------- Getter et seter ---------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::setFullyConnected(const bool &fullyConnected)
{
    this->fullyConnected = fullyConnected;
}

template <typename InputImageType, typename OutputImageType>
bool AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::getFullyConnected()
{
    return fullyConnected;
}

template <typename InputImageType, typename OutputImageType>
int AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::getObjectCount()
{
    return filter->GetObjectCount();
}

// ------------------------------- Copy ---------------------------------------


template <typename InputImageType, typename OutputImageType>
Algorithm<InputImageType, OutputImageType> * AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::copy()
{
    AlgorithmITKConnectedComponent<InputImageType, OutputImageType> *copy = new AlgorithmITKConnectedComponent<InputImageType, OutputImageType>();
    copy->setFullyConnected(fullyConnected);
    return copy;
}

// ---------------------- Fonctions d'éxecution ------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::executeInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    filter->SetInput(itkImage);
    filter->SetFullyConnected(fullyConnected);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    connectedComponentSliceFilter->SetFullyConnected(fullyConnected);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(connectedComponentSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::executeOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    filter->SetInput(itkImage);
    filter->SetFullyConnected(fullyConnected);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKConnectedComponent<InputImageType, OutputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    connectedComponentSliceFilter->SetFullyConnected(fullyConnected);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(connectedComponentSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

#endif // ALGORITHMITKCONNECTEDCOMPONENT_H
