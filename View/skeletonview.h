#ifndef SKELETONVIEW
#define SKELETONVIEW

#include "../deftypes.h"
#include "Model/skeletonmodel.h"

#include <iostream>
#include <QString>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QPushButton>

class SkeletonView: public QDialog
{

public:
    SkeletonView(QWidget *parent);
    ~SkeletonView();

    void setFilename(QString filename);
    void setImage(Image *image);
    void skeletonization();

private:
    SkeletonModel *skeletonModel;
    QString filename;
    QSpinBox distCharges, fieldStrenght, percHDPts;
    Image *image;
};

#endif // SKELETONVIEW

