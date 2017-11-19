#ifndef FACE_H
#define FACE_H
#include <vector>
#include <map>
#include <utility>
#include "Point.h"

/// Face
// structure représentant une face du maillage
///
// vector<unsigned> indices : vecteur des indices des sommets de la face, dans l'ordre trigonométrique
// int nbIdicesPerFace : nombre de sommet de la face
// vector<int> facesAdjacentes: vecteurs des indices des faces adjacentes, dans l'ordre des indices des sommets
//      la face facesAdjacentes[0] est la face opposée au sommet indices[0]
///
// constructeurs

struct Face {
    
    std::vector<unsigned> indices;
    int nbIndicesPerFace;
    std::vector<int> facesAdjacentes;

    Face();
    Face(int nbIndexPerFace);
    Face(const std::vector<unsigned>& indices, int nbIndicesPerFace);
    Face(const Face& face);
    
};

#endif /* FACE_H */
