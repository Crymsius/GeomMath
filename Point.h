#ifndef POINT_H
#define POINT_H
#include "Vector3.h"

class Point : public Vector3 {
    public :
    
    Point();
    Point(Vector3 coordinates);
    Point(float x, float y, float z);
};
#endif //POINT_H