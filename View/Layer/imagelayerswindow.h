#ifndef IMAGELAYERSWINDOW_H
#define IMAGELAYERSWINDOW_H

#include "../../Model/Layer/imagelayer.h"

#include <QDialog>
#include <QGroupBox>
#include <QComboBox>
#include <QAction>
#include <QStackedWidget>

#define IMAGE_LAYER_WINDOW_MINIMUM_WIDTH 500
#define LABEL_INDEX_COMPONENT_PROPERTY "indexComponent"

class ImageLayersWindow : public QDialog
{

    Q_OBJECT

public:

    ImageLayersWindow(QWidget *parent = 0);

    void setImageLayers(std::vector<ImageLayer> *imageLayers);
    void addViewLayer(const ImageLayer &layer);
    void addApplyActionLayer(int indexLayer, QAction *applyAction);

public slots:

    void changeComponentColor();
    void removeLayer();

signals:

    void componentColorChanged();
    void imageLayerRemoved(int indexLayer);

private:

    std::vector<ImageLayer> *imageLayers;

    QComboBox comboBoxLayers;
    std::vector<QComboBox*> comboBoxesComponentsLayers;
    QStackedWidget stackedWidgetLayers;

};

#endif // IMAGELAYERSWINDOW_H
