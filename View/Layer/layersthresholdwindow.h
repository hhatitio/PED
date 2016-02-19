#ifndef LAYERSTHRESHOLDWINDOW_H
#define LAYERSTHRESHOLDWINDOW_H

#include "../../deftypes.h"
#include "Model/interval.h"
#include "../Spanslider/qxtspanslider.h"

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QImage>

#define LAYERS_THRESOLD_WINDOW_MINIMUM_WIDTH    400

#define COLOR_THRESHOLD_LOWER	qRgb(255, 0, 0)
#define COLOR_THRESHOLD_INSIDE	qRgb(0, 255, 0)
#define COLOR_THRESHOLD_OUTSIDE	qRgb(255, 255, 0)
#define COLOR_THRESHOLD_UPPER	qRgb(0, 0, 255)

enum LayerThresholdType
{
    DEFAULT,
    LOWER,
    UPPER,
    INSIDE,
    OUTSIDE,
    ZMOTION
};

class LayersThresholdWindow : public QDialog
{

    Q_OBJECT

public:

    LayersThresholdWindow(QWidget *parent);

    QImage * getLayer(Image *image, int slice);

    QImage * getDefaultLayer(Image *image, int slice);
    QImage * getLowerLayer(Image *image, int slice);
    QImage * getUpperLayer(Image *image, int slice);
    QImage * getInsideLayer(Image *image, int slice);
    QImage * getOutsideLayer(Image *image, int slice);

    void setDataParameters(PixelType lower, PixelType upper);

private:

    QComboBox layerType;
    QxtSpanSlider sliderIntensity;
    QSpinBox lowerIntensity;
    QSpinBox upperIntensity;
    QSlider zMouvement;

};

#endif // LAYERSTHRESHOLDWINDOW_H
