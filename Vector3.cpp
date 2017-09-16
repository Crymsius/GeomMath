#include "Vector3.h"

Vector3::Vector3() : x(), y(), z() {};

Vector3::Vector3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
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

Vector3& Vector3::operator+(float scalar) {
    this->x += scalar ;
    this->y += scalar ;
    this->z += scalar ;
    return *this;
}
Vector3& Vector3::operator-(float scalar) {
    this->x -= scalar ;
    this->y -= scalar ;
    this->z -= scalar ;
    return *this;
}
Vector3& Vector3::operator*(float scalar) {
    this->x *= scalar ;
    this->y *= scalar ;
    this->z *= scalar ;
    return *this;
}