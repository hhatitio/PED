#ifndef ALGORITHMVIEWITKBINARYTHRESHOLD_H
#define ALGORITHMVIEWITKBINARYTHRESHOLD_H

#include "../algorithmview.h"
#include "../../Spanslider/qxtspanslider.h"
#include "../../../Model/Algorithm/ITK/algorithmitkbinarythreshold.h"

#include <QRadioButton>
#include <QGridLayout>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class AlgorithmViewITKBinaryThreshold : public AlgorithmView
{

public:

    AlgorithmViewITKBinaryThreshold(double lower, double upper, QWidget *parent = 0);
    ~AlgorithmViewITKBinaryThreshold();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewITKBinaryThreshold * copy();

private:

    QRadioButton buttonThreshold;
    QRadioButton buttonInterval;
    QSlider sliderThreshold;
    QSpinBox spinBoxThreshold;
    QxtSpanSlider sliderInterval;
    QSpinBox lower;
    QSpinBox upper;

};

AlgorithmViewITKBinaryThreshold::AlgorithmViewITKBinaryThreshold(double lower, double upper, QWidget *parent) :
    AlgorithmView("Seuillage binaire", parent), buttonThreshold(this), buttonInterval(this),
    sliderThreshold(Qt::Horizontal), sliderInterval(Qt::Horizontal)
{
    // Création des composants du formulaire
    QLabel *labelThreshold = new QLabel("Seuil : ");
    buttonThreshold.setChecked(true);
    sliderThreshold.setRange(lower, upper-1);
    sliderThreshold.setValue(upper/2);
    spinBoxThreshold.setRange(lower, upper);
    spinBoxThreshold.setValue(upper/2);
    QLabel *labelInterval = new QLabel("Interval d'intensité : ");
    buttonInterval.setChecked(false);
    sliderInterval.setRange(lower, upper);
    sliderInterval.setLowerValue(lower);
    sliderInterval.setUpperValue(upper);
    this->lower.setRange(lower, upper);
    this->lower.setValue(lower);
    this->upper.setRange(lower, upper);
    this->upper.setValue(upper);

    // Connexions des composants
    QObject::connect(&sliderThreshold, SIGNAL(sliderPressed()), &buttonThreshold, SLOT(toggle()));
    QObject::connect(&sliderThreshold, SIGNAL(valueChanged(int)), &spinBoxThreshold, SLOT(setValue(int)));
    QObject::connect(&spinBoxThreshold, SIGNAL(valueChanged(int)), &sliderThreshold, SLOT(setValue(int)));
    QObject::connect(&spinBoxThreshold, SIGNAL(valueChanged(int)), &buttonThreshold, SLOT(toggle()));
    QObject::connect(&sliderInterval, SIGNAL(sliderPressed()), &buttonInterval, SLOT(toggle()));
    QObject::connect(&sliderInterval, SIGNAL(lowerValueChanged(int)), &(this->lower), SLOT(setValue(int)));
    QObject::connect(&sliderInterval, SIGNAL(upperValueChanged(int)), &(this->upper), SLOT(setValue(int)));
    QObject::connect(&(this->lower), SIGNAL(valueChanged(int)), &sliderInterval, SLOT(setLowerValue(int)));
    QObject::connect(&(this->lower), SIGNAL(valueChanged(int)), &buttonInterval, SLOT(toggle()));
    QObject::connect(&(this->upper), SIGNAL(valueChanged(int)), &sliderInterval, SLOT(setUpperValue(int)));
    QObject::connect(&(this->upper), SIGNAL(valueChanged(int)), &buttonInterval, SLOT(toggle()));

    // Création du formulaire
    QGridLayout *layoutForm = new QGridLayout(this);
    layoutForm->addWidget(&buttonThreshold, 0, 0, 1, 1);
    layoutForm->addWidget(labelThreshold, 0, 1, 1, 1);
    layoutForm->addWidget(&spinBoxThreshold, 0, 2, 1, 1);
    layoutForm->addWidget(&sliderThreshold, 0, 3, 1, 1);
    layoutForm->addWidget(&buttonInterval, 1, 0, 1, 1);
    layoutForm->addWidget(labelInterval, 1, 1, 1, 1);
    layoutForm->addWidget(&(this->lower), 1, 2, 1, 1);
    layoutForm->addWidget(&sliderInterval, 1, 3, 1, 1);
    layoutForm->addWidget(&(this->upper), 1, 4, 1, 1);
    hide();
}

AlgorithmViewITKBinaryThreshold::~AlgorithmViewITKBinaryThreshold() { }

void AlgorithmViewITKBinaryThreshold::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKBinaryThreshold<Image, Image> *algorithmBinaryThreshold = (AlgorithmITKBinaryThreshold<Image, Image> *) algorithm;
    buttonThreshold.setChecked(false);
    sliderThreshold.setValue(algorithmBinaryThreshold->getLower());
    spinBoxThreshold.setValue(algorithmBinaryThreshold->getLower());
    buttonInterval.setChecked(true);
    sliderInterval.setLowerValue(algorithmBinaryThreshold->getLower());
    sliderInterval.setUpperValue(algorithmBinaryThreshold->getUpper());
    lower.setValue(algorithmBinaryThreshold->getLower());
    upper.setValue(algorithmBinaryThreshold->getUpper());
}

void AlgorithmViewITKBinaryThreshold::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKBinaryThreshold<Image, Image> *algorithmBinaryThreshold = (AlgorithmITKBinaryThreshold<Image, Image> *) algorithm;
    if (buttonThreshold.isChecked())
    {
        algorithmBinaryThreshold->setLower(spinBoxThreshold.value());
        algorithmBinaryThreshold->setUpper(spinBoxThreshold.maximum());
    }
    else
    {
        algorithmBinaryThreshold->setLower(lower.value());
        algorithmBinaryThreshold->setUpper(upper.value());
    }
    algorithmBinaryThreshold->setInsideValue(spinBoxThreshold.maximum());
    algorithmBinaryThreshold->setOutsideValue(0);
}

QString AlgorithmViewITKBinaryThreshold::toString()
{
    QString result = "Seuillage binaire";
    if (buttonThreshold.isChecked())
    {
        result += "\n -> Seuil inférieur : " + QString::number(spinBoxThreshold.value());
        result += "\n -> Seuil supérieur : " + QString::number(spinBoxThreshold.maximum());
    }
    else
    {
        result += "\n -> Seuil inférieur : " + QString::number(lower.value());
        result += "\n -> Seuil supérieur : " + QString::number(upper.value());
    }
    return result;
}

AlgorithmViewITKBinaryThreshold * AlgorithmViewITKBinaryThreshold::copy()
{
    AlgorithmViewITKBinaryThreshold *copy = new AlgorithmViewITKBinaryThreshold(lower.minimum(), upper.maximum());
    return copy;
}

#endif // ALGORITHMVIEWITKBINARYTHRESHOLD_H
