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
#include <stdio.h>
#include <stdlib.h>

using namespace std;


// ----------------------------------------------------------
Mesh::Mesh(std::string file)
{
    loadFromFile(file);
}


// ----------------------------------------------------------
Mesh::~Mesh()
{

}


// ----------------------------------------------------------
int Mesh::loadFromFile(std::string file)
{
    vector<int> tab;
    int *** mesh;
    
    // On construit un tableau unidimensionnel des valeurs
    // contenues dans le fichier.
    size_t s = readFile(file, tab);
    mesh = (int***) malloc(_dim1 * sizeof(int**));
    
    for (unsigned int x = 0; x < _dim1; x++)
    {
        mesh[x] = (int **) malloc(_dim2 * sizeof(int*));
        // ... x axis here
        for (unsigned int y = 0; y < _dim2; y++)
        {
            mesh[x][y] = (int *) malloc(_dim3 *sizeof(int));
            // ... y axis here
            for (unsigned int z = 0; z < _dim3; z++)
            {
                int pos = z * _dim3 * _dim2 + y * _dim2 + x;
                mesh[x][y][z] = tab[pos];
            }
        }
    }
    
    // On construit la liste des faces à partir du tableau mesh.
    // et on ramène les positions de chaques faces entre -1 et 1.
    computeFace(mesh);
    normalizeMesh();
    
    //On libere le tableau mesh qui n'est plus utile.
    for (unsigned int x = 0; x < _dim1; x++)
    {
        for (unsigned int y = 0; y < _dim2; y++)
        {
            free(mesh[x][y]);
        }
        free(mesh[x]);
    }
    free(mesh);
    
    return s;
}


// ----------------------------------------------------------
size_t Mesh::readFile(string filename, vector<int> &tab)
{
    size_t size = 0;
    string format;
    int val;
    
    ifstream file(filename.data(), ios::in);
    
    if (file) {
        file >> format >> _dim1 >> _dim2 >> _dim3 >> _valmax;
        size = _dim1 * _dim2 * _dim3;
        
        if (format.compare("PGM3D") != 0)
            cerr << "Warning: Wrong format" << endl;
        
        for (unsigned int i = 0; i < size; i++)
        {
            file >> val;
            if (val > _valmax) val = _valmax;
            tab.push_back(val);
        }
        file.close();
    } else {
        cerr << "Error: File not read." << endl;
    }
    return size;
}


// ----------------------------------------------------------
void Mesh::computeFace(int *** t)
{
    // p1 .. p8 représentent les 8 sommets du cube
    // englobant un voxel à une position (x,y,z).
    // Une illustration de la disposition du cube est
    // disponible dans le pdf joint au code source.
    vec3 p1, p2, p3, p4, p5, p6, p7, p8;
    
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
                int valCur = t[x][y][z];
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
                    if (x > 0)
                        bool4 = (valCur > t[x - 1][y][z]);
                    if (x < _dim1 - 1)
                        bool2 = (valCur > t[x + 1][y][z]);
                    if (y > 0)
                        bool6 = (valCur > t[x][y - 1][z]);
                    if (y < _dim2 - 1)
                        bool5 = (valCur > t[x][y + 1][z]);
                    if (z > 0)
                        bool1 = (valCur > t[x][y][z - 1]);
                    if (z < _dim3 - 1)
                        bool3 = (valCur > t[x][y][z + 1]);
                    
                    if (bool1) {
                        Face f1 = { p1, p2, p3, p4, valCur};
                        _faces.push_back(f1);
                    }
                    if (bool2) {
                        Face f2 = { p3, p4, p5, p6, valCur};
                        _faces.push_back(f2);
                    }
                    if (bool3) {
                        Face f3 = { p5, p6, p7, p8, valCur};
                        _faces.push_back(f3);
                    }
                    if (bool4) {
                        Face f4 = { p7, p8, p1, p2, valCur};
                        _faces.push_back(f4);
                    }
                    if (bool5) {
                        Face f5 = { p2, p3, p6, p7, valCur};
                        _faces.push_back(f5);
                    }
                    if (bool6) {
                        Face f6 = { p1, p4, p5, p8, valCur};
                        _faces.push_back(f6);
                    }
                    //std::cout << bool1 << bool2 << bool3 << bool4 << bool5 << bool6 << std::endl;
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
    vec3 posMin = {_faces[0].a.x, _faces[0].a.y, _faces[0].a.z,};
    vec3 posMax = {_faces[0].a.x, _faces[0].a.y, _faces[0].a.z,};
    
    for (unsigned int i = 0; i < _faces.size(); i++)
    {
        checkMinMax(posMin, posMax, _faces[i].a);
        checkMinMax(posMin, posMax, _faces[i].b);
        checkMinMax(posMin, posMax, _faces[i].c);
        checkMinMax(posMin, posMax, _faces[i].d);
    }
    
    // Pour une séquence de point X = {x0 .. xn} ou xi représente
    // la position d'un point sur une dimension, la formule xi - delta
    // assurera que la séquence X sera centrée sur 0. Il restera alors
    // plus qu'à diviser toutes les positions par maxScale pour que X
    // soit ramenée entre -1 et 1.
    float delta_x = (posMax.x + posMin.x) / 2;
    float delta_y = (posMax.y + posMin.y) / 2;
    float delta_z = (posMax.z + posMin.z) / 2;
    
    vec3 scale;
    scale.x = fabs(posMax.x - posMin.x) / 2;
    scale.y = fabs(posMax.y - posMin.y) / 2;
    scale.z = fabs(posMax.z - posMin.z) / 2;
    
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
