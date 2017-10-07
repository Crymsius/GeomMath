#include "Cercle.h"
#include "Mat3.h"
#include <tgmath.h>

Cercle::Cercle (Point a, Point b, Point c) :
    a(a),
    b(b),
    c(c)
{}

Cercle::Cercle (const Face& f, const Mesh& mesh) :
    a(mesh._Vertices()[f.indices[0]].position),
    b(mesh._Vertices()[f.indices[1]].position),
    c(mesh._Vertices()[f.indices[2]].position)
{}

Cercle::Cercle (const Cercle& cerc) :
    a(cerc.a),
    b(cerc.b),
    c(cerc.c)
{}

bool Cercle::isInCercle(const Point p) {
    Mat3<float> mat((b.x - a.x), (b.y - a.y), (std::pow((b.x - a.x),2) + std::pow((b.y - a.y),2)),
                (c.x - a.x), (c.y - a.y), (std::pow((c.x - a.x),2) + std::pow((c.y - a.y),2)),
                (p.x - a.x), (p.y - a.y), (std::pow((p.x - a.x),2) + std::pow((p.y - a.y),2)));
    float det = - mat.det();
    
    return det > 0;
}