#ifndef ALGORITHMCONNECTEDCOMPONENTEXTRACTION_H
#define ALGORITHMCONNECTEDCOMPONENTEXTRACTION_H

#include "../algorithm.h"
#include "../algorithmexception.h"
#include "../ITK/algorithmitkdiscretegaussian.h"
#include "../ITK/algorithmitkbinarythreshold.h"
#include "../ITK/algorithmitkconnectedcomponent.h"
#include "../ITK/algorithmitklabelshapekeepnobjects.h"
#include "../ITK/algorithmitkbinarymorphologicalclosing.h"

#include <QDebug>

template <typename InputImageType, typename OutputImageType>
class AlgorithmConnectedComponentExtraction : public Algorithm<InputImageType, OutputImageType>
{
    // Défintion des types en rapport avec l'image d'entrée

    typedef typename Algorithm<InputImageType, InputImageType>::InputPixelType          InputPixelType;
    typedef typename Algorithm<InputImageType, InputImageType>::InputImagePointer       InputImagePointer;

    // Défintion des types en rapport avec l'image de sortie

    typedef typename AlgorithmITK<InputImageType, InputImageType>::OutputImagePointer   OutputImagePointer;

public:

    AlgorithmConnectedComponentExtraction();
    ~AlgorithmConnectedComponentExtraction();

    void setLowThreshold(const InputPixelType &low);
    void setHighThreshold(const InputPixelType &high);

    InputPixelType getLowThreshold();
    InputPixelType getHighThreshold();

    Algorithm<InputImageType, OutputImageType> * copy();

    void executeInPlace(InputImagePointer image);
    void executeSliceBySliceInPlace(InputImagePointer image);

    OutputImagePointer executeOutPlace(InputImagePointer image);
    OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image);

private:

    InputPixelType lowThreshold;
    InputPixelType highThreshold;

    AlgorithmITKDiscreteGaussian<Image, Image> *gaussianDiscretAlgorithm;
    AlgorithmITKBinaryThreshold<Image, Image> *binaryThresholdAlgorithm;
    AlgorithmITKConnectedComponent<Image, Image> *connectedComponentAlgorithm;
    AlgorithmITKLabelShapeKeepNObjects<Image> *labelShapeKeepAlgorithm1;
    AlgorithmITKLabelShapeKeepNObjects<Image> *labelShapeKeepAlgorithm2;
    AlgorithmITKBinaryMorphologicalClosing<Image, Image> *binaryMorphologicalClosingAlgorithm;

};

// ------------------- Constructeur et destructeur ---------------------------

template <typename InputImageType, typename OutputImageType>
AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::AlgorithmConnectedComponentExtraction() :
    Algorithm<InputImageType, OutputImageType>("[Adrien] Extraction de composantes connexes"), lowThreshold(0), highThreshold(0)
{
    gaussianDiscretAlgorithm = new AlgorithmITKDiscreteGaussian<Image, Image>();
    binaryThresholdAlgorithm = new AlgorithmITKBinaryThreshold<Image, Image>();
    connectedComponentAlgorithm = new AlgorithmITKConnectedComponent<Image, Image>();
    labelShapeKeepAlgorithm1 = new AlgorithmITKLabelShapeKeepNObjects<Image>();
    labelShapeKeepAlgorithm2 = new AlgorithmITKLabelShapeKeepNObjects<Image>();
    binaryMorphologicalClosingAlgorithm = new AlgorithmITKBinaryMorphologicalClosing<Image, Image>();
}

template <typename InputImageType, typename OutputImageType>
AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::~AlgorithmConnectedComponentExtraction()
{
    delete gaussianDiscretAlgorithm;
    delete binaryThresholdAlgorithm;
    delete connectedComponentAlgorithm;
    delete labelShapeKeepAlgorithm1;
    delete labelShapeKeepAlgorithm2;
    delete binaryMorphologicalClosingAlgorithm;
}

// ------------------------- Getter et seter ---------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::setLowThreshold(const InputPixelType &low)
{
    lowThreshold = low;
}

