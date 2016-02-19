#ifndef IMAGESCROLL_H
#define IMAGESCROLL_H

#include "deftypes.h"
#include "Model/zoomer.h"
#include "Model/interval.h"
#include "Model/Layer/imagelayer.h"

#include <QScrollArea>
#include <QLabel>
#include <QImage>
#include <QRgb>

class ImageScroll : public QScrollArea
{
    Q_OBJECT

public:

    explicit ImageScroll(QWidget* parent = 0);
    virtual ~ImageScroll();

    const QLabel& getLabel() const;
    const QImage& getImage() const;
    const Zoomer& getZoomer() const;

    QImage& getImage();

    void installLabelEventFilter(QObject* object);

    void updateLabel();
    void updateImage(QImage *image);
    void updateImage(Image *image, int slice, const Interval<PixelType> &intervalIntensity);
    void addImageLayer(const ImageLayer &layer, int slice);
    void resizeBlackImage(const int &width, const int &height);

private slots:

    void zoom(const qreal zoomFactor, const qreal zoomCoefficient);
    void drag(const QPoint movementVector);

private:

    QLabel label;
    QImage qImage;
    Zoomer zoomer;
};

#endif // IMAGESCROLL_H
