#ifndef VERTEX_H
#define VERTEX_H
#include "Point.h"
#include "Face.h"

struct Vertex {
    Point position; // Position du sommet
    int faceIncidente; // Accès à UNE face incidente
};

#endif /* Vertex_h */