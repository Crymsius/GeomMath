#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Point.h"

/// Triangle
// classe représentant une face un triangle
///
// un triangle est défini par 3 points.
///
// constructeurs

class Triangle {
public:
    Point s0;
    Point s1;
    Point s2;

    Triangle();
    Triangle(Point s0, Point s1, Point s2);
};
#endif