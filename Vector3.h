#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3 {
    public:

    float x;
    float y;
    float z;

    Vector3();
    Vector3(float x, float y, float z);

    bool operator==(const Vector3 &rhs) const;

    Vector3& operator+(const Vector3 &rhs);
    Vector3& operator-(const Vector3 &rhs);

    Vector3& operator+(float scalar);
    Vector3& operator-(float scalar);
    Vector3& operator*(float scalar);
};

#endif