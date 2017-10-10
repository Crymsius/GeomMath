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

Point Cercle::centre() {
    float D = 2 * (a.x*(b.y-c.y) + b.x*(c.y-a.y) + c.x*(a.y-b.y));
    float x = (((a.x * a.x)+(a.y * a.y))*(b.y - c.y) + ((b.x * b.x)+(b.y * b.y))*(c.y - a.y) + ((c.x * c.x)+(c.y * c.y))*(a.y - b.y))/D;
    float y = (((a.x * a.x)+(a.y * a.y))*(c.x - b.x) + ((b.x * b.x)+(b.y * b.y))*(a.x - c.x) + ((c.x * c.x)+(c.y * c.y))*(b.x - a.x))/D;
    return Point(x,y,0.f);
}