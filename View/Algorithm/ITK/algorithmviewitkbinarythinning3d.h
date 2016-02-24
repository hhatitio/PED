#ifndef ALGORITHMVIEWITKBINARYTHINNING3D
#define ALGORITHMVIEWITKBINARYTHINNING3D

#include "../algorithmview.h"
#include "../../Spanslider/qxtspanslider.h"
#include "../../../Model/Algorithm/ITK/algorithmitkbinarythinning3d.h"

#include <iostream>

#include <QRadioButton>
#include <QGridLayout>
#include <QSlider>
#include <QSpinBox>
#include <QLabel>

class AlgorithmViewITKBinaryThinning3D : public AlgorithmView
{
public:

    AlgorithmViewITKBinaryThinning3D(QWidget *parent = 0);
    ~AlgorithmViewITKBinaryThinning3D();

    void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm);
    void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm);

    QString toString();

    AlgorithmViewITKBinaryThinning3D * copy();
};

AlgorithmViewITKBinaryThinning3D::AlgorithmViewITKBinaryThinning3D(QWidget *parent) :
    AlgorithmView("Squelettisation 3D", parent)
{
    hide();
}

AlgorithmViewITKBinaryThinning3D::~AlgorithmViewITKBinaryThinning3D() {}

void AlgorithmViewITKBinaryThinning3D::updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm)
{
}

void AlgorithmViewITKBinaryThinning3D::updateAlgorithmParameters(Algorithm<Image, Image> *algorithm)
{
}

QString AlgorithmViewITKBinaryThinning3D::toString()
{
    QString result = "Squelettisation 3D";

    return result;
}

AlgorithmViewITKBinaryThinning3D * AlgorithmViewITKBinaryThinning3D::copy()
{
    AlgorithmViewITKBinaryThinning3D * copy = new AlgorithmViewITKBinaryThinning3D();

    return copy;
}

#endif // ALGORITHMVIEWITKBINARYTHINNING3D

