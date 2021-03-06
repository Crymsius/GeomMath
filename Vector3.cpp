#include "Vector3.h"

Vector3::Vector3() : x(0), y(0), z(0) {};

Vector3::Vector3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(const Vector3& coordinates) {
    this->x = coordinates.x;
    this->y = coordinates.y;
    this->z = coordinates.z;
}

Vector3& Vector3::operator=(const Vector3& rhs) {
    if (this == &rhs)
        return *this;
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

bool Vector3::operator==(const Vector3 &rhs) const {
    return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
}

Vector3& Vector3::operator+(const Vector3 &rhs) {
    this->x += rhs.x ;
    this->y += rhs.y ;
    this->z += rhs.z ;
    return *this;
}
Vector3& Vector3::operator-(const Vector3 &rhs) {
    this->x -= rhs.x ;
    this->y -= rhs.y ;
    this->z -= rhs.z ;
    return *this;
}

Vector3& Vector3::operator+(double scalar) {
    this->x += scalar ;
    this->y += scalar ;
    this->z += scalar ;
    return *this;
}
Vector3& Vector3::operator-(double scalar) {
    this->x -= scalar ;
    this->y -= scalar ;
    this->z -= scalar ;
    return *this;
}
Vector3& Vector3::operator*(double scalar) {
    this->x *= scalar ;
    this->y *= scalar ;
    this->z *= scalar ;
    return *this;
}
Vector3& Vector3::operator/(double scalar) {
    this->x /= scalar ;
    this->y /= scalar ;
    this->z /= scalar ;
    return *this;
}