#include "View/imagescroll.h"

#include <QScrollBar>

ImageScroll::ImageScroll(QWidget* parent) : QScrollArea(parent)
{
    label.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label.setScaledContents(true);
    label.installEventFilter(&zoomer);

    setBackgroundRole(QPalette::Dark);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setLineWidth(0);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setWidgetResizable(false);
    setAlignment(Qt::AlignCenter);
    setWidget(&label);

    QObject::connect(&zoomer, SIGNAL(zoomFactorChanged(qreal,qreal)), this, SLOT(zoom(qreal,qreal)));
    QObject::connect(&zoomer, SIGNAL(isMovedFrom(QPoint)), this, SLOT(drag(QPoint)));
}

ImageScroll::~ImageScroll() { }

void ImageScroll::installLabelEventFilter(QObject* object)
{
    label.installEventFilter(object);
}

const QLabel& ImageScroll::getLabel() const
{
    return label;
}

const QImage& ImageScroll::getImage() const
{
    return qImage;
}

const Zoomer& ImageScroll::getZoomer() const
{
    return zoomer;
}

QImage& ImageScroll::getImage()
{
    return qImage;
}

void ImageScroll::updateLabel()
{
    if (!qImage.isNull())
    {
        label.resize(zoomer.getFactor() * qImage.size());
        label.setPixmap(QPixmap::fromImage(qImage).scaledToWidth(zoomer.getFactor()*qImage.width(), Qt::FastTransformation));
    }
}

void ImageScroll::updateImage(QImage *image)
{
    qImage = *image;
}

void ImageScroll::updateImage(Image *image, int slice, const Interval<PixelType> &intervalIntensity)
{
    resizeBlackImage(image->n_rows, image->n_cols);

    const PixelType minIntensity = intervalIntensity.min();
    const qreal fact = 255./intervalIntensity.count();

    QRgb *line = (QRgb*)(qImage.bits()+qImage.byteCount());
    Image::slice_iterator itBegin = image->begin_slice(slice);
    Image::slice_iterator itEnd = image->end_slice(slice);

    for (Image::slice_iterator it = itBegin; it != itEnd; ++it)
    {
        PixelType color = (intervalIntensity.restrictedValue(*it)-minIntensity)*fact;
        *(line--) = qRgb(color, color, color);
    }
}

void ImageScroll::addImageLayer(const ImageLayer &layer, int slice)
{
    QRgb *line = (QRgb*)(qImage.bits()+qImage.byteCount());
    Image::slice_iterator itBegin = layer.getImage()->begin_slice(slice);
    Image::slice_iterator itEnd = layer.getImage()->end_slice(slice);

    std::map<PixelType, QColor> components = layer.getComponents();

    for (Image::slice_iterator it = itBegin; it != itEnd; ++it)
    {
        if (*it != 0)
            *(line--) = components[*it].rgb();
        else
            line--;
    }
}

void ImageScroll::resizeBlackImage(const int& width, const int& height)
{
    qImage = QImage(width, height, QImage::Format_ARGB32);
    qImage.fill(0xff000000);
}

void ImageScroll::zoom(const qreal zoomFactor, const qreal zoomCoefficient)
{
    label.resize(zoomFactor * qImage.size());
    label.setPixmap(QPixmap::fromImage(qImage).scaledToWidth(zoomer.getFactor()*qImage.width(), Qt::FastTransformation));
    QScrollBar *hBar = this->horizontalScrollBar();
    hBar->setValue(int(zoomCoefficient * hBar->value() + ((zoomCoefficient - 1) * hBar->pageStep()/2)));
    QScrollBar *vBar = this->verticalScrollBar();
    vBar->setValue(int(zoomCoefficient * vBar->value() + ((zoomCoefficient - 1) * vBar->pageStep()/2)));
}

void ImageScroll::drag(const QPoint movementVector)
{
    QScrollArea& scrollArea = *this;
    if (movementVector.x())
    {
        if (movementVector.x() > 0)
            scrollArea.horizontalScrollBar()->setValue(scrollArea.horizontalScrollBar()->value()-movementVector.x()+1);
        else
            scrollArea.horizontalScrollBar()->setValue(scrollArea.horizontalScrollBar()->value()-movementVector.x()-1);
    }
    if (movementVector.y())
    {
        if (movementVector.y() > 0)
            scrollArea.verticalScrollBar()->setValue(scrollArea.verticalScrollBar()->value()-movementVector.y()+1);
        else
            scrollArea.verticalScrollBar()->setValue(scrollArea.verticalScrollBar()->value()-movementVector.y()-1);
    }
}
