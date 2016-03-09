//
//  mesh.h
//  pgm3dViewer
//
//  Created by Ruffino Andrea on 01/10/2015.
//
//  La classe Mesh met en place un objet 3D à partir d'un fichier PGM3D.
//  Seules les faces sont mis à disposition pour représenter le modèle.
//  Ces faces sont elles mêmes représentées par 4 sommets et une valeur
//  (de couleur en niveau de gris ou de niveau de transparence par exmple).
//
//  Ainsi on peut connaître le nombre de faces mais pas forcément
//  le nombre de voxels sachant qu'une face peut se partager une
//  voire plusieurs paire de voxel.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "meshStruct.hpp"
#include "deftypes.h"

class Mesh {
    
public:
    Mesh(std::string f = "");
    Mesh(Image *im);
    ~Mesh();
    
    int  loadFromFile(std::string file);
    size_t readFile(std::string f, std::vector<int> &t);
    void computeFace(int*** m);
    void computeFace(Image *im);
    void normalizeMesh();
    
    std::vector<Face> getFaces();
    int getValmax();
    
private:
    std::vector<Face> _faces;
    unsigned int _dim1, _dim2, _dim3;
    int _valmax;
};

#endif /* mesh_hpp */
