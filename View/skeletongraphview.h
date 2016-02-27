#ifndef SKELETONGRAPHVIEW
#define SKELETONGRAPHVIEW


#include "../deftypes.h"
#include "Model/skeletongraph.h"

#include <iostream>
#include <QString>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QPushButton>

class SkeletonGraphView: public QDialog
{

public:
    SkeletonGraphView(QWidget *parent);
    ~SkeletonGraphView();
    void showGraph();

private:
    SkeletonGraph *skeletongraph;
    QString filename;
    QSpinBox distCharges, fieldStrenght, percHDPts;
    Image *image;
};


#endif // SKELETONGRAPHVIEW

