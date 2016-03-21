#ifndef ALGORITHMITKBINARYMORPHOLOGICALCLOSING_H
#define ALGORITHMITKBINARYMORPHOLOGICALCLOSING_H

#include "algorithmitk.h"
#include "../algorithmexception.h"

#include <itkFlatStructuringElement.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>

template <typename InputImageType, typename OutputImageType>
class AlgorithmITKBinaryMorphologicalClosing : public AlgorithmITK<InputImageType, OutputImageType>
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

    typedef itk::FlatStructuringElement<Dimension>                                                                  KernelType;
    typedef typename KernelType::RadiusType                                                                         KernelRadiusType;
    typedef typename itk::BinaryMorphologicalClosingImageFilter<InputITKImageType, OutputITKImageType, KernelType>  Filter;

public:

    AlgorithmITKBinaryMorphologicalClosing();
    ~AlgorithmITKBinaryMorphologicalClosing();

    void setKernel(const double *kernel);
    void setKernel(const KernelRadiusType &kernel);
    void setForegroundValue(const OutputPixelType &foregroundValue);

    KernelRadiusType getKernel();
    OutputPixelType getForegroundValue();

    Algorithm<InputImageType, OutputImageType> * copy();

    void executeInPlace(InputImagePointer image);
    void executeSliceBySliceInPlace(InputImagePointer image);

    OutputImagePointer executeOutPlace(InputImagePointer image);
    OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image);

private:

    typename Filter::Pointer filter;

    KernelType kernel;

    OutputPixelType foregroundValue;

};


// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType, typename OutputImageType>
AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::AlgorithmITKBinaryMorphologicalClosing() :
    AlgorithmITK<InputImageType, OutputImageType>("Fermeture morphologique binaire")
{
    filter = Filter::New();
}

template <typename InputImageType, typename OutputImageType>
AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::~AlgorithmITKBinaryMorphologicalClosing()
{
    filter->Delete();
}

// ------------------------- Getter et seter ---------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::setKernel(const double *kernel)
{
    KernelRadiusType kernelRadius;
    for (unsigned int dim=0; dim < Dimension; ++dim) {
        kernelRadius[dim] = kernel[dim];
    }
    this->kernel = KernelType::Ball(kernelRadius);
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::setKernel(const KernelRadiusType &kernelRadius)
{
    this->kernel = KernelType::Ball(kernelRadius);
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::setForegroundValue(const OutputPixelType &foregroundValue)
{
    this->foregroundValue = foregroundValue;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::KernelRadiusType
AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::getKernel()
{
    return kernel.GetRadius();
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::OutputPixelType
AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::getForegroundValue()
{
    return foregroundValue;
}

// ------------------------------- Copy ---------------------------------------


template <typename InputImageType, typename OutputImageType>
Algorithm<InputImageType, OutputImageType> * AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::copy()
{
    AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType> *copy =
            new AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>();
    copy->setKernel(kernel.GetRadius());
    copy->setForegroundValue(foregroundValue);
    return copy;
}

// ---------------------- Fonctions d'éxecution ------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::executeInPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, false);

    filter->SetInput(itkImage);
    filter->SetKernel(kernel);
    filter->SetForegroundValue(foregroundValue);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
template <typename InputImageType, typename OutputImageType>
void AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    throw AlgorithmException(MESSAGE_NO_SLICE_BY_SLICE_INPLACE_FUNCTION_EXCEPTION);
}
#pragma GCC diagnostic pop

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::executeOutPlace(InputImagePointer image)
{
    typename InputITKImageType::Pointer itkImage = ITKTools<PixelType>::convertInItkImage(image, true);

    filter->SetInput(itkImage);
    filter->SetKernel(kernel);
    filter->SetForegroundValue(foregroundValue);
    filter->GraftOutput(itkImage);
    filter->UpdateLargestPossibleRegion();
    filter->Update();

    return ITKTools<PixelType>::convertInImage(itkImage, false);

    return image;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmITKBinaryMorphologicalClosing<InputImageType, OutputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    throw AlgorithmException(MESSAGE_NO_SLICE_BY_SLICE_OUTPLACE_FUNCTION_EXCEPTION);
    return image;
}

#endif // ALGORITHMITKBINARYMORPHOLOGICALCLOSING_H
