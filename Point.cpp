#include "Point.h"

Point::Point() {}

Point::Point(Vector3 coordinates) {
    this->x = coordinates.x;
    this->y = coordinates.y;
    this->z = coordinates.z;
}

Point::Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}