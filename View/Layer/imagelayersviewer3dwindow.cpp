#include "imagelayersviewer3dwindow.h"

#include "../../Model/dgtaltools.h"

#include <QVBoxLayout>
#include <QLabel>

ImageLayersViewer3DWindow::ImageLayersViewer3DWindow(QWidget *parent) : QDialog(parent),
    buttonValid("Valider")
{
    // Ajout des composants de la vue
    QVBoxLayout *layoutLayers = new QVBoxLayout(this);
    QLabel *emptyLayer = new QLabel("Aucune image calque.", parent);
    layoutLayers->addWidget(&comboBoxLayers);
    layoutLayers->addWidget(emptyLayer);
    layoutLayers->addWidget(&buttonValid);
    buttonValid.hide();

    // Connexions
    QObject::connect(&buttonValid, SIGNAL(clicked()), this, SLOT(openViewer3D()));

    // Définition de la fenêtre
    setWindowTitle("OS3D - Visualisation 3D");
    setMinimumWidth(IMAGE_LAYER_VIEWER3D_WINDOW_MINIMUM_WIDTH);
}

void ImageLayersViewer3DWindow::setImageLayers(std::vector<ImageLayer> *imageLayers)
{
    this->imageLayers = imageLayers;
}

void ImageLayersViewer3DWindow::addViewLayer(const ImageLayer &layer)
{
    // Si premier calque alors on supprime le calque vide et on affiche le bouton valider
    if (comboBoxLayers.count() == 0)
    {
        layout()->itemAt(1)->widget()->hide();
        buttonValid.show();
    }

    // Ajout du calque au choix de la liste
    QString nameLayer = layer.getName();
    comboBoxLayers.addItem(nameLayer);
}

void ImageLayersViewer3DWindow::openViewer3D()
{
    int indexLayer = comboBoxLayers.currentIndex();
    ImageLayer imageLayer = (*imageLayers)[indexLayer];

    if (withDGtal)
    {
        //std::cout << "with DGTAL" << std::endl;
        dgtalViewer.clear();
        dgtalViewer.show();

        Image *image = imageLayer.getImage();
        DGtalITKImage dgtalImage = DGtalTools<PixelType>::convertInDGtalITKImage(image, false);
        typename DGtalITKImage::Domain::ConstIterator itBegin = dgtalImage.domain().begin();
        typename DGtalITKImage::Domain::ConstIterator itEnd = dgtalImage.domain().end();

        for (DGtalITKImage::Domain::ConstIterator it=itBegin; it!=itEnd; ++it)
        {
            if (dgtalImage(*it) != 0)
                dgtalViewer << *it;
        }
    }
    else
    {
        //std::cout << "without DGTAL" << std::endl;
        QString windowTitle = "0S3D - Visualistion 3D - Calque : "+imageLayer.getName();
        myViewer.setWindowTitle(windowTitle);
        myViewer.setImageLayer(imageLayer.getImage());
        if (imageLayer.hasGraph())
            myViewer.setGraphLayer(imageLayer.getGraph()->getEEdges(),
                                   imageLayer.getGraph()->getENodes());
        myViewer.show();
    }
}

void ImageLayersViewer3DWindow::removeViewLayer(int indexLayer)
{
    // Suppression du calque dans la liste
    comboBoxLayers.removeItem(indexLayer);

    if (imageLayers->size() == 0)
    {
        // Si la liste de calques est vide on le signale et on cache le bouton valider
        layout()->itemAt(1)->widget()->show();
        buttonValid.hide();
    }
}
