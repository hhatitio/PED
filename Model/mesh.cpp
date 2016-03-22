//
//  mesh.cpp
//  pgm3dViewer
//
//  Created by Ruffino Andrea on 01/10/2015.
//
//  La classe Mesh construit un objet 3D à partir d'un fichier PGM3D.
//  Seules les faces sont mis à disposition pour représenter le modèle.
//  Ces faces sont elles mêmes représentées par 4 sommets et une valeur
//  (de couleur en niveau de gris ou de niveau de transparence par exmple).
//
//  Ainsi on peut connaître le nombre de faces mais pas forcément
//  le nombre de voxels sachant qu'une face peut se partager une
//  voire plusieurs paire de voxel.
//

#include "mesh.h"
#include <cmath>

using namespace std;


// ----------------------------------------------------------
Mesh::Mesh(Image *im)
{
    // On construit la liste des faces à partir des voxels de l'image.
    // et on ramène les positions de chaques faces entre -1 et 1.
    _valmax = 255;
    
    if(im != NULL)
    {
        computeFace(im);
        normalizeMesh();
    }
}

// ----------------------------------------------------------
Mesh::~Mesh()
{

}

// ----------------------------------------------------------
void Mesh::computeFace(Image *im)
{
    // p1 .. p8 représentent les 8 sommets du cube
    // englobant un voxel à une position (x,y,z).
    // Une illustration de la disposition du cube est
    // disponible dans le pdf joint au code source.
    vec3 p1, p2, p3, p4, p5, p6, p7, p8;

    _dim3 = ((unsigned int)im->n_slices);
    _dim2 = ((unsigned int)im->n_cols);
    _dim1 = ((unsigned int)im->n_rows);

    for (unsigned int z = 0; z < _dim3; z++)
    {
        for (unsigned int y = 0; y < _dim2; y++)
        {
            for (unsigned int x = 0; x < _dim1; x++)
            {
                // On crée une variable booléenne pour chacunes des six
                // faces du cube englobant le voxel à la position (x,y,z).
                // Si le booléen est vrai la construction de la face
                // correspondante s'effectuera, sinon elle sera 'oubliée'.
                int pos =  x+(y*_dim1)+(z*_dim1*_dim2);
                int valCur = im->at(pos);
                bool bool1 = true, bool2 = true, bool3 = true,
                     bool4 = true, bool5 = true, bool6 = true;

                if (valCur != 0)
                {
                    p1.x = x - 0.5; p1.y = y - 0.5; p1.z = z - 0.5;
                    p2.x = x - 0.5; p2.y = y + 0.5; p2.z = z - 0.5;
                    p3.x = x + 0.5; p3.y = y + 0.5; p3.z = z - 0.5;
                    p4.x = x + 0.5; p4.y = y - 0.5; p4.z = z - 0.5;
                    p5.x = x + 0.5; p5.y = y - 0.5; p5.z = z + 0.5;
                    p6.x = x + 0.5; p6.y = y + 0.5; p6.z = z + 0.5;
                    p7.x = x - 0.5; p7.y = y + 0.5; p7.z = z + 0.5;
                    p8.x = x - 0.5; p8.y = y - 0.5; p8.z = z + 0.5;

                    // On met à jour les booléens, la face sera dessinée
                    // uniquement si elle est strictement supérieur au
                    // voisin concerné (du dessus, du dessous etc...).
                    if (x > 0){
                        pos = (x-1)+(y*_dim1)+(z*_dim1*_dim2);
                        bool4 = (valCur > im->at(pos));
                    } else {
                        bool4 = true;
                    }
                    
                    if (x < _dim1 - 1){
                        pos = (x+1)+(y*_dim1)+(z*_dim1*_dim2);
                        bool2 = (valCur > im->at(pos));
                    } else {
                        bool2 = true;
                    }
                    
                    if (y > 0){
                        pos = x+((y-1)*_dim1)+(z*_dim1*_dim2);
                        bool6 = (valCur > im->at(pos));
                    } else {
                        bool6 = true;
                    }
                    
                    if (y < _dim2 - 1){
                        pos = x+((y+1)*_dim1)+(z*_dim1*_dim2);
                        bool5 = (valCur > im->at(pos));
                    } else {
                        bool5 = true;
                    }
                    
                    if (z > 0){
                        pos = x+(y*_dim1)+((z-1)*_dim1*_dim2);
                        bool1 = (valCur > im->at(pos));
                    }else {
                        bool1 = true;
                    }
                    
                    if (z < _dim3 - 1){
                        pos = x+(y*_dim1)+((z+1)*_dim1*_dim2);
                        bool3 = (valCur > im->at(pos));
                    }else {
                        bool3 = true;
                    }

                    if (bool1) {
                        Face f1 = { p1, p2, p3, p4, valCur};
                        _faces.push_back(f1);
                    }
                    if (bool2) {
                        Face f2 = { p4, p3, p6, p5, valCur};
                        _faces.push_back(f2);
                    }
                    if (bool3) {
                        Face f3 = { p5, p6, p7, p8, valCur};
                        _faces.push_back(f3);
                    }
                    if (bool4) {
                        Face f4 = { p8, p7, p2, p1, valCur};
                        _faces.push_back(f4);
                    }
                    if (bool5) {
                        Face f5 = { p2, p7, p6, p3, valCur};
                        _faces.push_back(f5);
                    }
                    if (bool6) {
                        Face f6 = { p1, p4, p5, p8, valCur};
                        _faces.push_back(f6);
                    }
                }
            }
        }
    }
}

