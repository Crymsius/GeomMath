#ifndef SHAPE_H
#define SHAPE_H
#include <vector>
#include "Vertex.h"

/// Shape
// structure représentant la forme - ou croute - résultante,  après crust
///
// vector<Vertex> vertices : vecteur des sommets, identique à celui du Mesh lors de sa création
// vector<pair<unsigned, unsigned>> aretes : vecteur des aretes, représentées par une pair d'indices de sommets

struct Shape {
    std::vector<Vertex> vertices; /// Sommets du maillage
    std::vector<std::pair<unsigned, unsigned>> aretes;
};


#endif /* Shape_hp*/
