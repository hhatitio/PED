#ifndef ALGORITHMVIEWITKBINARYTHINNINGIMAGEFILTER
#define ALGORITHMVIEWITKBINARYTHINNINGIMAGEFILTER

#include "../algorithmview.h"
#include "../../Spanslider/qxtspanslider.h"
#include "../../../Model/Algorithm/ITK/algorithmitkbinarythinningimagefilter.h"

#include <iostream>

#include <QRadioButton>
#include <QGridLayout>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class AlgorithmViewITKBinaryThinningImageFilter : public AlgorithmView
{
public:

    AlgorithmViewITKBinaryThinningImageFilter(double coordinateTolerance, double directionTolerance, QWidget *parent = 0);
    ~AlgorithmViewITKBinaryThinningImageFilter();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewITKBinaryThinningImageFilter * copy();

private:

    // Widgets servant à récupérer les paramètres utiles aux filtres

    QRadioButton buttonCoordinate;
    QRadioButton buttonDirection;
    QSlider sliderCoordinate;
    QSpinBox spinBoxCoordinate;
    QSlider sliderDirection;
    QSpinBox spinBoxDirection;

};

AlgorithmViewITKBinaryThinningImageFilter::AlgorithmViewITKBinaryThinningImageFilter(double minValue, double maxValue, QWidget *parent) :
    AlgorithmView("Squelettisation", parent), buttonCoordinate(this), buttonDirection(this), sliderCoordinate(Qt::Horizontal), sliderDirection(Qt::Horizontal)
{
    /** TODO : Construction du formulaire **/
    std::cout << "View - Construction formulaire ..." << std::endl;

    // Création des composants du formulaire
    QLabel *labelCoordinate = new QLabel("Coordinate Tolerance : ");
    buttonCoordinate.setChecked(true);
    sliderCoordinate.setRange(minValue, maxValue);
    sliderCoordinate.setValue(maxValue/2);
    spinBoxCoordinate.setRange(minValue, maxValue);
    spinBoxCoordinate.setValue(maxValue/2);

    QLabel *labelDirection = new QLabel("Direction Tolerance : ");
    buttonDirection.setChecked(false);
    sliderDirection.setRange(minValue, maxValue);
    sliderDirection.setValue(maxValue/2);
    spinBoxDirection.setRange(minValue, maxValue);
    spinBoxDirection.setValue(maxValue/2);

    // Connexions des composants
    QObject::connect(&sliderCoordinate, SIGNAL(sliderPressed()), &buttonCoordinate, SLOT(toggle()));
    QObject::connect(&sliderCoordinate, SIGNAL(valueChanged(int)), &spinBoxCoordinate, SLOT(setValue(int)));
    QObject::connect(&spinBoxCoordinate, SIGNAL(valueChanged(int)), &sliderCoordinate, SLOT(setValue(int)));
    QObject::connect(&spinBoxCoordinate, SIGNAL(valueChanged(int)), &buttonCoordinate, SLOT(toggle()));

    QObject::connect(&sliderDirection, SIGNAL(sliderPressed()), &buttonDirection, SLOT(toggle()));
    QObject::connect(&sliderDirection, SIGNAL(valueChanged(int)), &spinBoxDirection, SLOT(setValue(int)));
    QObject::connect(&spinBoxDirection, SIGNAL(valueChanged(int)), &sliderDirection, SLOT(setValue(int)));
    QObject::connect(&spinBoxDirection, SIGNAL(valueChanged(int)), &buttonDirection, SLOT(toggle()));

    // Création du formulaire
    QGridLayout *layoutForm = new QGridLayout(this);
    layoutForm->addWidget(&buttonCoordinate, 0, 0, 1, 1);
    layoutForm->addWidget(labelCoordinate, 0, 1, 1, 1);
    layoutForm->addWidget(&spinBoxCoordinate, 0, 2, 1, 1);
    layoutForm->addWidget(&sliderCoordinate, 0, 3, 1, 1);
    layoutForm->addWidget(&buttonDirection, 1, 0, 1, 1);
    layoutForm->addWidget(labelDirection, 1, 1, 1, 1);
    layoutForm->addWidget(&spinBoxDirection, 1, 2, 1, 1);
    layoutForm->addWidget(&sliderDirection, 1, 3, 1, 1);

    hide();
}

AlgorithmViewITKBinaryThinningImageFilter::~AlgorithmViewITKBinaryThinningImageFilter() {}

void AlgorithmViewITKBinaryThinningImageFilter::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
    /** TODO : MàJ des widgets en fonction des paramètres de l'algorithme **/
    std::cout << "View - MaJ widgets ..." << std::endl;

    AlgorithmITKBinaryThinningImageFilter<Image, Image> *algorithmSkeleton = (AlgorithmITKBinaryThinningImageFilter<Image, Image> *) algorithm;

    buttonCoordinate.setChecked(true);
    sliderCoordinate.setValue(algorithmSkeleton->getCoordinateTolerance());
    spinBoxCoordinate.setValue(algorithmSkeleton->getCoordinateTolerance());
    buttonDirection.setChecked(false);
    sliderDirection.setValue(algorithmSkeleton->getDirectionTolerance());
    spinBoxDirection.setValue(algorithmSkeleton->getDirectionTolerance());
}

void AlgorithmViewITKBinaryThinningImageFilter::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
    /** TODO : MàJ des paramètres de l'algorithme en fonction des widgets **/
    std::cout << "View - MaJ parametres ..." << std::endl;

    AlgorithmITKBinaryThinningImageFilter<Image, Image> *algorithmSkeleton = (AlgorithmITKBinaryThinningImageFilter<Image, Image> *) algorithm;

    if (buttonCoordinate.isChecked())
        algorithmSkeleton->setCoordinateTolerance(spinBoxCoordinate.value());
    else algorithmSkeleton->setDirectionTolerance(spinBoxDirection.value());
}

QString AlgorithmViewITKBinaryThinningImageFilter::toString()
{
    QString result = "Squelettisation";

    /** TODO : afficher la vue sous forme d'une chaine de caractères **/
    std::cout << "View - Afficher vue chaine de caracteres ..." << std::endl;

    result += "\n -> Coordinate Tolerance : " + QString::number(spinBoxCoordinate.value());
    result += "\n -> Direction Tolerance : " + QString::number(spinBoxDirection.value());

    return result;
}

AlgorithmViewITKBinaryThinningImageFilter * AlgorithmViewITKBinaryThinningImageFilter::copy()
{
    AlgorithmViewITKBinaryThinningImageFilter * copy = new AlgorithmViewITKBinaryThinningImageFilter(spinBoxCoordinate.value(), spinBoxDirection.value());

    std::cout << "View - Copy ..." << std::endl;

    return copy;
}

#endif // ALGORITHMVIEWITKBINARYTHINNINGIMAGEFILTER

