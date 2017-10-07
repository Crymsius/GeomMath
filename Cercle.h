#ifndef CERCLE_H
#define CERCLE_H
#include "Point.h"
#include "Face.h"
#include "Mesh.h"

class Cercle {
public:
    //constructors
    Cercle(Point a, Point b, Point c);
    Cercle(const Face& f, const Mesh& mesh);
    Cercle(const Cercle& cerc);
    
    //methods
    bool isInCercle(const Point p);
    
private:
    Point a,b,c;
};

#endif