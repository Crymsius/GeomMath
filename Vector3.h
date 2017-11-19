#ifndef VECTOR3_H
#define VECTOR3_H

// Vector3
// classe représentant un vecteur 3D
///
// double x, y, z : coordonnées du vecteur
///
// opérateurs :
// • +, -, =, == avec un autre Vector2
// • +, -, *, / avec un scalaire

class Vector3 {
    public:

    double x;
    double y;
    double z;

    Vector3();
    Vector3(double x, double y, double z);
    Vector3(const Vector3& coordinates);

    bool operator==(const Vector3 &rhs) const;

    Vector3& operator+(const Vector3 &rhs);
    Vector3& operator-(const Vector3 &rhs);
    Vector3& operator=(const Vector3 &rhs);

    Vector3& operator+(double scalar);
    Vector3& operator-(double scalar);
    Vector3& operator*(double scalar);
    Vector3& operator/(double scalar);
};

#endif