#include "imagelayerswindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QColor>
#include <QColorDialog>

ImageLayersWindow::ImageLayersWindow(QWidget *parent) : QDialog(parent)
{
    // Ajout des composants de la vue
    QVBoxLayout *layoutLayers = new QVBoxLayout(this);
    QLabel *emptyLayer = new QLabel("Aucune image calque.", parent);
    stackedWidgetLayers.addWidget(emptyLayer);
    stackedWidgetLayers.setCurrentWidget(emptyLayer);
    layoutLayers->addWidget(&comboBoxLayers);
    layoutLayers->addWidget(&stackedWidgetLayers);

    // Connexions
    QObject::connect(&comboBoxLayers, SIGNAL(currentIndexChanged(int)), &stackedWidgetLayers, SLOT(setCurrentIndex(int)));

    // Définition de la fenêtre
    setWindowTitle("OS3D - Gestion des images calques");
    setMinimumWidth(IMAGE_LAYER_WINDOW_MINIMUM_WIDTH);
}

void ImageLayersWindow::setImageLayers(std::vector<ImageLayer> *imageLayers)
{
    // Mise à jour des images calques
    this->imageLayers = imageLayers;

    // On supprime les anciennes vues de calques
    while (stackedWidgetLayers.count() != 0)
        stackedWidgetLayers.removeWidget(stackedWidgetLayers.widget(0));

    if (imageLayers->size() == 0)
    {
        // Si la liste de calques est vide on le signale
        QLabel *emptyLayer = new QLabel("Aucune image calque.", parentWidget());
        stackedWidgetLayers.addWidget(emptyLayer);
        stackedWidgetLayers.setCurrentWidget(emptyLayer);
    }
    else
    {
        // Sinon on ajoute les nouvelles vues de calques
        for (std::vector<ImageLayer>::iterator itLayer = imageLayers->begin(); itLayer != imageLayers->end(); ++itLayer)
            addViewLayer(*itLayer);
    }
}

void ImageLayersWindow::addViewLayer(const ImageLayer &layer)
{
    // Si premier calque alors on supprime le message de calque vide
    if (comboBoxLayers.count() == 0)
        stackedWidgetLayers.removeWidget(stackedWidgetLayers.widget(0));

    // Ajout du calque au choix de la liste
    QString nameLayer = layer.getName();
    comboBoxLayers.addItem(nameLayer);

    // Création de la vue du calque
    QGroupBox *groupLayer = new QGroupBox(nameLayer, this);
    QGridLayout *layoutLayer = new QGridLayout(groupLayer);
    std::map<PixelType, QColor> components = layer.getComponents();
    int indexComponent = 1;
    QLabel *labelComponents = new QLabel("Composant : ", groupLayer);
    QPushButton *colorComponents = new QPushButton("Changer couleur", groupLayer);
    QComboBox *comboBoxComponents = new QComboBox(groupLayer);
    // Ajout de chaque composants dans la comboBoxComponents
    for (std::map<PixelType, QColor>::iterator itComponent=components.begin(); itComponent!=components.end(); ++itComponent, ++indexComponent)
    {
        comboBoxComponents->addItem(QString::number(indexComponent), itComponent->first);
    }
    comboBoxesComponentsLayers.push_back(comboBoxComponents);
    layoutLayer->addWidget(labelComponents, 0, 0, 1, 1);
    layoutLayer->addWidget(comboBoxComponents, 0, 1, 1, 1);
    layoutLayer->addWidget(colorComponents, 0, 2, 1, 1);

    // Création du bouton de suppression
    QPushButton *deleteLayer = new QPushButton("Supprimer", groupLayer);
    layoutLayer->addWidget(deleteLayer, 1, 0, 1, 3);

    // Connexion des boutons
    QObject::connect(colorComponents, SIGNAL(clicked()), this, SLOT(changeComponentColor()));
    QObject::connect(deleteLayer, SIGNAL(clicked()), this, SLOT(removeLayer()));

    // Ajout de la vue à la pile
    stackedWidgetLayers.addWidget(groupLayer);
}

void ImageLayersWindow::addApplyActionLayer(int indexLayer, QAction *applyAction)
{
    // Ajout du bouton d'application à la vue du calque
    QGroupBox *groupLayer = (QGroupBox*) stackedWidgetLayers.widget(indexLayer);
    QGridLayout *layoutLayer = (QGridLayout*) groupLayer->layout();
    QPushButton *applyLayer = new QPushButton("Appliquer", groupLayer);
    applyLayer->addAction(applyAction);
    layoutLayer->addWidget(applyLayer, layoutLayer->count(), 0, 1, 3);

    // Connexion du bouton
    QObject::connect(applyLayer, SIGNAL(clicked()), applyAction, SLOT(trigger()));
}

void ImageLayersWindow::changeComponentColor()
{
    // Récupération de la couleur courante
    int indexLayers = comboBoxLayers.currentIndex();
    int indexComponent = comboBoxesComponentsLayers[indexLayers]->currentIndex();
    PixelType keyComponent = qvariant_cast<PixelType>(comboBoxesComponentsLayers[indexLayers]->itemData(indexComponent));
    QColor currentColor = (*imageLayers)[indexLayers].getComponents()[keyComponent];

    // Choix de la nouvelle couleur
    QColor newColor = QColorDialog::getColor(currentColor, nullptr, QString("Sélection couleur du composant"));

    if (newColor.isValid())
        // Si l'utilisateur a choisi une nouvelle couleur on la met à jour
        (*imageLayers)[indexLayers].setComponentColor(keyComponent, newColor);

    emit componentColorChanged();
}

void ImageLayersWindow::removeLayer()
{
    int indexLayer = comboBoxLayers.currentIndex();

    // Suppression des composants en liaison avec le calque
    comboBoxLayers.removeItem(indexLayer);
    comboBoxesComponentsLayers.erase(comboBoxesComponentsLayers.begin()+indexLayer);
    stackedWidgetLayers.removeWidget(stackedWidgetLayers.widget(indexLayer));
    imageLayers->erase(imageLayers->begin()+indexLayer);

    if (imageLayers->size() == 0)
    {
        // Si la liste de calques est vide on le signale
        QLabel *emptyLayer = new QLabel("Aucune image calque.", parentWidget());
        stackedWidgetLayers.addWidget(emptyLayer);
        stackedWidgetLayers.setCurrentWidget(emptyLayer);
    }

    emit imageLayerRemoved(indexLayer);
}
