#include "Triangle.h"
Triangle::Triangle() : s0(), s1(), s2() {};

Triangle::Triangle(Point s0, Point s1, Point s2) {
    this->s0 = s0;
    this->s1 = s1;
    this->s2 = s2;
}