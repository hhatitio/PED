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
    _transparency = 0.90;
    _splitVal = 0.;
    
    if(im == NULL){
    _mesh = new Mesh(path);
    }else{
        _mesh = new Mesh(im);
    }
    _faces = _mesh->getFaces();
    _trspcMod = 0;
    _splitMod = 0;
    
    std::cout << "NbFaces : " << _faces.size() << std::endl;
}


MyOpenGLWidget::~MyOpenGLWidget() { }


void MyOpenGLWidget::initializeGL()
{
    glClearColor(0.2,0.2,0.2,1.);
    
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //glCullFace(GL_FRONT);
    //glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    //glEnable(GL_POLYGON_SMOOTH);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
    if (e->key() == Qt::Key_M)
        _trspcMod = (_trspcMod + 1) % 2;
    if (e->key() == Qt::Key_S)
        _splitMod = (_splitMod + 1) % 2;
    
    if (e->key() == Qt::Key_Plus)
        if (_transparency < 1.0)
            _transparency += 0.05;
    if (e->key() == Qt::Key_Minus)
        if (_transparency > 0.2)
            _transparency -= 0.05;
    
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

    if (e->key() == Qt::Key_A) _scale *= 1.05;
    if (e->key() == Qt::Key_Z) _scale *= 0.95;
    paintGL();
}


void MyOpenGLWidget::drawFace(Face f)
{
    float a;
    float alpha = (f.val / _mesh->getValmax());
    
    /*if ((_trspcMod % 2) == 0)
        a = 1. - (alpha * _transparency);
    else*/
        a = alpha * _transparency;
    
    float r = alpha;//(f.a.x + 1) / 2;
    float g = alpha;//(f.a.y + 1) / 2;
    float b = alpha;//(f.a.z + 1) / 2;
    
    //std::cout <<r << " " << g << " " << b << " " << alpha << std::endl;
    
    if ((f.a.z > _splitVal && f.c.z > _splitVal) || !_splitMod)
    {
        glBegin(GL_QUADS);
        glColor4f(r, g, b, a);
        glVertex3f(f.a.x, f.a.y, f.a.z);
        glVertex3f(f.b.x, f.b.y, f.b.z);
        glVertex3f(f.c.x, f.c.y, f.c.z);
        glVertex3f(f.d.x, f.d.y, f.d.z);
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
    drawAxes();
    drawBoundingBox(1, 1, 1);
    
    for (unsigned int i = 0; i < _faces.size(); i++) {
        drawFace(_faces[i]);
    }
    /*
     glBegin(GL_QUADS);
     glColor4f(0.9, 0.6, 0.2, 0.4);
     glVertex3f(-1., 1., -0.2);
     glVertex3f( 1., 1., -0.2);
     glVertex3f( 1.,-1., -0.2);
     glVertex3f(-1.,-1., -0.2);
     glEnd();
     glBegin(GL_QUADS);
     glColor4f(0.4, 0.6, 0.9, 0.4);
     glVertex3f(-1., 1., 0.3);
     glVertex3f( 1., 1., 0.3);
     glVertex3f( 1.,-1., 0.3);
     glVertex3f(-1.,-1., 0.3);
     glEnd();
     glBegin(GL_QUADS);
     glColor4f(0.2, 0.9, 0.2, 1.);
     glVertex3f(-1., 1., 0.5);
     glVertex3f( 1., 1., 0.5);
     glVertex3f( 1.,-1., 0.5);
     glVertex3f(-1.,-1., 0.5);
     glEnd();*/
    
}
