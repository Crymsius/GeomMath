#include "Face.h"

Face::Face() :
    nbIndicesPerFace(0)
{}

Face::Face(int nbIndicesPerFace):
    indices(nbIndicesPerFace),
    nbIndicesPerFace(nbIndicesPerFace),
    facesAdjacentes(nbIndicesPerFace)
{}

Face::Face(const std::vector<unsigned>& indices, int nbIndicesPerFace):
    indices(indices),
    nbIndicesPerFace(nbIndicesPerFace)
{
    facesAdjacentes.reserve(nbIndicesPerFace);
}

Face::Face(const Face &face) {
    this->indices = face.indices;
    this->nbIndicesPerFace = face.nbIndicesPerFace;
    this->facesAdjacentes = face.facesAdjacentes;
}