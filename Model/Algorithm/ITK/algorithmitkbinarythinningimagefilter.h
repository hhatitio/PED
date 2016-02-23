#ifndef ALGORITHMITKBINARYTHINNINGIMAGEFILTER
#define ALGORITHMITKBINARYTHINNINGIMAGEFILTER

#include "algorithmitk.h"

#include <itkBinaryThinningImageFilter.h>
#include <iostream>

template <typename InputImageType, typename OutputImageType>
class AlgorithmITKBinaryThinningImageFilter: public AlgorithmITK<InputImageType, OutputImageType>
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

    typedef typename itk::BinaryThinningImageFilter<InputITKImageType, OutputITKImageType>                         Filter;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceFilterType                          SliceBySliceFilterType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceInputImageType                      SliceBySliceInputImageType;
    typedef typename AlgorithmITK<InputImageType, InputImageType>::SliceBySliceOutputImageType                     SliceBySliceOutputImageType;
    typedef typename itk::BinaryThinningImageFilter<SliceBySliceInputImageType, SliceBySliceOutputImageType>       SliceBySliceFilter;

public:

    AlgorithmITKBinaryThinningImageFilter();
    ~AlgorithmITKBinaryThinningImageFilter();

    // Setter et Getter

    void setCoordinateTolerance(const double &coordinateTolerance);
    void setDirectionTolerance(const double &directionTolerance);

    double getCoordinateTolerance();
    double getDirectionTolerance();

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

    // Paramètres utiles aux filtres

    double coordinateTolerance;
    double directionTolerance;
};

// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType, typename OutputImageType>
AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::AlgorithmITKBinaryThinningImageFilter():
    AlgorithmITK<InputImageType, OutputImageType>("Squelettisation"), coordinateTolerance(5), directionTolerance(5)
{
    std::cout << "Model - New ..." << std::endl;

    filter = Filter::New();
    sliceFilter = SliceBySliceFilterType::New();
    skeletonSliceFilter = SliceBySliceFilter::New();
}

template <typename InputImageType, typename OutputImageType>
AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::~AlgorithmITKBinaryThinningImageFilter()
{
    std::cout << "Model - Delete ..." << std::endl;

    filter->Delete();
    sliceFilter->Delete();
    skeletonSliceFilter->Delete();
}

// ------------------- Getter et setter ---------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::setCoordinateTolerance(const double &coordinateTolerance) {
    this->coordinateTolerance = coordinateTolerance;
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::setDirectionTolerance(const double &directionTolerance) {
    this->directionTolerance = directionTolerance;
}

template <typename InputImageType, typename OutputImageType>
double AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::getCoordinateTolerance() {
    return this->coordinateTolerance;
}

template <typename InputImageType, typename OutputImageType>
double AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::getDirectionTolerance() {
    return this->directionTolerance;
}

// ------------------- Copy ---------------------------

template <typename InputImageType, typename OutputImageType>
Algorithm<InputImageType, OutputImageType> * AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::copy()
{
    AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType> *copy = new AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>();

    std::cout << "Model - Copy ..." << std::endl;

    copy->setCoordinateTolerance(coordinateTolerance);
    copy->setDirectionTolerance(directionTolerance);
    return copy;
}

// ------------------- Fonctions d'exécution ---------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::executeInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    std::cout << "Model - Execution : executeInPlace ..." << std::endl;
    /** TODO : exécution du filtre en mode InPlace **/

    filter->SetInput(itkImage);
    filter->SetCoordinateTolerance(coordinateTolerance);
    filter->SetDirectionTolerance(directionTolerance);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    std::cout << "Model - Execution : executeSliceBySliceInPlace ..." << std::endl;
    /** TODO : exécution du filtre en mode SliceBySlice InPlace **/

    skeletonSliceFilter->SetCoordinateTolerance(coordinateTolerance);
    skeletonSliceFilter->SetDirectionTolerance(directionTolerance);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(skeletonSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::executeOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    std::cout << "Model - Execution : executeOutPlace ..." << std::endl;
    /** TODO : exécution du filtre en mode OutPlace **/

    filter->SetInput(itkImage);
    filter->SetCoordinateTolerance(coordinateTolerance);
    filter->SetDirectionTolerance(directionTolerance);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKBinaryThinningImageFilter<InputImageType, OutputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    std::cout << "Model - Execution : executeSliceBySliceOutPlace ..." << std::endl;
    /** TODO : exécution du filtre en mode SliceBySlice OutPlace **/

    skeletonSliceFilter->SetCoordinateTolerance(coordinateTolerance);
    skeletonSliceFilter->SetDirectionTolerance(directionTolerance);
    sliceFilter->SetInput(itkImage);
    sliceFilter->SetFilter(skeletonSliceFilter);
    sliceFilter->GraftOutput(itkImage);
    sliceFilter->UpdateLargestPossibleRegion();
    sliceFilter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);
}

#endif // ALGORITHMITKBINARYTHINNINGIMAGEFILTER

