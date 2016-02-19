#ifndef ALGORITMITKTHRESHOLD_H
#define ALGORITMITKTHRESHOLD_H

#include "../algorithmview.h"
#include "../../Spanslider/qxtspanslider.h"
#include "../../../Model/Algorithm/ITK/algorithmitkthreshold.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QLabel>
#include <QCheckBox>

class AlgorithmViewITKThreshold : public AlgorithmView
{

public:

    AlgorithmViewITKThreshold(double lower, double upper, QWidget *parent = 0);
    ~AlgorithmViewITKThreshold();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewITKThreshold * copy();

private:

    QxtSpanSlider sliderIntensity;
    QSpinBox lower;
    QSpinBox upper;
    QCheckBox buttonOutsideValue;
    QSpinBox outsideValue;

};

AlgorithmViewITKThreshold::AlgorithmViewITKThreshold(double lower, double upper, QWidget *parent) :
    AlgorithmView("Seuillage", parent), sliderIntensity(Qt::Horizontal)
{
    // Construction des composants du formulaire
    this->lower.setRange(lower, upper);
    this->lower.setValue(lower);
    this->upper.setRange(lower, upper);
    this->upper.setValue(upper);
    sliderIntensity.setRange(lower, upper);
    sliderIntensity.setLowerValue(lower);
    sliderIntensity.setUpperValue(upper);
    buttonOutsideValue.setChecked(false);
    outsideValue.setRange(lower, upper);
    outsideValue.setValue(lower);
    QLabel *labelIntensity = new QLabel("Interval d'intensité : ");
    QLabel *labelOutsideValue = new QLabel("Valeur extérieure : ");

    // Connexion du slider
    QObject::connect(&sliderIntensity, SIGNAL(lowerPositionChanged(int)), &(this->lower), SLOT(setValue(int)));
    QObject::connect(&(this->lower), SIGNAL(valueChanged(int)), &sliderIntensity, SLOT(setLowerValue(int)));
    QObject::connect(&sliderIntensity, SIGNAL(upperPositionChanged(int)), &(this->upper), SLOT(setValue(int)));
    QObject::connect(&(this->upper), SIGNAL(valueChanged(int)), &sliderIntensity, SLOT(setUpperValue(int)));

    // Création du formulaire
    QHBoxLayout *layoutIntensity = new QHBoxLayout();
    layoutIntensity->addWidget(labelIntensity);
    layoutIntensity->addWidget(&(this->lower));
    layoutIntensity->addWidget(&sliderIntensity);
    layoutIntensity->addWidget(&(this->upper));
    QHBoxLayout *layoutOutsideValue = new QHBoxLayout();
    layoutOutsideValue->addWidget(labelOutsideValue);
    layoutOutsideValue->addWidget(&buttonOutsideValue);
    layoutOutsideValue->addWidget(&outsideValue);
    QVBoxLayout *layoutForm = new QVBoxLayout(this);
    layoutForm->addLayout(layoutIntensity);
    layoutForm->addLayout(layoutOutsideValue);
    hide();
}

AlgorithmViewITKThreshold::~AlgorithmViewITKThreshold() { }

void AlgorithmViewITKThreshold::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKThreshold<Image> *algorithmThreshold = (AlgorithmITKThreshold<Image> *) algorithm;
    sliderIntensity.setLowerValue(algorithmThreshold->getLower());
    sliderIntensity.setUpperValue(algorithmThreshold->getUpper());
    lower.setValue(algorithmThreshold->getLower());
    upper.setValue(algorithmThreshold->getUpper());
    outsideValue.setValue(algorithmThreshold->getOutsideValue());
    (outsideValue.value() == outsideValue.minimum())
            ? buttonOutsideValue.setChecked(false)
            : buttonOutsideValue.setChecked(true);
}

void AlgorithmViewITKThreshold::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKThreshold<Image> *algorithmThreshold = (AlgorithmITKThreshold<Image> *) algorithm;
    algorithmThreshold->setLower(lower.value());
    algorithmThreshold->setUpper(upper.value());
    (buttonOutsideValue.isChecked())
        ? algorithmThreshold->setOutsideValue(outsideValue.value())
        : algorithmThreshold->setOutsideValue(outsideValue.minimum());
}

QString AlgorithmViewITKThreshold::toString()
{
    QString result = "Seuillage";
    result += "\n -> Seuil inférieur : " + QString::number(lower.value());
    result += "\n -> Seuil supérieur : " + QString::number(upper.value());
    (buttonOutsideValue.isChecked())
        ? result += "\n -> Valeur extérieure : " + QString::number(outsideValue.value())
        : result += "\n -> Valeur extérieure : " + QString::number(outsideValue.minimum());
    return result;
}

AlgorithmViewITKThreshold * AlgorithmViewITKThreshold::copy()
{
    AlgorithmViewITKThreshold *copy = new AlgorithmViewITKThreshold(lower.minimum(), upper.maximum());
    return copy;
}

#endif // ALGORITMITKTHRESHOLD_H