// ----------------------------------------------------------
static void checkMinMax(vec3 &min, vec3 &max, vec3 v)
{
    min.x = (v.x < min.x) ? v.x : min.x;
    min.y = (v.y < min.y) ? v.y : min.y;
    min.z = (v.z < min.z) ? v.z : min.z;
    
    max.x = (v.x > max.x) ? v.x : max.x;
    max.y = (v.y > max.y) ? v.y : max.y;
    max.z = (v.z > max.z) ? v.z : max.z;
}


// ----------------------------------------------------------
void Mesh::normalizeMesh()
{
    if (_faces.size() < 1)
    {
        return;
    }
    
    // On initialise min et max aux positions du premier sommet
    // de la première face.
    _posMin = {_faces[0].a.x, _faces[0].a.y, _faces[0].a.z,};
    _posMax = {_faces[0].a.x, _faces[0].a.y, _faces[0].a.z,};
    
    for (unsigned int i = 0; i < _faces.size(); i++)
    {
        checkMinMax(_posMin, _posMax, _faces[i].a);
        checkMinMax(_posMin, _posMax, _faces[i].b);
        checkMinMax(_posMin, _posMax, _faces[i].c);
        checkMinMax(_posMin, _posMax, _faces[i].d);
    }
    
    // Pour une séquence de point X = {x0 .. xn} ou xi représente
    // la position d'un point sur une dimension, la formule xi - delta
    // assurera que la séquence X sera centrée sur 0. Il restera alors
    // plus qu'à diviser toutes les positions par maxScale pour que X
    // soit ramenée entre -1 et 1.
    float delta_x = (_posMax.x + _posMin.x) / 2;
    float delta_y = (_posMax.y + _posMin.y) / 2;
    float delta_z = (_posMax.z + _posMin.z) / 2;
    
    vec3 scale;
    scale.x = fabs(_posMax.x - _posMin.x) / 2;
    scale.y = fabs(_posMax.y - _posMin.y) / 2;
    scale.z = fabs(_posMax.z - _posMin.z) / 2;
    
    // Pour garder le rapport entre les distances sur les
    // 3 dimensions, on divise par le meme coeff qui est
    // le max des "max" 3 dimensions.
    float maxScale =   ( scale.x > scale.y ) ?
                (( scale.x > scale.z ) ? scale.x : scale.z):
                (( scale.y > scale.z ) ? scale.y : scale.z);
    
    // On applique la formule ((x - delta) / maxScale) sur les 4 sommets
    // de chaque face du tableau de face.
    for (unsigned int i = 0; i < _faces.size(); i++)
    {
        _faces[i].a.x = (_faces[i].a.x - delta_x) / maxScale;
        _faces[i].a.y = (_faces[i].a.y - delta_y) / maxScale;
        _faces[i].a.z = (_faces[i].a.z - delta_z) / maxScale;
        
        _faces[i].b.x = (_faces[i].b.x - delta_x) / maxScale;
        _faces[i].b.y = (_faces[i].b.y - delta_y) / maxScale;
        _faces[i].b.z = (_faces[i].b.z - delta_z) / maxScale;
        
        _faces[i].c.x = (_faces[i].c.x - delta_x) / maxScale;
        _faces[i].c.y = (_faces[i].c.y - delta_y) / maxScale;
        _faces[i].c.z = (_faces[i].c.z - delta_z) / maxScale;
        
        _faces[i].d.x = (_faces[i].d.x - delta_x) / maxScale;
        _faces[i].d.y = (_faces[i].d.y - delta_y) / maxScale;
        _faces[i].d.z = (_faces[i].d.z - delta_z) / maxScale;
    }
}


// ----------------------------------------------------------
std::vector<Face> Mesh::getFaces() {
    return _faces;
}


// ----------------------------------------------------------
int Mesh::getValmax() {
    return _valmax;
}


// ----------------------------------------------------------
unsigned int Mesh::getDim1()
{
    return _dim1;
}


// ----------------------------------------------------------
unsigned int Mesh::getDim2()
{
    
    return _dim2;
}


// ----------------------------------------------------------
unsigned int Mesh::getDim3()
{
    
    return _dim3;
}


// ----------------------------------------------------------
vec3 Mesh::getPosMin()
{
    return _posMin;
}


// ----------------------------------------------------------
vec3 Mesh::getPosMax()
{
    return _posMax;
}
