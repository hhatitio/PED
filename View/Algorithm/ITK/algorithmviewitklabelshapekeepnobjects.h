#ifndef ALGORITHMVIEWITKLABELSHAPEKEEPNOBJECTS_H
#define ALGORITHMVIEWITKLABELSHAPEKEEPNOBJECTS_H

#include "../algorithmview.h"
#include "../../../Model/Algorithm/ITK/algorithmitklabelshapekeepnobjects.h"

#include <QFormLayout>
#include <QSpinBox>
#include <QComboBox>

#include <itkLabelShapeKeepNObjectsImageFilter.h>

class AlgorithmViewITKLabelShapeKeepNObjects : public AlgorithmView
{

    // Label utiliser pour les attributs
    typedef typename AlgorithmITKLabelShapeKeepNObjects<ITKImage>::LabelObjectType LabelObjectFilterType;

public:

    AlgorithmViewITKLabelShapeKeepNObjects(int maxValue, QWidget *parent = 0);
    ~AlgorithmViewITKLabelShapeKeepNObjects();

    void buildComboBoxAttributes();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewITKLabelShapeKeepNObjects * copy();

private:

    QSpinBox backgroundValue;
    QSpinBox numberOfObjects;
    QComboBox attribute;

};

AlgorithmViewITKLabelShapeKeepNObjects::AlgorithmViewITKLabelShapeKeepNObjects(int maxValue, QWidget *parent) :
    AlgorithmView("Sélection de composantes connexes", parent)
{
    // Construction du formulaire
    QFormLayout *layoutForm = new QFormLayout(this);
    backgroundValue.setMaximum(maxValue);
    buildComboBoxAttributes();
    layoutForm->addRow("Valeur arrière-plan", &backgroundValue);
    layoutForm->addRow("Nombre de composantes", &numberOfObjects);
    layoutForm->addRow("Attribut", &attribute);
    hide();
}

AlgorithmViewITKLabelShapeKeepNObjects::~AlgorithmViewITKLabelShapeKeepNObjects() { }

void AlgorithmViewITKLabelShapeKeepNObjects::buildComboBoxAttributes()
{
    // Ajout de l'ensemble des attibuts du filtre
    attribute.addItem("Nombre de pixels", QVariant(100));
    attribute.addItem("Taille physique", QVariant(101));
    attribute.addItem("Centre", QVariant(104));
    attribute.addItem("Boîte englobante", QVariant(105));
    attribute.addItem("Nombre de pixels au bord", QVariant(106));
    attribute.addItem("Périmètre au bord", QVariant(107));
    attribute.addItem("Diamètre", QVariant(108));
    attribute.addItem("Moments principaux", QVariant(109));
    attribute.addItem("Axes principaux", QVariant(110));
    attribute.addItem("Allongement", QVariant(111));
    attribute.addItem("Périmètre", QVariant(112));
    attribute.addItem("Rondeur", QVariant(113));
    attribute.addItem("Équivalent rayon sphérique", QVariant(114));
    attribute.addItem("Équivalent périmètre sphérique", QVariant(115));
    attribute.addItem("Équivalent diamètre ellipsoïde", QVariant(116));
    attribute.addItem("Forme plate", QVariant(117));
    attribute.addItem("Ratio périmètre au bord", QVariant(118));
}

void AlgorithmViewITKLabelShapeKeepNObjects::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKLabelShapeKeepNObjects<Image> *algorithmLabelShape = (AlgorithmITKLabelShapeKeepNObjects<Image> *) algorithm;
    backgroundValue.setValue(algorithmLabelShape->getBackgroundValue());
    numberOfObjects.setValue(algorithmLabelShape->getNumberOfObjects());
    attribute.setCurrentIndex(algorithmLabelShape->getAttribute());
}

void AlgorithmViewITKLabelShapeKeepNObjects::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKLabelShapeKeepNObjects<Image> *algorithmLabelShape = (AlgorithmITKLabelShapeKeepNObjects<Image> *) algorithm;
    algorithmLabelShape->setBackgroundValue(backgroundValue.value());
    algorithmLabelShape->setNumberOfObjects(numberOfObjects.value());
    algorithmLabelShape->setAttribute(attribute.currentIndex());
}

QString AlgorithmViewITKLabelShapeKeepNObjects::toString()
{
    QString result = "Sélection de composantes connexes";
    result += "\n -> Valeur de l'arrière-plan : " + QString::number(backgroundValue.value());
    result += "\n -> Nombre de composantes : " + QString::number(numberOfObjects.value());
    result += "\n -> Attribut : " + QString::fromStdString(LabelObjectFilterType::GetNameFromAttribute(attribute.currentIndex()));
    return result;
}

AlgorithmViewITKLabelShapeKeepNObjects * AlgorithmViewITKLabelShapeKeepNObjects::copy()
{
    AlgorithmViewITKLabelShapeKeepNObjects *copy = new AlgorithmViewITKLabelShapeKeepNObjects(backgroundValue.maximum());
    return copy;
}

#endif // ALGORITHMVIEWITKLABELSHAPEKEEPNOBJECTS_H
