#ifndef ALGORITHMVIEWCONNECTEDCOMPONENTEXTRACTION_H
#define ALGORITHMVIEWCONNECTEDCOMPONENTEXTRACTION_H

#include "../algorithmview.h"
#include "../../Spanslider/qxtspanslider.h"
#include "../../../Model/Algorithm/Adrien/algorithmconnectedcomponentextraction.h"

#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QLabel>

class AlgorithmViewConnectedComponentExtraction : public AlgorithmView
{

public:

    AlgorithmViewConnectedComponentExtraction(double lowThreshold, double highThreshold, QWidget *parent = 0);
    ~AlgorithmViewConnectedComponentExtraction();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewConnectedComponentExtraction * copy();

private:

    QxtSpanSlider sliderThreshold;
    QSpinBox lowThreshold;
    QSpinBox highThreshold;

};

AlgorithmViewConnectedComponentExtraction::AlgorithmViewConnectedComponentExtraction(double lowThreshold, double highThreshold, QWidget *parent) :
    AlgorithmView("[Adrien] Extraction de composantes connexes", parent), sliderThreshold(Qt::Horizontal)
{
    // Construction des composants du formulaire
    sliderThreshold.setRange(lowThreshold, highThreshold);
    sliderThreshold.setLowerValue(lowThreshold);
    sliderThreshold.setUpperValue(highThreshold);
    this->lowThreshold.setRange(lowThreshold, highThreshold);
    this->lowThreshold.setValue(lowThreshold);
    this->highThreshold.setRange(lowThreshold, highThreshold);
    this->highThreshold.setValue(highThreshold);

    // Construction du formulaire
    QHBoxLayout *layoutThreshold = new QHBoxLayout(this);
    QLabel *labelThreshold = new QLabel("Interval d'intensité : ");
    layoutThreshold->addWidget(labelThreshold);
    layoutThreshold->addWidget(&(this->lowThreshold));
    layoutThreshold->addWidget(&sliderThreshold);
    layoutThreshold->addWidget(&(this->highThreshold));
    hide();

    // Connexion du slider
    QObject::connect(&(this->lowThreshold), SIGNAL(valueChanged(int)), &sliderThreshold, SLOT(setLowerValue(int)));
    QObject::connect(&sliderThreshold, SIGNAL(lowerValueChanged(int)), &(this->lowThreshold), SLOT(setValue(int)));
    QObject::connect(&(this->highThreshold), SIGNAL(valueChanged(int)), &sliderThreshold, SLOT(setUpperValue(int)));
    QObject::connect(&sliderThreshold, SIGNAL(upperValueChanged(int)), &(this->highThreshold), SLOT(setValue(int)));
}

AlgorithmViewConnectedComponentExtraction::~AlgorithmViewConnectedComponentExtraction() { }

void AlgorithmViewConnectedComponentExtraction::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
    AlgorithmConnectedComponentExtraction<Image, Image> *algorithmExtraction = (AlgorithmConnectedComponentExtraction<Image, Image> *) algorithm;
    sliderThreshold.setLowerValue(algorithmExtraction->getLowThreshold());
    sliderThreshold.setUpperValue(algorithmExtraction->getHighThreshold());
    lowThreshold.setValue(algorithmExtraction->getLowThreshold());
    highThreshold.setValue(algorithmExtraction->getHighThreshold());
}

void AlgorithmViewConnectedComponentExtraction::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
    AlgorithmConnectedComponentExtraction<Image, Image> *algorithmExtraction = (AlgorithmConnectedComponentExtraction<Image, Image> *) algorithm;
    algorithmExtraction->setLowThreshold(lowThreshold.value());
    algorithmExtraction->setHighThreshold(highThreshold.value());
}

QString AlgorithmViewConnectedComponentExtraction::toString()
{
    QString result = "[Adrien] Extraction de composantes connexes";
    result += "\n -> Seuil inférieur : " + QString::number(lowThreshold.value());
    result += "\n -> Seuil supérieur : " + QString::number(highThreshold.value());
    return result;
}


AlgorithmViewConnectedComponentExtraction * AlgorithmViewConnectedComponentExtraction::copy()
{
    AlgorithmViewConnectedComponentExtraction *copy = new AlgorithmViewConnectedComponentExtraction(lowThreshold.minimum(), highThreshold.maximum());
    return copy;
}

#endif // ALGORITHMVIEWCONNECTEDCOMPONENTEXTRACTION_H
