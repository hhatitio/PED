#ifndef ALGORITHMDGTALDISTANCETRANSFORMATION_H
#define ALGORITHMDGTALDISTANCETRANSFORMATION_H

#include "algorithmdgtal.h"
#include "../algorithmexception.h"

#include <DGtal/images/IntervalForegroundPredicate.h>
#include <DGtal/geometry/volumes/distance/DistanceTransformation.h>

template <typename InputImageType, typename OutputImageType>
class AlgorithmDGtalDistanceTransformation : public AlgorithmDGtal<InputImageType, OutputImageType>
{
    // Défintion des types en rapport avec l'image d'entrée

    typedef typename AlgorithmDGtal<InputImageType, OutputImageType>::InputPixelType        InputPixelType;
    typedef typename AlgorithmDGtal<InputImageType, OutputImageType>::InputImagePointer     InputImagePointer;
    typedef typename AlgorithmDGtal<InputImageType, OutputImageType>::InputDGtalImageType   InputDGtalImageType;

    // Défintion des types en rapport avec l'image de sortie

    typedef typename AlgorithmDGtal<InputImageType, OutputImageType>::OutputPixelType       OutputPixelType;
    typedef typename AlgorithmDGtal<InputImageType, OutputImageType>::OutputImagePointer    OutputImagePointer;
    typedef typename AlgorithmDGtal<InputImageType, OutputImageType>::OutputDGtalImageType  OutputDGtalImageType;

    // Défintion des filtres

    typedef DGtal::functors::IntervalForegroundPredicate<InputDGtalImageType>                   Binarizer;
    typedef DGtal::DistanceTransformation<DGtal::Z3i::Space, Binarizer, DGtal::Z3i::L2Metric>   FilterType;

public:

    AlgorithmDGtalDistanceTransformation();
    ~AlgorithmDGtalDistanceTransformation();

    Algorithm<InputImageType, OutputImageType> * copy();

    void executeInPlace(InputImagePointer image);
    void executeSliceBySliceInPlace(InputImagePointer image);

    OutputImagePointer executeOutPlace(InputImagePointer image);
    OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image);

};

// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType, typename OutputImageType>
AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::AlgorithmDGtalDistanceTransformation() :
    AlgorithmDGtal<InputImageType, OutputImageType>("Distance transformation") { }

template <typename InputImageType, typename OutputImageType>
AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::~AlgorithmDGtalDistanceTransformation() { }

// ------------------------------- Copy ---------------------------------------


template <typename InputImageType, typename OutputImageType>
Algorithm<InputImageType, OutputImageType> * AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::copy()
{
    AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType> *copy =
            new AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>();
    return copy;
}

// ---------------------- Fonctions d'éxecution ------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::executeInPlace(InputImagePointer image)
{
    OutputDGtalImageType dgtalImage = DGtalTools<OutputPixelType>::convertInDGtalITKImage(image, false);
    Binarizer binarizer (dgtalImage, 1, 135);
    FilterType filter (&dgtalImage.domain(), &binarizer, &DGtal::Z3i::l2Metric);

    typename OutputDGtalImageType::Iterator itImage = dgtalImage.begin();
    for (typename FilterType::ConstRange::ConstIterator it=filter.constRange().begin(); it!=filter.constRange().end(); ++it, ++itImage)
    {
        dgtalImage.setValue(itImage, *it);
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
template <typename InputImageType, typename OutputImageType>
void AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    throw AlgorithmException(MESSAGE_NO_SLICE_BY_SLICE_INPLACE_FUNCTION_EXCEPTION);
}
#pragma GCC diagnostic pop

template <typename InputImageType, typename OutputImageType>
typename AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::executeOutPlace(InputImagePointer image)
{
    OutputDGtalImageType dgtalImage = DGtalTools<OutputPixelType>::convertInDGtalITKImage(image, true);
    Binarizer binarizer (dgtalImage, 1, 135);
    FilterType filter (&dgtalImage.domain(), &binarizer, &DGtal::Z3i::l2Metric);

    typename OutputDGtalImageType::Iterator itImage = dgtalImage.begin();
    for (typename FilterType::ConstRange::ConstIterator it=filter.constRange().begin(); it!=filter.constRange().end(); ++it, ++itImage)
    {
        dgtalImage.setValue(itImage, *it);
    }

    return DGtalTools<OutputPixelType>::convertInImage(dgtalImage, false);
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmDGtalDistanceTransformation<InputImageType, OutputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    throw AlgorithmException(MESSAGE_NO_SLICE_BY_SLICE_OUTPLACE_FUNCTION_EXCEPTION);
    return image;
}

#endif // ALGORITHMDGTALDISTANCETRANSFORMATION_H
