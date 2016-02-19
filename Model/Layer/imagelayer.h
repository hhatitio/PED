#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include "../../deftypes.h"

#include <QString>
#include <QColor>
#include <QImage>
#include <QPoint>

class ImageLayer
{

public:

    // Dimension de l'image calque

    static const unsigned int Dimension = Image::Dimension;

    ImageLayer();
    ImageLayer(const QString &name, Image *image);

    QString getName() const;
    Image * getImage() const;
    std::map<PixelType, QColor> getComponents() const;

    void setComponentColor(PixelType component, const QColor &color);

    void setImagePixel(const int pixel[Dimension], int indexComponent, int neighborhood = 0);
    void removeImagePixel(const int pixel[Dimension], int indexComponent, int neighborhood = 0);

    static QColor randColor();
    static bool checkSizeComponents(Image *image);

private:

    QString name;
    Image *image;
    std::map<PixelType, QColor> components;

};

#endif // IMAGELAYER_H
