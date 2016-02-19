#include "imagelayerstoolswindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>

ImageLayersToolsWindow::ImageLayersToolsWindow(QWidget *parent) : QDialog(parent),
    buttonPencil("Crayon", this), buttonEraser("Gomme", this)
{
    // Ajout des composants de la vue
    QVBoxLayout *layoutLayers = new QVBoxLayout(this);
    QLabel *emptyLayer = new QLabel("Aucune image calque.", parent);
    stackedWidgetLayers.addWidget(emptyLayer);
    stackedWidgetLayers.setCurrentWidget(emptyLayer);
    QHBoxLayout *layoutToolsButton = new QHBoxLayout();
    layoutToolsButton->addWidget(&buttonPencil);
    layoutToolsButton->addWidget(&buttonEraser);
    layoutLayers->addWidget(&comboBoxLayers);
    layoutLayers->addWidget(&stackedWidgetLayers);
    layoutLayers->addLayout(layoutToolsButton);
    buttonPencil.hide();
    buttonEraser.hide();

    // Connexions
    QObject::connect(&comboBoxLayers, SIGNAL(currentIndexChanged(int)), &stackedWidgetLayers, SLOT(setCurrentIndex(int)));
    QObject::connect(&comboBoxLayers, SIGNAL(currentIndexChanged(int)), parent, SLOT(drawSlice()));
    QObject::connect(&buttonPencil, SIGNAL(clicked()), this, SLOT(setType()));
    QObject::connect(&buttonEraser, SIGNAL(clicked()), this, SLOT(setType()));

    // Définition de la fenêtre
    setWindowTitle("OS3D - Outils calques");
    setMinimumWidth(IMAGE_LAYER_TOOLS_WINDOW_MINIMUM_WIDTH);
}

void ImageLayersToolsWindow::setImageLayers(std::vector<ImageLayer> *imageLayers)
{
    this->imageLayers = imageLayers;
}

ImageLayer ImageLayersToolsWindow::getCurrentImageLayer()
{
    return (*imageLayers)[comboBoxLayers.currentIndex()];
}

ImageLayersToolsWindow::ToolType ImageLayersToolsWindow::getType()
{
    return type;
}

void ImageLayersToolsWindow::addViewLayer(const ImageLayer &layer)
{
    // Si premier calque alors on supprime le calque vide et on affiche les outils
    if (comboBoxLayers.count() == 0)
    {
        stackedWidgetLayers.removeWidget(stackedWidgetLayers.widget(0));
        buttonPencil.show();
        buttonEraser.show();
    }

    // Ajout du calque au choix de la liste
    QString nameLayer = layer.getName();
    comboBoxLayers.addItem(nameLayer);

    // Création de la vue du calque
    QGroupBox *groupLayer = new QGroupBox(nameLayer, this);
    QGridLayout *layoutLayer = new QGridLayout(groupLayer);
    std::map<PixelType, QColor> components = layer.getComponents();
    int indexComponent = 1;
    QLabel *labelComponents = new QLabel("Composant : ", groupLayer);
    QComboBox *comboBoxComponents = new QComboBox(groupLayer);
    // Ajout de chaque composants dans la comboBoxComponents
    for (std::map<PixelType, QColor>::iterator itComponent=components.begin(); itComponent!=components.end(); ++itComponent, ++indexComponent)
    {
        comboBoxComponents->addItem(QString::number(indexComponent), itComponent->first);
    }
    comboBoxesComponentsLayers.push_back(comboBoxComponents);
    // Choix de la taille de l'outil
    QLabel *labelSizeTool = new QLabel("Taille : ", this);
    QComboBox *comboBoxSizeTool = new QComboBox(this);
    for (int size=1; size <6; ++size)
        comboBoxSizeTool->addItem(QString::number(size), size);
    comboBoxesSizesToolLayers.push_back(comboBoxSizeTool);
    layoutLayer->addWidget(labelComponents, 0, 0, 1, 1);
    layoutLayer->addWidget(comboBoxComponents, 0, 1, 1, 1);
    layoutLayer->addWidget(labelSizeTool, 1, 0, 1, 1);
    layoutLayer->addWidget(comboBoxSizeTool, 1, 1, 1, 1);

    // Ajout de la vue à la pile
    stackedWidgetLayers.addWidget(groupLayer);
}

void ImageLayersToolsWindow::apply(const QPoint &position, int slice)
{
    if (!imageLayers->empty())
    {
        int pixel[ImageLayer::Dimension];
        int imageWidth = (*imageLayers)[comboBoxLayers.currentIndex()].getImage()->n_cols;
        int imageHeight = (*imageLayers)[comboBoxLayers.currentIndex()].getImage()->n_rows;

        // Vérification que la souris est toujours dans l'image
        if (0 < position.x() && position.x() < imageHeight && 0 < position.y() && position.y() < imageWidth)
        {
            // Cordonnées inverse pour l'image
            pixel[0] = std::abs((int)(position.x()-imageHeight));
            pixel[1] = std::abs((int)(position.y()-imageWidth));
            pixel[2] = slice;

            // Récupération des données du formulaire
            int indexLayer = comboBoxLayers.currentIndex();
            int indexComponent = comboBoxesComponentsLayers[indexLayer]->currentIndex();
            int size = comboBoxesSizesToolLayers[indexLayer]->itemData(comboBoxesSizesToolLayers[indexLayer]->currentIndex()).toInt();

            switch (type)
            { // Application de l'outil choisi au calque
            case Pencil : (*imageLayers)[indexLayer].setImagePixel(pixel, indexComponent, size); break;
            case Eraser : (*imageLayers)[indexLayer].removeImagePixel(pixel, indexComponent, size); break;
            case Empty : break;
            }
        }
    }
}

void ImageLayersToolsWindow::setType()
{
    if (!imageLayers->empty())
    {
        if (buttonPencil.isChecked())
            type = Pencil;
        else if (buttonEraser.isChecked())
            type = Eraser;
    }
    else
    {
        type = Empty;
    }
}

void ImageLayersToolsWindow::removeViewLayer(int indexLayer)
{
    // Suppression des composants en liaison avec le calque
    comboBoxLayers.removeItem(indexLayer);
    comboBoxesComponentsLayers.erase(comboBoxesComponentsLayers.begin()+indexLayer);
    comboBoxesSizesToolLayers.erase(comboBoxesSizesToolLayers.begin()+indexLayer);
    stackedWidgetLayers.removeWidget(stackedWidgetLayers.widget(indexLayer));

    if (imageLayers->size() == 0)
    {
        // Si la liste de calques est vide on le signale et on cache les outils
        QLabel *emptyLayer = new QLabel("Aucune image calque.", parentWidget());
        stackedWidgetLayers.addWidget(emptyLayer);
        stackedWidgetLayers.setCurrentWidget(emptyLayer);
        buttonPencil.hide();
        buttonEraser.hide();
    }
}
