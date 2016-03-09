//
//  myopenglwidget.h
//  pgm3dViewer
//
//  Created by Ruffino Andrea on 01/10/2015.
//
//  Module OpenGL de la fenetre Qt, on permet à
//  l'utilisateur de visualiser un modèle 3D avec
//  camera implementant rotation, translation et zoom.
//

#ifndef myopenglwidget_h
#define myopenglwidget_h

#include <QtOpenGL>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <vector>

#include "Model/mesh.h"

class MyOpenGLWidget : public QGLWidget
{
public:
    MyOpenGLWidget(QWidget *parent, Image* im =NULL);
   // MyOpenGLWidget(QWidget *parent, Image* im);
    ~MyOpenGLWidget();
    
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    
signals:
    
    public slots:
    
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    
    void draw();
    void drawFace(Face f);
    
private:
    float x_R, y_R, z_R;
    float x_T, y_T, z_T;
    float _scale;
    float _transparency;
    float _splitVal;
    
    unsigned int _trspcMod;
    unsigned int _splitMod;
    
    QPoint _lastPos;
    
    Mesh *_mesh;
    std::vector<Face> _faces;
};


#endif /* myopenglwidget_h */
