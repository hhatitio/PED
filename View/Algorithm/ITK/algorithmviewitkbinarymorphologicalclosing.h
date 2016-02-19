#ifndef ALGORITHMVIEWITKBINARYMORPHOLOGICALCLOSING_H
#define ALGORITHMVIEWITKBINARYMORPHOLOGICALCLOSING_H

#include "../algorithmview.h"
#include "../../../Model/Algorithm/ITK/algorithmitkbinarymorphologicalclosing.h"

#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>

class AlgorithmViewITKBinaryMorphologicalClosing : public AlgorithmView
{

    static const unsigned int Dimension = AlgorithmITKBinaryMorphologicalClosing<ITKImage, ITKImage>::Dimension;
    typedef typename AlgorithmITKBinaryMorphologicalClosing<ITKImage, ITKImage>::KernelRadiusType KernelRadiusType;

public:

    AlgorithmViewITKBinaryMorphologicalClosing(int maxValue, int kernelDimension, QWidget *parent = 0);
    ~AlgorithmViewITKBinaryMorphologicalClosing();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewITKBinaryMorphologicalClosing * copy();

private:

    std::vector<QDoubleSpinBox*> kernel;
    QSpinBox foregroundValue;
    int kernelDimension;

};

AlgorithmViewITKBinaryMorphologicalClosing::AlgorithmViewITKBinaryMorphologicalClosing(int maxValue, int kernelDimension, QWidget *parent) :
    AlgorithmView("Fermeture morphologique binaire", parent)
{
    // Construction du formulaire
    QFormLayout *layoutForm = new QFormLayout(this);
    this->kernelDimension = kernelDimension;
    for (int dim=0; dim<kernelDimension; ++dim)
    {
        QDoubleSpinBox *valDim = new QDoubleSpinBox();
        layoutForm->addRow(QString("Noyau (dimension %1)").arg(dim), valDim);
        kernel.push_back(valDim);
    }
    foregroundValue.setMaximum(maxValue);
    layoutForm->addRow("Valeur permier-plan", &foregroundValue);
    hide();
}

AlgorithmViewITKBinaryMorphologicalClosing::~AlgorithmViewITKBinaryMorphologicalClosing() { }

void AlgorithmViewITKBinaryMorphologicalClosing::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKBinaryMorphologicalClosing<Image, Image> *algorithmClosing = (AlgorithmITKBinaryMorphologicalClosing<Image, Image> *) algorithm;
    KernelRadiusType kernelRadius = algorithmClosing->getKernel();
    for (int dim=0; dim < Dimension; ++dim)
    {
        kernel[dim]->setValue(kernelRadius[dim]);
    }
    foregroundValue.setValue(algorithmClosing->getForegroundValue());
}

void AlgorithmViewITKBinaryMorphologicalClosing::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKBinaryMorphologicalClosing<Image, Image> *algorithmClosing = (AlgorithmITKBinaryMorphologicalClosing<Image, Image> *) algorithm;
    KernelRadiusType kernelRadius;
    for (int dim=0; dim < Dimension; ++dim)
    {
        kernelRadius[dim] = kernel[dim]->value();
    }
    algorithmClosing->setKernel(kernelRadius);
    algorithmClosing->setForegroundValue(foregroundValue.value());
}

QString AlgorithmViewITKBinaryMorphologicalClosing::toString()
{
    QString result = "Fermeture morphologique binaire";
    result += "\n -> Noyau : ";
    for (int dim = 0; dim < Dimension; ++dim)
    {
        result += QString::number(kernel[dim]->value()) + " ";
    }
    result += "\n -> Valeur du premier-plan : " + QString::number(foregroundValue.value());
    return result;
}

AlgorithmViewITKBinaryMorphologicalClosing * AlgorithmViewITKBinaryMorphologicalClosing::copy()
{
    AlgorithmViewITKBinaryMorphologicalClosing *copy = new AlgorithmViewITKBinaryMorphologicalClosing(foregroundValue.maximum(), kernelDimension);
    return copy;
}

#endif // ALGORITHMVIEWITKBINARYMORPHOLOGICALCLOSING_H
