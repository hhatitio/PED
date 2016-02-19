#ifndef ALGORITHMVIEWDGTALDISTANCETRANSFORMATION_H
#define ALGORITHMVIEWDGTALDISTANCETRANSFORMATION_H

#include "../algorithmview.h"

#include <QFormLayout>
#include <QLabel>

class AlgorithmViewDGtalDistanceTransformation : public AlgorithmView
{

public:

    AlgorithmViewDGtalDistanceTransformation(QWidget *parent = 0);
    ~AlgorithmViewDGtalDistanceTransformation();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewDGtalDistanceTransformation * copy();

};

AlgorithmViewDGtalDistanceTransformation::AlgorithmViewDGtalDistanceTransformation(QWidget *parent) :
    AlgorithmView("Distance transformation", parent)
{
    // Construction du formulaire
    QFormLayout *layoutForm = new QFormLayout(this);
    QLabel *label = new QLabel("Aucun paramètres.");
    layoutForm->addWidget(label);
    hide();
}

AlgorithmViewDGtalDistanceTransformation::~AlgorithmViewDGtalDistanceTransformation() { }

#pragma GCC diagnostic ignored "-Wunused-parameter"
void AlgorithmViewDGtalDistanceTransformation::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm) { }

void AlgorithmViewDGtalDistanceTransformation::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm) { }
#pragma GCC diagnostic pop

QString AlgorithmViewDGtalDistanceTransformation::toString()
{
    QString result = "Distance transformation";
    return result;
}

AlgorithmViewDGtalDistanceTransformation * AlgorithmViewDGtalDistanceTransformation::copy()
{
    AlgorithmViewDGtalDistanceTransformation *copy = new AlgorithmViewDGtalDistanceTransformation();
    return copy;
}

#endif // ALGORITHMVIEWDGTALDISTANCETRANSFORMATION_H
