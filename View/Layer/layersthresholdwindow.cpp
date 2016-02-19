#include "layersthresholdwindow.h"

#include "../Spanslider/qxtspanslider.h"

#include <QGridLayout>
#include <QLabel>

LayersThresholdWindow::LayersThresholdWindow(QWidget *parent) : QDialog(parent), sliderIntensity(Qt::Horizontal)
{
    // Initialisation du choix du calque
    layerType.addItem("Par défault");
    layerType.addItem("Seuil inférieur");
    layerType.addItem("Seuil supérieur");
    layerType.addItem("Dans l'intervalle");
    layerType.addItem("En dehors de l'intervalle");

    // Initialisation du formulaire
    QGridLayout *layoutIntensity = new QGridLayout(this);
    QLabel *labelLayer = new QLabel("Type d'affichage : ", parent);
    QLabel *labelIntensity = new QLabel("Interval d'intensité : ", parent);
    layoutIntensity->addWidget(labelLayer, 0, 0, 1, 1);
    layoutIntensity->addWidget(&layerType, 0, 1, 1, 3);
    layoutIntensity->addWidget(labelIntensity, 1, 0, 1, 1);
    layoutIntensity->addWidget(&lowerIntensity, 1, 1, 1, 1);
    layoutIntensity->addWidget(&sliderIntensity, 1, 2, 1, 1);
    layoutIntensity->addWidget(&upperIntensity, 1, 3, 1, 1);

    // Connexions
    QObject::connect(&layerType, SIGNAL(currentIndexChanged(int)), parent, SLOT(drawSlice()));
    QObject::connect(&sliderIntensity, SIGNAL(lowerValueChanged(int)), &lowerIntensity, SLOT(setValue(int)));
    QObject::connect(&sliderIntensity, SIGNAL(upperValueChanged(int)), &upperIntensity, SLOT(setValue(int)));
    QObject::connect(&lowerIntensity, SIGNAL(valueChanged(int)), parent, SLOT(drawSlice()));
    QObject::connect(&upperIntensity, SIGNAL(valueChanged(int)), parent, SLOT(drawSlice()));
    QObject::connect(this, SIGNAL(finished(int)), parent, SLOT(drawSlice()));

    // Initialisation de la fenêtre
    setWindowTitle("OS3D - Calques seuillage");
    setMinimumWidth(LAYERS_THRESOLD_WINDOW_MINIMUM_WIDTH);
}

QImage * LayersThresholdWindow::getLayer(Image *image, int slice)
{
    switch (layerType.currentIndex())
    {
        // Calcul du calque en fonction du type choisi
        case LayerThresholdType::DEFAULT: return getDefaultLayer(image, slice);
        case LayerThresholdType::LOWER: return getLowerLayer(image, slice);
        case LayerThresholdType::UPPER: return getUpperLayer(image, slice);
        case LayerThresholdType::INSIDE: return getInsideLayer(image,slice);
        case LayerThresholdType::OUTSIDE: return getOutsideLayer(image, slice);
        default: std::cerr << "Index de calque inconnu." << std::endl;
            return nullptr;
    }
}

QImage * LayersThresholdWindow::getDefaultLayer(Image *image, int slice)
{
    QImage *qImage = new QImage(image->n_rows, image->n_cols, QImage::Format_ARGB32);

    Interval<PixelType> intervalIntensity (lowerIntensity.value(), upperIntensity.value());
    const PixelType minIntensity = intervalIntensity.min();
    const qreal fact = 255./intervalIntensity.count();

    QRgb *line = (QRgb*)(qImage->bits()+qImage->byteCount());
    Image::slice_iterator itBegin = image->begin_slice(slice);
    Image::slice_iterator itEnd = image->end_slice(slice);

    // Remplissage de l'image restreint par l'interval d'intensité choisi
    for (Image::slice_iterator it = itBegin; it != itEnd; ++it) {
        PixelType color = (intervalIntensity.restrictedValue(*it)-minIntensity)*fact;
        *(line--) = qRgb(color, color, color);
    }

    return qImage;
}

