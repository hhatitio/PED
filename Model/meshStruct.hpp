//
//  meshStruct.hpp
//  pgm3dViewer
//
//  Created by Ruffino Andrea on 04/10/2015.
//
//  Implementation d'une structure de voxels de faces et de
//  vecteurs à 3 dimensions. Les opérations les plus simple
//  ont été implémentées.
//

#ifndef meshStruct_hpp
#define meshStruct_hpp

#include <stdio.h>

typedef struct { float x, y, z; } vec3;
typedef struct { vec3 pos, nrm; } Voxel;
typedef struct { vec3 a, b, c, d; float val; } Face;

#endif /* meshStruct_hpp */
