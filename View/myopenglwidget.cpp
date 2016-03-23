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


vec3 _blue      = {0.22, 0.62, 0.96};
vec3 _deepblue  = {0.07, 0.20, 0.60};
vec3 _black     = {0.16, 0.16, 0.16};
vec3 _green     = {0.16, 0.94, 0.46};
vec3 _white     = {0.98, 0.98, 0.98};


MyOpenGLWidget::MyOpenGLWidget(QWidget *parent, Image *im, EdgeMap edges, NodeMap nodes)
: QGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    
    x_R = 0.; x_T = 0.;
    y_R = 0.; y_T = 0.;
    z_R = 0.; z_T =-10.;
    
    _scale = 1.;
    _splitVal = 0.;
    _isGraph = false;
    
    if (im == NULL)
        _mesh = NULL;
    else
        setImageLayer(im);
    
    if (edges.size() != 0)
        setGraphLayer(edges, nodes);
    
    _splitMod = 0;
    
    _alphaSkel = 0.8;
    _alphaVol  = 0.4;
    
    _boundingboxes = true;
    _axes = false;
    
    std::cout << "NbFaces : " << _faces.size() << std::endl;
}

MyOpenGLWidget::~MyOpenGLWidget() { }


void MyOpenGLWidget::setImageLayer(Image *im)
{
    _mesh = new Mesh(im);
    _faces = _mesh->getFaces();
}

void MyOpenGLWidget::setGraphLayer(EdgeMap edges, NodeMap nodes)
{
    if (_mesh == NULL) return;
    
    _edges = edges;
    _isGraph = true;
    _edgeCoord.clear();
    
    for (auto it = _edges.begin(); it!= _edges.end(); ++it)
    {
        ExtendedEdge * e = it->second;
        ExtendedNode * n1 = nodes.at(e->getFirstNodePos());
        ExtendedNode * n2 = nodes.at(e->getSecondNodePos());
        
        _edgeCoord.push_back(n1->getX());
        _edgeCoord.push_back(n1->getY());
        _edgeCoord.push_back(n1->getZ());
        
        _edgeCoord.push_back(n2->getX());
        _edgeCoord.push_back(n2->getY());
        _edgeCoord.push_back(n2->getZ());
    }
    
    vec3 posMin = _mesh->getPosMin();
    vec3 posMax = _mesh->getPosMax();
    
    float delta_x = (posMax.x + posMin.x) / 2;
    float delta_y = (posMax.y + posMin.y) / 2;
    float delta_z = (posMax.z + posMin.z) / 2;
    
    vec3 scale;
    scale.x = fabs(posMax.x - posMin.x) / 2;
    scale.y = fabs(posMax.y - posMin.y) / 2;
    scale.z = fabs(posMax.z - posMin.z) / 2;
    
    float maxScale =   ( scale.x > scale.y ) ?
    (( scale.x > scale.z ) ? scale.x : scale.z):
    (( scale.y > scale.z ) ? scale.y : scale.z);
    
    for (unsigned int i = 0; i < _edgeCoord.size(); i += 3)
    {
        _edgeCoord[i+0] = (_edgeCoord[i+0] - delta_x) / maxScale;
        _edgeCoord[i+1] = (_edgeCoord[i+1] - delta_y) / maxScale;
        _edgeCoord[i+2] = (_edgeCoord[i+2] - delta_z) / maxScale;
    }
}


void MyOpenGLWidget::initializeGL()
{
    glClearColor(_black.x,_black.y,_black.z,1.);
    
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
    
    if (e->key() == Qt::Key_E)
        _arcs = !_arcs;
    
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
    
    
    if (e->key() == Qt::Key_P)
        z_R += 8;
    if (e->key() == Qt::Key_M)
        z_R -= 8;
    
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

//std::vector<int> MyOpenGLWidget::getCoordOutOfIndex(int n_cols, int n_rows, int idx)
//{
    //// Mathematical formulas to calculate 3D Coordinte of voxel
    //int sizeLayer = n_cols * n_rows;
    //int z = (int)idx / sizeLayer;
    
    //int idx2D = idx - z*sizeLayer;
    //int y = (int)idx2D / n_cols;
    //int x = idx2D % n_cols;
    
    //// Store the calculated coordinates
    //std::vector<int> coord;
    //coord.push_back(x);
    //coord.push_back(y);
    //coord.push_back(z);
    //return coord;
//}


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
        a = (_arcs) ? 0.0 : 0.9;
        r = _white.x;
        g = _white.y;
        b = _white.z;
    }
    else if (f.val == VOXEL_NODE) {
        a = 1.0;
        r = _blue.x;
        g = _blue.y;
        b = _blue.z;
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

void MyOpenGLWidget::drawArcs()
{
    for (unsigned int i = 0; i < _edgeCoord.size(); i += 6)
    {
        glBegin(GL_LINES);
        glColor3f (_green.x, _green.y, _green.z);
        glVertex3f(_edgeCoord[i+0], _edgeCoord[i+1], _edgeCoord[i+2]);
        glVertex3f(_edgeCoord[i+3], _edgeCoord[i+4], _edgeCoord[i+5]);
        glEnd();
    }
}

static void drawBoundingBox(float x, float y, float z)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(0.24, 0.24, 0.24);
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
    glColor3f(0.24, 0.24, 0.24);
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
    if (_arcs && _isGraph) drawArcs();
    
    for (unsigned int i = 0; i < _faces.size(); i++) {
        drawFace(_faces[i]);
    }
}
