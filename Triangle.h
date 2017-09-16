#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Point.h"

class Triangle {
public:
    Point s0;
    Point s1;
    Point s2;

    Triangle();
    Triangle(Point s0, Point s1, Point s2);
};
#endif