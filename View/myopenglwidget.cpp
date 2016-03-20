//
//  myopenglwidget.cpp
//  pgm3dViewer
//
//  Created by Ruffino Andrea on 01/10/2015.
//
//  Module OpenGL de la fenetre Qt, on permet à
//  l'utilisateur de visualiser un modèle 3D avec
//  camera implementant rotation, translation et zoom.
//

#include "myopenglwidget.h"

#include <iostream>
#include <string>
#include <cmath>

std::string path = "/Users/andrearuffino/Downloads/pgm3dViewer/Data/test.pgm3d";


MyOpenGLWidget::MyOpenGLWidget(QWidget *parent, Image *im)
: QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    
    x_R = 0.; x_T = 0.;
    y_R = 0.; y_T = 0.;
    z_R = 0.; z_T =-10.;
    
    _scale = 1.;
    _splitVal = 0.;
    
    if(im == NULL){
        _mesh = NULL;
    }else{
        setImageLayer(im);
    }
    _splitMod = 0;
    
    _alphaSkel = 0.8;
    _alphaVol  = 0.4;
    
    _boundingboxes = true;
    _axes = true;
    
    std::cout << "NbFaces : " << _faces.size() << std::endl;
}


MyOpenGLWidget::~MyOpenGLWidget() { }


void MyOpenGLWidget::setImageLayer(Image *im)
{
    _mesh = new Mesh(im);
    _faces = _mesh->getFaces();

}


void MyOpenGLWidget::initializeGL()
{
    glClearColor(0.2,0.2,0.2,1.);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void MyOpenGLWidget::resizeGL(int width, int height)
{
    double ratio = (double)width/height;
    
    glViewport(0,0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.*ratio, 2.*ratio, -2, 2, -20., 20.0);
    glMatrixMode(GL_MODELVIEW);
    
    paintGL();
}


void MyOpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(x_T, y_T, z_T);
    glRotatef(x_R / 18.0, 1.0, 0.0, 0.0);
    glRotatef(y_R / 18.0, 0.0, 1.0, 0.0);
    glRotatef(z_R / 18.0, 0.0, 0.0, 1.0);
    glScalef(_scale,_scale,_scale);
    
    draw();
    update();
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 18;
    while (angle > 360)
        angle -= 360 * 18;
}


void MyOpenGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != x_R) {
        x_R = angle;
        paintGL();
    }
}


void MyOpenGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != y_R) {
        y_R = angle;
        paintGL();
    }
}


void MyOpenGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != z_R) {
        z_R = angle;
        paintGL();
    }
}


void MyOpenGLWidget::mousePressEvent(QMouseEvent *e)
{
    _lastPos = e->pos();
}


void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* e)
{
    int dx = e->x() - _lastPos.x();
    int dy = e->y() - _lastPos.y();
    
    setXRotation(x_R + 8 * dy);
    setYRotation(y_R + 8 * dx);
    
    _lastPos = e->pos();
}


void MyOpenGLWidget::wheelEvent(QWheelEvent *e)
{
    if (e->delta() > 0) _scale *= 1.05;
    else _scale *= 0.95;
    paintGL();
}

void MyOpenGLWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_S)
        _splitMod = (_splitMod + 1) % 2;
    
    if (e->key() == Qt::Key_B)
        _boundingboxes = !_boundingboxes;
    
    if (e->key() == Qt::Key_A)
        _axes = !_axes;
    
    if (e->key() == Qt::Key_Plus)
        if (_alphaVol < 1.00)
            _alphaVol += 0.05;
    
    if (e->key() == Qt::Key_Minus)
        if (_alphaVol > 0.00)
            _alphaVol -= 0.05;
    
    if (e->key() == Qt::Key_PageUp)
        if (_splitVal < 0.95)
            _splitVal += 0.05;
    
    if (e->key() == Qt::Key_PageDown)
        if (_splitVal > -0.95)
            _splitVal -= 0.05;
    
    if (e->key() == Qt::Key_Down)
        y_T += 0.05;
    if (e->key() == Qt::Key_Up)
        y_T -= 0.05;
    if (e->key() == Qt::Key_Left)
        x_T += 0.05;
    if (e->key() == Qt::Key_Right)
        x_T -= 0.05;
    paintGL();
}


void MyOpenGLWidget::drawFace(Face f)
{
    float r, g, b, a;
    int max = _mesh->getValmax();
    
    if (f.val == VOXEL_VOLM) {
        a = _alphaVol;
        r = .7;
        g = .7;
        b = .7;
    }
    else if (f.val == VOXEL_SKEL) {
        a = 1.0;
        r = 1.0;
        g = 1.0;
        b = 1.0;
    }
    else if (f.val == VOXEL_NODE) {
        a = 1.0;
        r = 1.0;
        g = 0.3;
        b = 0.0;
    }
    else {
        a = 1.0;
        r = f.val / (float)max;
        g = f.val / (float)max;
        b = f.val / (float)max;
    }
    
    if ((f.a.z > _splitVal && f.c.z > _splitVal) || !_splitMod)
    {
        glBegin(GL_QUADS);
        glColor4f(r, g, b, a);
        glVertex3f(f.a.x, f.a.y, f.a.z);
        glVertex3f(f.d.x, f.d.y, f.d.z);
        glVertex3f(f.c.x, f.c.y, f.c.z);
        glVertex3f(f.b.x, f.b.y, f.b.z);
        glEnd();
    }
}

static void drawBoundingBox(float x, float y, float z)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(0.22, 0.62, 0.96);
    glVertex3f(-x,-y,-z);
    glVertex3f(-x, y,-z);
    glVertex3f( x, y,-z);
    glVertex3f( x,-y,-z);
    glVertex3f( x,-y, z);
    glVertex3f( x, y, z);
    glVertex3f(-x, y, z);
    glVertex3f(-x,-y, z);
    glEnd();
    
    glBegin(GL_LINES);
    glColor3f(0.22, 0.62, 0.96);
    glVertex3f(-x, y,-z); glVertex3f(-x, y, z);
    glVertex3f( x, y,-z); glVertex3f( x, y, z);
    glVertex3f(-x,-y,-z); glVertex3f( x,-y,-z);
    glVertex3f( x,-y, z); glVertex3f(-x,-y, z);
    glEnd();
}


static void drawAxes()
{
    glBegin(GL_LINES);
    glColor3f (1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    
    glColor3f (0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);
    
    glColor3f (0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();
}


void MyOpenGLWidget::draw() {
    
    if (_axes) drawAxes();
    if (_boundingboxes) drawBoundingBox(1, 1, 1);
    
    for (unsigned int i = 0; i < _faces.size(); i++) {
        drawFace(_faces[i]);
    }
}
