#ifndef VIEWER3D_H
#define VIEWER3D_H

#include "../Model/Layer/imagelayer.h"

#include <QGLViewer/qglviewer.h>

class Viewer3D : public QGLViewer
{

public :

    void setImageLayer(const ImageLayer &imageLayer);
    void initGLScene();

protected :

    virtual void init();
    virtual void draw();

private :

    ImageLayer imageLayer;

};

#endif // VIEWER3D_H
