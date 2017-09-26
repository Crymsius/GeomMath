#ifndef FACE_H
#define FACE_H
#include <vector>
#include <map>
#include <utility>
#include "Point.h"

struct Face {
    
    std::vector<unsigned> indices; // sommets dans l'ordre trigo
    int nbIndicesPerFace; // nb de sommet de la face
    std::vector<int> facesAdjacentes; // Accès aux faces adjacentes
    //ATTENTION IL FAUT CHANGER LES FACESADJACENTES POUR UN TABLEAU !!!!

    Face();
    Face(int nbIndexPerFace);
    Face(const std::vector<unsigned>& indices, int nbIndicesPerFace);
    Face(const Face& face);
    
};

#endif /* FACE_H */
