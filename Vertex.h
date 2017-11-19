#ifndef VERTEX_H
#define VERTEX_H
#include "Point.h"
#include "Face.h"

/// Vertex
// structure représentant un sommet du maillage
///
// Point position : Position du sommet
// int faceIncidente : indice d'une face incidente au sommet

struct Vertex {
    Point position; // Position du sommet
    int faceIncidente; // Accès à UNE face incidente
};

#endif /* Vertex_h */