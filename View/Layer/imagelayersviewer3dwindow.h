#ifndef IMAGELAYERSVIEWER3DWINDOW_H
#define IMAGELAYERSVIEWER3DWINDOW_H

#include "../../Model/Layer/imagelayer.h"

#include "../viewer3d.h"
#include "../myopenglwidget.h"

#include <DGtal/io/viewers/Viewer3D.h>

#include <QDialog>
#include <QComboBox>
#include <QPushButton>


#define IMAGE_LAYER_VIEWER3D_WINDOW_MINIMUM_WIDTH 500

class ImageLayersViewer3DWindow : public QDialog
{

    Q_OBJECT

    typedef DGtal::Viewer3D<> Viewer;

public:

    ImageLayersViewer3DWindow(QWidget *parent = 0);

    void setImageLayers(std::vector<ImageLayer> *imageLayers);

    void addViewLayer(const ImageLayer &layer);

public slots:

    void openViewer3D();
    void removeViewLayer(int indexLayer);

private:

    std::vector<ImageLayer> *imageLayers;

    QComboBox comboBoxLayers;
    QPushButton buttonValid;

    Viewer dgtalViewer;
    //Viewer3D myViewer;
    MyOpenGLWidget myViewer;

    static const bool withDGtal = false;

};

#endif // IMAGELAYERSVIEWER3DWINDOW_H
