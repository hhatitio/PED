#ifndef ALGORITHMITK_H
#define ALGORITHMITK_H

#include "../algorithm.h"

#include "../../itktools.h"

#include <itkSliceBySliceImageFilter.h>

template <typename InputImageType, typename OutputImageType> class AlgorithmITK : public Algorithm<InputImageType, OutputImageType>
{

public:

    // Dimension de l'image en entrée

    static const unsigned int Dimension = Algorithm<InputImageType, OutputImageType>::Dimension;

    // Défintion des types en rapport avec l'image d'entrée

    typedef typename Algorithm<InputImageType, OutputImageType>::InputPixelType     InputPixelType;
    typedef typename Algorithm<InputImageType, OutputImageType>::InputImagePointer  InputImagePointer;
    typedef typename itk::Image<InputPixelType, Dimension>                          InputITKImageType;

    // Défintion des types en rapport avec l'image de sortie

    typedef typename Algorithm<InputImageType, OutputImageType>::OutputPixelType    OutputPixelType;
    typedef typename Algorithm<InputImageType, OutputImageType>::OutputImagePointer OutputImagePointer;
    typedef typename itk::Image<OutputPixelType, Dimension>                         OutputITKImageType;

    // Définition des types filtre SliceBySlice

    typedef itk::SliceBySliceImageFilter<InputITKImageType, OutputITKImageType>     SliceBySliceFilterType;
    typedef typename SliceBySliceFilterType::InternalInputImageType                 SliceBySliceInputImageType;
    typedef typename SliceBySliceFilterType::InternalOutputImageType                SliceBySliceOutputImageType;

    // Fonctions d'algorithmes

    AlgorithmITK(const std::string &name) : Algorithm<InputImageType, OutputImageType>(name) { }
    virtual ~AlgorithmITK() { }

    virtual Algorithm<InputImageType, OutputImageType> * copy() = 0;

    virtual void executeInPlace(InputImagePointer image) = 0;
    virtual void executeSliceBySliceInPlace(InputImagePointer image) = 0;

    virtual OutputImagePointer executeOutPlace(InputImagePointer image) = 0;
    virtual OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image) = 0;

};

#endif // ALGORITHMITK_H
