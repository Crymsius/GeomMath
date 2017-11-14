#ifndef VECTOR3_H
#define VECTOR3_H

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