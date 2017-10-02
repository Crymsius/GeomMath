//
//  Mat3.h
//  GeomMath
//
//  Created by Crymsius on 02/10/2017.
//  Copyright © 2017 Crymsius. All rights reserved.
//

#ifndef Mat3_h
#define Mat3_h

#include <type_traits>

template<typename T>
class Mat3 {
public:
    static_assert(std::is_arithmetic<T>::value, "error tavu");
    
    //constructors
    Mat3();
    Mat3(T a00, T a10, T a20,
         T a01, T a11, T a21,
         T a02, T a12, T a22);
    Mat3(const Mat3& mat);
    
    //methods
    T det();
    
    //attribute
    T a00, a10, a20;
    T a01, a11, a21;
    T a02, a12, a22;
    
};

template<typename T>
Mat3<T>::Mat3() :
    a00(1), a10(0), a20(0),
    a01(0), a11(1), a21(0),
    a02(0), a12(0), a22(1)
{}

template<typename T>
Mat3<T>::Mat3(T a00, T a10, T a20, T a01, T a11, T a21, T a02, T a12, T a22) :
    a00(a00), a10(a10), a20(a20),
    a01(a01), a11(a11), a21(a21),
    a02(a02), a12(a12), a22(a22)
{}

template<typename T>
Mat3<T>::Mat3(const Mat3& mat) :
    a00(mat.a00), a10(mat.a10), a20(mat.a20),
    a01(mat.a01), a11(mat.a11), a21(mat.a21),
    a02(mat.a02), a12(mat.a12), a22(mat.a22)
{}

//operators
template<typename T>
Mat3<T> operator+(const Mat3<T>& mat1, const Mat3<T>& mat2) {
    return Mat3<T>(mat1.a00 + mat2.a00, mat1.a10 + mat2.a10, mat1.a20 + mat2.a20,
                   mat1.a01 + mat2.a01, mat1.a11 + mat2.a11, mat1.a21 + mat2.a21,
                   mat1.a02 + mat2.a02, mat1.a12 + mat2.a12, mat1.a22 + mat2.a22);
}

//methods
template<typename T>
T det(const Mat3<T>& mat) {
    return mat.a00*(mat.a11*mat.a22-mat.a12*mat.a21) - mat.a01*(mat.a10*mat.a22-mat.a12*mat.a20) + mat.a02*(mat.a10*mat.a21-mat.a11*mat.a20);
}

template<typename T>
T Mat3<T>::det() {
    return det(*this);
}


#endif /* Mat3_h */
