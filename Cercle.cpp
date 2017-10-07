#include "Cercle.h"
#include "Mat3.h"
#include <tgmath.h>

bool Cercle::isInCercle(const Point p) {
    Mat3<float> mat((b.x - a.x), (b.y - a.y), (std::pow((b.x - a.x),2) + std::pow((b.y - a.y),2)),
                (c.x - a.x), (c.y - a.y), (std::pow((c.x - a.x),2) + std::pow((c.y - a.y),2)),
                (p.x - a.x), (p.y - a.y), (std::pow((p.x - a.x),2) + std::pow((p.y - a.y),2)));
    float det = - mat.det();
    
    return det > 0;
}