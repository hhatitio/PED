#ifndef ALGORITHMVIEWITKDISCRETEGAUSSIAN_H
#define ALGORITHMVIEWITKDISCRETEGAUSSIAN_H

#include "../algorithmview.h"
#include "../../../Model/Algorithm/ITK/algorithmitkdiscretegaussian.h"

#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>

class AlgorithmViewITKDiscreteGaussian : public AlgorithmView
{

public:

    AlgorithmViewITKDiscreteGaussian(int dimensionality, QWidget *parent = 0);
    ~AlgorithmViewITKDiscreteGaussian();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewITKDiscreteGaussian * copy();

private:

    QDoubleSpinBox variance;
    QSpinBox dimensionality;

};

AlgorithmViewITKDiscreteGaussian::AlgorithmViewITKDiscreteGaussian(int dimensionality, QWidget *parent) :
    AlgorithmView("Gaussien discret", parent)
{
    // Construction du formulaire
    QFormLayout *layoutForm = new QFormLayout(this);
    variance.setMinimum(0.01);
    this->dimensionality.setRange(1, dimensionality);
    this->dimensionality.setValue(dimensionality);
    layoutForm->addRow("Variance", &variance);
    layoutForm->addRow("Dimension lissage", &(this->dimensionality));
    hide();
}

AlgorithmViewITKDiscreteGaussian::~AlgorithmViewITKDiscreteGaussian() { }

void AlgorithmViewITKDiscreteGaussian::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKDiscreteGaussian<Image, Image> *algorithmDiscretGaussian = (AlgorithmITKDiscreteGaussian<Image, Image> *) algorithm;
    variance.setValue(algorithmDiscretGaussian->getVariance());
    dimensionality.setValue(algorithmDiscretGaussian->getDimensionality());
}

void AlgorithmViewITKDiscreteGaussian::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKDiscreteGaussian<Image, Image> *algorithmDiscretGaussian = (AlgorithmITKDiscreteGaussian<Image, Image> *) algorithm;
    algorithmDiscretGaussian->setVariance(variance.value());
    algorithmDiscretGaussian->setDimensionality(dimensionality.value());
}

QString AlgorithmViewITKDiscreteGaussian::toString()
{
    QString result = "Gaussien discret";
    result += "\n -> Variance : " + QString::number(variance.value());
    result += "\n -> Dimension lissage : " + QString::number(dimensionality.value());
    return result;
}

AlgorithmViewITKDiscreteGaussian * AlgorithmViewITKDiscreteGaussian::copy()
{
    AlgorithmViewITKDiscreteGaussian *copy = new AlgorithmViewITKDiscreteGaussian(dimensionality.value());
    return copy;
}

#endif // ALGORITHMVIEWITKDISCRETEGAUSSIAN_H
