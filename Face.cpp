#include "Face.h"

Face::Face() :
    nbIndicesPerFace(0)
{}

Face::Face(unsigned nbIndicesPerFace):
    nbIndicesPerFace(nbIndicesPerFace),
    indices(3, 0)
{}

Face::Face(const std::vector<unsigned>& indices, unsigned nbIndicesPerFace):
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