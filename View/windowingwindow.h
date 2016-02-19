#ifndef WINDOWINGWINDOW_H
#define WINDOWINGWINDOW_H

#include "../deftypes.h"
#include "../Model/interval.h"

#include <QDialog>
#include <QSpinBox>

#define WINDOWING_MINIMUM_WIDTH  200

class WindowingWindow : public QDialog
{
    Q_OBJECT

public:

    WindowingWindow(QWidget *parent);
    ~WindowingWindow();

    void buildWindowing(Interval<PixelType> *interval);

public slots:

    void changeWindowing();
    void resetWindowing();

private:

    QSpinBox centerWindowing;
    QSpinBox widthWindowing;

    Interval<PixelType> intervalIntensity;
    Interval<PixelType> *currentIntervalIntensity;

};

#endif // WINDOWINGWINDOW_H
