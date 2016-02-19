#ifndef ALGORITHMDGTAL_H
#define ALGORITHMDGTAL_H

#include "../algorithm.h"

#include "../../dgtaltools.h"

template <typename InputImageType, typename OutputImageType> class AlgorithmDGtal : public Algorithm<InputImageType, OutputImageType>
{

public:

    // Défintion des types en rapport avec l'image d'entrée

    typedef typename Algorithm<InputImageType, OutputImageType>::InputPixelType             InputPixelType;
    typedef typename Algorithm<InputImageType, OutputImageType>::InputImagePointer          InputImagePointer;
    typedef typename DGtal::ImageContainerByITKImage<DGtal::Z3i::Domain, InputPixelType>    InputDGtalImageType;

    // Défintion des types en rapport avec l'image de sortie

    typedef typename Algorithm<InputImageType, OutputImageType>::OutputPixelType            OutputPixelType;
    typedef typename Algorithm<InputImageType, OutputImageType>::OutputImagePointer         OutputImagePointer;
    typedef typename DGtal::ImageContainerByITKImage<DGtal::Z3i::Domain, OutputPixelType>   OutputDGtalImageType;

    // Fonctions d'algorithmes

    AlgorithmDGtal(const std::string &name) : Algorithm<InputImageType, OutputImageType>(name) { }
    virtual ~AlgorithmDGtal() { }

    virtual Algorithm<InputImageType, OutputImageType> * copy() = 0;

    virtual void executeInPlace(InputImagePointer image) = 0;
    virtual void executeSliceBySliceInPlace(InputImagePointer image) = 0;

    virtual OutputImagePointer executeOutPlace(InputImagePointer image) = 0;
    virtual OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image) = 0;

};

#endif // ALGORITHMDGTAL_H
