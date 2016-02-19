#ifndef ALGORITHMVIEWITKCONNECTEDCOMPONENT_H
#define ALGORITHMVIEWITKCONNECTEDCOMPONENT_H

#include "../algorithmview.h"
#include "../../../Model/Algorithm/ITK/algorithmitkconnectedcomponent.h"

#include <QFormLayout>
#include <QCheckBox>

class AlgorithmViewITKConnectedComponent : public AlgorithmView
{

public:

    AlgorithmViewITKConnectedComponent(QWidget *parent = 0);
    ~AlgorithmViewITKConnectedComponent();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewITKConnectedComponent * copy();

private:

    QCheckBox fullyConnected;

};

AlgorithmViewITKConnectedComponent::AlgorithmViewITKConnectedComponent(QWidget *parent) : AlgorithmView("Composantes connexes", parent)
{
    // Construction du formulaire
    QFormLayout *layoutForm = new QFormLayout(this);
    layoutForm->addRow("Entièrement connexe", &fullyConnected);
    hide();
}

AlgorithmViewITKConnectedComponent::~AlgorithmViewITKConnectedComponent() { }

void AlgorithmViewITKConnectedComponent::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKConnectedComponent<Image, Image> *algorithmConnectedComponent = (AlgorithmITKConnectedComponent<Image, Image> *) algorithm;
    fullyConnected.setChecked(algorithmConnectedComponent->getFullyConnected());
}

void AlgorithmViewITKConnectedComponent::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
    AlgorithmITKConnectedComponent<Image, Image> *algorithmConnectedComponent = (AlgorithmITKConnectedComponent<Image, Image> *) algorithm;
    algorithmConnectedComponent->setFullyConnected(fullyConnected.isChecked());
}

QString AlgorithmViewITKConnectedComponent::toString()
{
    QString result = "Composantes connexes";
    result += "\n -> Entièrement connexe : ";
    result += (fullyConnected.isChecked()) ? "true" : "false";
    return result;
}

AlgorithmViewITKConnectedComponent * AlgorithmViewITKConnectedComponent::copy()
{
    AlgorithmViewITKConnectedComponent *copy = new AlgorithmViewITKConnectedComponent();
    return copy;
}

#endif // ALGORITHMVIEWITKCONNECTEDCOMPONENT_H
