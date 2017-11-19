#ifndef CERCLE_H
#define CERCLE_H
#include "Point.h"
#include "Face.h"
#include "Mesh.h"

/// Cercle
// classe représentant un cercle
//
// Le cercle par 3 points non confondus et non allignés.
// C'est le cercle circonscrit au triangle formé par les 3 points.
//
/// constructeurs
// On peut contruire un cercle avec
// 3 points
// 1 face et le mesh associé
//
/// méthodes
// - bool isInCercle(const Point p) :
//      renvoie true sur le point p est dans le cercle, false sinon
// - Point centre() :
//      renvoie le centre du cercle.
class Cercle {
public:
    //constructors
    Cercle(Point a, Point b, Point c);
    Cercle(const Face& f, const Mesh& mesh);
    Cercle(const Cercle& cerc);
    
    //methods
    bool isInCercle(const Point p);
    Point centre();
    
private:
    Point a,b,c;
};

#endif