#ifndef FACE_H
#define FACE_H
#include <vector>
#include <map>
#include <utility>
#include "Point.h"

struct Face {
    
    std::vector<unsigned> indices; // sommets dans l'ordre trigo
    unsigned nbIndicesPerFace; // nb de sommet de la face
    
    std::vector<unsigned> facesAdjacentes; // Accès aux faces adjacentes
    
    Face();
    Face(unsigned nbIndexPerFace);
    Face(const std::vector<unsigned>& indices, unsigned nbIndicesPerFace);
    Face(const Face& face);
    
};

#endif /* FACE_H */
