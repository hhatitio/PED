//
//  meshStruct.cpp
//  pgm3dViewer
//
//  Created by Ruffino Andrea on 04/10/2015.
//
//  Implementation d'une structure de voxels de faces et de
//  vecteurs à 3 dimensions. Les opérations les plus simple
//  ont été implémentées.
//

#include "meshStruct.hpp"


bool operator==(vec3 v1, vec3 v2) {
    return ((v1.x == v2.x) &&
            (v1.y == v2.y) &&
            (v1.z == v2.z));
}

bool operator==(Face f1, Face f2) {
    return ((f1.a == f2.a) &&
            (f1.b == f2.b) &&
            (f1.c == f2.c) &&
            (f1.d == f2.d));
}

vec3 operator+(vec3 v1, vec3 v2) {
    vec3 res;
    res.x = v1.x + v2.x;
    res.y = v1.y + v2.y;
    res.z = v1.z + v2.z;
    return res;
}

vec3 operator-(vec3 v1, vec3 v2) {
    vec3 res;
    res.x = v1.x - v2.x;
    res.y = v1.y - v2.y;
    res.z = v1.z - v2.z;
    return res;
}