template <typename InputImageType, typename OutputImageType>
void AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::setHighThreshold(const InputPixelType &high)
{
    highThreshold = high;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::InputPixelType
AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::getLowThreshold()
{
    return lowThreshold;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::InputPixelType
AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::getHighThreshold()
{
    return highThreshold;
}

// ------------------------------- Copy ---------------------------------------


template <typename InputImageType, typename OutputImageType>
Algorithm<InputImageType, OutputImageType> * AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::copy()
{
    AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType> *copy = new AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>();
    copy->setLowThreshold(lowThreshold);
    copy->setHighThreshold(highThreshold);
    return copy;
}

// ---------------------- Fonctions d'éxecution ------------------------------

template <typename InputImageType, typename OutputImageType>
void AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::executeInPlace(InputImagePointer image)
{
    // Filtrage Gaussien
    gaussianDiscretAlgorithm->setVariance(0.6);
    gaussianDiscretAlgorithm->setDimensionality(2);
    gaussianDiscretAlgorithm->executeInPlace(image);

    // Binarisation de l'image
    binaryThresholdAlgorithm->setLower(lowThreshold);
    binaryThresholdAlgorithm->setUpper(highThreshold);
    binaryThresholdAlgorithm->setInsideValue(1);
    binaryThresholdAlgorithm->setOutsideValue(0);
    binaryThresholdAlgorithm->executeInPlace(image);

    // Extraction en composantes connexes
    connectedComponentAlgorithm->setFullyConnected(true);
    connectedComponentAlgorithm->executeInPlace(image);

    // On garde les 2 plus grosses
    labelShapeKeepAlgorithm1->setBackgroundValue(0);
    labelShapeKeepAlgorithm1->setNumberOfObjects(2);
    //labelShapeKeepAlgorithm1->setAttribute(AlgorithmITKLabelShapeKeepNObjects<Image>::LabelObjectFilterType::NUMBER_OF_PIXELS);
    labelShapeKeepAlgorithm1->executeInPlace(image);

    // On supprime la plus grosse
    labelShapeKeepAlgorithm2->setBackgroundValue(0);
    labelShapeKeepAlgorithm2->setNumberOfObjects(1);
    //labelShapeKeepAlgorithm2->setAttribute(AlgorithmITKLabelShapeKeepNObjects<Image>::LabelObjectFilterType::NUMBER_OF_PIXELS);
    labelShapeKeepAlgorithm2->executeInPlace(image);

    // Fermeture
    binaryMorphologicalClosingAlgorithm->setForegroundValue(120);
    double kernel[Dimension] = {5, 5, 0};
    binaryMorphologicalClosingAlgorithm->setKernel(kernel);
    binaryMorphologicalClosingAlgorithm->executeInPlace(image);

    qDebug() << QString("Nombre de composantes connexes : %1").arg(connectedComponentAlgorithm->getObjectCount());
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
template <typename InputImageType, typename OutputImageType>
void AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::executeSliceBySliceInPlace(InputImagePointer image)
{
    throw AlgorithmException(MESSAGE_NO_SLICE_BY_SLICE_INPLACE_FUNCTION_EXCEPTION);
}
#pragma GCC diagnostic pop

template <typename InputImageType, typename OutputImageType>
typename AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::executeOutPlace(InputImagePointer image)
{
    OutputImageType *result = new OutputImageType(*image);

    gaussianDiscretAlgorithm->setVariance(0.6);
    gaussianDiscretAlgorithm->setDimensionality(2);
    gaussianDiscretAlgorithm->executeInPlace(result);

    binaryThresholdAlgorithm->setLower(lowThreshold);
    binaryThresholdAlgorithm->setUpper(highThreshold);
    binaryThresholdAlgorithm->setInsideValue(1);
    binaryThresholdAlgorithm->setOutsideValue(0);
    binaryThresholdAlgorithm->executeInPlace(result);

    connectedComponentAlgorithm->setFullyConnected(true);
    connectedComponentAlgorithm->executeInPlace(result);

    labelShapeKeepAlgorithm1->setBackgroundValue(0);
    labelShapeKeepAlgorithm1->setNumberOfObjects(2);
    //labelShapeKeepAlgorithm1->setAttribute(AlgorithmITKLabelShapeKeepNObjects<Image>::LabelObjectFilterType::NUMBER_OF_PIXELS);
    labelShapeKeepAlgorithm1->executeInPlace(result);

    labelShapeKeepAlgorithm2->setBackgroundValue(0);
    labelShapeKeepAlgorithm2->setNumberOfObjects(1);
    //labelShapeKeepAlgorithm2->setAttribute(AlgorithmITKLabelShapeKeepNObjects<Image>::LabelObjectFilterType::NUMBER_OF_PIXELS);
    labelShapeKeepAlgorithm2->executeInPlace(result);

    binaryMorphologicalClosingAlgorithm->setForegroundValue(120);
    double kernel[Dimension] = {5, 5, 0};
    binaryMorphologicalClosingAlgorithm->setKernel(kernel);
    binaryMorphologicalClosingAlgorithm->executeInPlace(result);

    qDebug() << QString("Nombre de composantes connexes : %1").arg(connectedComponentAlgorithm->getObjectCount());

    return result;
}

template <typename InputImageType, typename OutputImageType>
typename AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::OutputImagePointer
AlgorithmConnectedComponentExtraction<InputImageType, OutputImageType>::executeSliceBySliceOutPlace(InputImagePointer image)
{
    throw AlgorithmException(MESSAGE_NO_SLICE_BY_SLICE_OUTPLACE_FUNCTION_EXCEPTION);
    return image;
}

#endif // ALGORITHMCONNECTEDCOMPONENTEXTRACTION_H
