#include "Vector2.h"

Vector2::Vector2() : x(), y() {};

Vector2::Vector2(float x, float y) {
    this->x = x;
    this->y = y;
}

bool Vector2::operator==(const Vector2 &rhs) const {
    return (this->x == rhs.x && this->y == rhs.y);
}

Vector2& Vector2::operator+(const Vector2 &rhs) {
    this->x += rhs.x ;
    this->y += rhs.y ;
    return *this;
}
Vector2& Vector2::operator-(const Vector2 &rhs) {
    this->x -= rhs.x ;
    this->y -= rhs.y ;
    return *this;
}

Vector2& Vector2::operator+(float scalar) {
    this->x += scalar ;
    this->y += scalar ;
    return *this;
}
Vector2& Vector2::operator-(float scalar) {
    this->x -= scalar ;
    this->y -= scalar ;
    return *this;
}
Vector2& Vector2::operator*(float scalar) {
    this->x *= scalar ;
    this->y *= scalar ;
    return *this;
}