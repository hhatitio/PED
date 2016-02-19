#ifndef ALGORITHMVIEW_H
#define ALGORITHMVIEW_H

#include "../../Model/Algorithm/algorithm.h"

#include <QGroupBox>
#include <QString>

class AlgorithmView : public QGroupBox
{

public:

    AlgorithmView(const QString &name, QWidget *parent = 0) : QGroupBox(parent)
    {
        this->name = name;
    }

    virtual ~AlgorithmView() { }

    void setName(QString name) { this->name = name; }
    QString getName() { return name; }

    virtual void updateParametersWithAlgorithm(Algorithm<Image, Image> *algorithm) = 0;
    virtual void updateAlgorithmParameters(Algorithm<Image, Image> *algorithm) = 0;

    virtual QString toString() = 0;

    virtual AlgorithmView * copy() = 0;

private:

    QString name;

};

#endif // ALGORITHMVIEW_H
