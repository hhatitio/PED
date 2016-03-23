#include "imagelayer.h"

#include <QColorDialog>
#include <QMessageBox>

ImageLayer::ImageLayer()
{
    name = "Pas de nom.";
    image = 0;
    graph = 0;
    hasSkeletonGraph = false;
}

ImageLayer::ImageLayer(const QString &name, Image *layer,SkeletonGraph* skeletonGraph)
{
    this->name = name;
    //const Image3D<short int> im_const = *layer;
    //image = new Image3D<short int>(im_const);
    image = layer;
    
    if (skeletonGraph == NULL) {
        hasSkeletonGraph = false;
        graph = NULL;
    }
    else {
        hasSkeletonGraph = true;
        graph = skeletonGraph;
    }

    // On construit les composants et leurs couleurs
    int numComponent = 1;
    for (Image::iterator it=image->begin(); it!=image->end(); ++it)
    {
        if ((components.find(*it) == components.end()) && *it != 0) {
            QColor color = randColor();
            //QColor color = QColor((*it), (*it), (*it));
            std::pair<PixelType, QColor> component(*it, color);
            components.insert(component);
            ++numComponent;
        }
    }
}

/*ImageLayer::~ImageLayer()
{
    if(graph != 0){
        delete graph;
    }

    if(image != 0){
        delete image;
    }
}*/

QString ImageLayer::getName() const
{
    return name;
}

Image * ImageLayer::getImage() const
{
    return image;
}

std::map<PixelType, QColor> ImageLayer::getComponents() const
{
    return components;
}

void ImageLayer::setComponentColor(PixelType component, const QColor &color)
{
    components[component] = color;
}

void ImageLayer::setImagePixel(const int pixel[Dimension], int indexComponent, int neighborhood)
{
    // Récupération de la valeur du composant
    std::map<PixelType, QColor>::iterator itComponents = components.begin();
    std::advance(itComponents, indexComponent);

    // Attribution du composant au pixel
    image->at(pixel[0], pixel[1], pixel[2]) = itComponents->first;

    // Attribution du composant au voisinage du pixel
    for (int x=pixel[0]-neighborhood; x<pixel[0]+neighborhood; ++x)
        for (int y=pixel[1]-neighborhood; y<pixel[1]+neighborhood; ++y)
            image->at(x, y, pixel[2]) = itComponents->first;
}

void ImageLayer::removeImagePixel(const int pixel[], int indexComponent, int neighborhood)
{
    // Récupération de la valeur du composant
    std::map<PixelType, QColor>::iterator itComponents = components.begin();
    std::advance(itComponents, indexComponent);

    // Si le pixel est égale au composant on le supprime
    if (image->at(pixel[0], pixel[1], pixel[2]) == itComponents->first)
        image->at(pixel[0], pixel[1], pixel[2]) = 0;

    // Suppression du composant au voisinage du pixel
    for (int x=pixel[0]-neighborhood; x<pixel[0]+neighborhood; ++x)
        for (int y=pixel[1]-neighborhood; y<pixel[1]+neighborhood; ++y)
            if (image->at(x, y, pixel[2]) == itComponents->first)
                image->at(x, y, pixel[2]) = 0;
}

QColor ImageLayer::randColor()
{
    // Calcul alétoire des canaux RGB
    int red = rand()%256;
    int green = rand()%256;
    int blue = rand()%256;

    return QColor(red, green, blue);
}

bool ImageLayer::checkSizeComponents(Image *image)
{
    int numComponent = 0;

    std::vector<PixelType> components;

    // Calcul du nombre de composants différents dans l'image
    for (Image::iterator it=image->begin(); it!=image->end(); ++it)
    {
        if ((std::find(components.begin(), components.end(), *it) == components.end()) && *it != 0)
        {
            components.push_back(*it);
            ++numComponent;
        }
    }

    int reply = QMessageBox::question(nullptr, "Validation de la sauvegarde du calque image",
                                      QString("Le calque comporte %1 composant(s). Êtes-vous sûr de vouloir sauvegarder ce calque ? ").arg(numComponent),
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) return true;
    else return false;
}

bool ImageLayer::hasGraph()
{
    return hasSkeletonGraph;
}


SkeletonGraph * ImageLayer::getGraph()
{
    return graph;
}
