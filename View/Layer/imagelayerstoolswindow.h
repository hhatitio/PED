#ifndef IMAGELAYERSTOOLSWINDOW_H
#define IMAGELAYERSTOOLSWINDOW_H

#include <QDialog>
#include <QComboBox>
#include <QStackedWidget>
#include <QRadioButton>
#include <QPoint>

#include "../../Model/Layer/imagelayer.h"

#define IMAGE_LAYER_TOOLS_WINDOW_MINIMUM_WIDTH 500

class ImageLayersToolsWindow : public QDialog
{

    Q_OBJECT

public:

    enum ToolType { Empty, Pencil, Eraser };

    ImageLayersToolsWindow(QWidget *parent = 0);

    void setImageLayers(std::vector<ImageLayer> *imageLayers);

    ImageLayer getCurrentImageLayer();
    ToolType getType();

    void addViewLayer(const ImageLayer &layer);
    void apply(const QPoint &position, int slice);

public slots:

    void setType();
    void removeViewLayer(int indexLayer);

private:

    std::vector<ImageLayer> *imageLayers;

    QComboBox comboBoxLayers;
    std::vector<QComboBox*> comboBoxesComponentsLayers;
    std::vector<QComboBox*> comboBoxesSizesToolLayers;
    QStackedWidget stackedWidgetLayers;

    QRadioButton buttonPencil;
    QRadioButton buttonEraser;

    ToolType type = ToolType::Empty;

};

#endif // IMAGELAYERSTOOLSWINDOW_H