QImage * LayersThresholdWindow::getLowerLayer(Image *image, int slice)
{
    QImage *qImage = new QImage(image->n_rows, image->n_cols, QImage::Format_ARGB32);

    Interval<PixelType> intervalIntensity (image->min(), image->max());
    const PixelType minIntensity = lowerIntensity.value();
    const qreal fact = 255./intervalIntensity.count();

    QRgb *line = (QRgb*)(qImage->bits()+qImage->byteCount());
    Image::slice_iterator itBegin = image->begin_slice(slice);
    Image::slice_iterator itEnd = image->end_slice(slice);

    // Création d'un calque comprenant les valeurs inférieurs à l'intensité minimale
    for (Image::slice_iterator it = itBegin; it != itEnd; ++it) {
        PixelType color = (intervalIntensity.restrictedValue(*it)-intervalIntensity.min())*fact;
        if (color < minIntensity)
            *(line--) = COLOR_THRESHOLD_LOWER;
        else
            *(line--) = qRgb(color, color, color);
    }

    return qImage;
}

QImage * LayersThresholdWindow::getUpperLayer(Image *image, int slice)
{
    QImage *qImage = new QImage(image->n_rows, image->n_cols, QImage::Format_ARGB32);

    Interval<PixelType> intervalIntensity (image->min(), image->max());
    const PixelType maxIntensity = upperIntensity.value();
    const qreal fact = 255./intervalIntensity.count();

    QRgb *line = (QRgb*)(qImage->bits()+qImage->byteCount());
    Image::slice_iterator itBegin = image->begin_slice(slice);
    Image::slice_iterator itEnd = image->end_slice(slice);

    // Création d'un calque comprenant les valeurs supérieurs à l'intensité maximale
    for (Image::slice_iterator it = itBegin; it != itEnd; ++it) {
        PixelType color = (intervalIntensity.restrictedValue(*it)-intervalIntensity.min())*fact;
        if (color > maxIntensity)
            *(line--) = COLOR_THRESHOLD_UPPER;
        else
            *(line--) = qRgb(color, color, color);
    }

    return qImage;
}

QImage * LayersThresholdWindow::getInsideLayer(Image *image, int slice)
{
    QImage *qImage = new QImage(image->n_rows, image->n_cols, QImage::Format_ARGB32);

    Interval<PixelType> intervalIntensity (image->min(), image->max());
    const PixelType minIntensity = lowerIntensity.value();
    const PixelType maxIntensity = upperIntensity.value();
    const qreal fact = 255./intervalIntensity.count();

    QRgb *line = (QRgb*)(qImage->bits()+qImage->byteCount());
    Image::slice_iterator itBegin = image->begin_slice(slice);
    Image::slice_iterator itEnd = image->end_slice(slice);

    // Création d'un calque comprenant les valeurs comprises dans l'intervale d'intensité
    for (Image::slice_iterator it = itBegin; it != itEnd; ++it) {
        PixelType color = (intervalIntensity.restrictedValue(*it)-intervalIntensity.min())*fact;
        if (minIntensity <= color && color <= maxIntensity)
            *(line--) = COLOR_THRESHOLD_INSIDE;
        else
            *(line--) = qRgb(color, color, color);
    }

    return qImage;
}

QImage * LayersThresholdWindow::getOutsideLayer(Image *image, int slice)
{
    QImage *qImage = new QImage(image->n_rows, image->n_cols, QImage::Format_ARGB32);

    Interval<PixelType> intervalIntensity (image->min(), image->max());
    const PixelType minIntensity = lowerIntensity.value();
    const PixelType maxIntensity = upperIntensity.value();
    const qreal fact = 255./intervalIntensity.count();

    QRgb *line = (QRgb*)(qImage->bits()+qImage->byteCount());
    Image::slice_iterator itBegin = image->begin_slice(slice);
    Image::slice_iterator itEnd = image->end_slice(slice);

    // Création d'un calque comprenant les valeurs non comprises dans l'intervale d'intensité
    for (Image::slice_iterator it = itBegin; it != itEnd; ++it) {
        PixelType color = (intervalIntensity.restrictedValue(*it)-intervalIntensity.min())*fact;
        if (color < minIntensity || color > maxIntensity)
            *(line--) = COLOR_THRESHOLD_OUTSIDE;
        else
            *(line--) = qRgb(color, color, color);
    }

    return qImage;
}

void LayersThresholdWindow::setDataParameters(PixelType lower, PixelType upper)
{
    // Mise à jour du seuil inférieur
    lowerIntensity.setRange(lower, upper);
    lowerIntensity.setValue(lower);

    // Mise à jour du seuil supérieur
    upperIntensity.setRange(lower, upper);
    upperIntensity.setValue(upper);

    // Mise à jour du slider
    sliderIntensity.setRange(lower, upper);
    sliderIntensity.setLowerValue(lower);
    sliderIntensity.setUpperValue(upper);
}
