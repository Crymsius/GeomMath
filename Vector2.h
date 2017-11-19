#ifndef VECTOR2_H
#define VECTOR2_H

/// Vector2
// classe représentant un vecteur 2D
///
// float x, y : coordonnées du vecteur
///
// opérateurs :
// • +, - avec un autre Vector2
// • +, -, * avec un scalaire


class Vector2 {
    public:

    float x;
    float y;

    Vector2();
    Vector2(float x, float y);

    bool operator==(const Vector2 &rhs) const;

    Vector2& operator+(const Vector2 &rhs);
    Vector2& operator-(const Vector2 &rhs);

    Vector2& operator+(float scalar);
    Vector2& operator-(float scalar);
    Vector2& operator*(float scalar);
};

#endif