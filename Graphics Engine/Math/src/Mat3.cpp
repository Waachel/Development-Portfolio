//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
    //Default Constructor
    Mat3::Mat3()
        :_m0(0.0f), _m1(0.0f), _m2(0.0f),
        _m4(0.0f), _m5(0.0f),  _m6(0.0f), 
        _m8(0.0f),  _m9(0.0f), _m10(0.0f)
    {

    }

    //Specialized Constructor
    Mat3::Mat3::Mat3(const Vec3& tV0, const Vec3& tV1, const Vec3& tV2)
    {
        _m0 = tV0._vx;
        _m1 = tV0._vy;
        _m2 = tV0._vz;

        _m4 = tV1._vx;
        _m5 = tV1._vy;
        _m6 = tV1._vz;

        _m8 = tV2._vx;
        _m9 = tV2._vy;
        _m10 = tV2._vz;

    }

    Mat3::Mat3(const Mat4& m)
    {
        this->_m0 = m._m0;
        this->_m1 = m._m1;
        this->_m2 = m._m2;
        this->_m3 = 0;
        this->_m4 = m._m4;
        this->_m5 = m._m5;
        this->_m6 = m._m6;
        this->_m7 = 0;
        this->_m8 = m._m8;
        this->_m9 = m._m9;
        this->_m10 = m._m10;
        this->_m11 = 0;
        this->_m12 = 0;
        this->_m13 = 0;
        this->_m14 = 0;
        this->_m15 = 1;
    }

    Mat3::Mat3(const Special type)
    {
        if (type == Special::Identity)
        {
            _m0 = 1.0f;
            _m1 = 0.0f;
            _m2 = 0.0f;

            _m4 = 0.0f;
            _m5 = 1.0f;
            _m6 = 0.0f;

            _m8 = 0.0f;
            _m9 = 0.0f;
            _m10 = 1.0f;
        }

        else
        {
            _m0 = 0.0f;
            _m1 = 0.0f;
            _m2 = 0.0f;

            _m4 = 0.0f;
            _m5 = 0.0f;
            _m6 = 0.0f;

            _m8 = 0.0f;
            _m9 = 0.0f;
            _m10 = 0.0f;
        }
    }

    Vec3 Mat3::get(const Row type) const
    {
        if (type == Row::i0)
        {
            return Vec3(_m0, _m1, _m2);
        }

        else if (type == Row::i1)
        {
            return Vec3(_m4, _m5, _m6);
        }

        else
        {
            return Vec3(_m8, _m9, _m10);
        }
    }

    void Mat3::set(const Mat3& mIn)
    {
        this->_m0 = mIn._m0;
        this->_m1 = mIn._m1;
        this->_m2 = mIn._m2;

        this->_m4 = mIn._m4;
        this->_m5 = mIn._m5;
        this->_m6 = mIn._m6;

        this->_m8 = mIn._m8;
        this->_m9 = mIn._m9;
        this->_m10 = mIn._m10;

    }

    void Mat3::set(const Special type)
    {
        if (type == Special::Identity)
        {
            _m0 = 1.0f;
            _m1 = 0.0f;
            _m2 = 0.0f;

            _m4 = 0.0f;
            _m5 = 1.0f;
            _m6 = 0.0f;

            _m8 = 0.0f;
            _m9 = 0.0f;
            _m10 = 1.0f;
        }

        else
        {
            _m0 = 0.0f;
            _m1 = 0.0f;
            _m2 = 0.0f;

            _m4 = 0.0f;
            _m5 = 0.0f;
            _m6 = 0.0f;

            _m8 = 0.0f;
            _m9 = 0.0f;
            _m10 = 0.0f;
        }

    }

    void Mat3::set(const Row type, const Vec3& V)
    {
        if (type == Row::i0)
        {
            _m0 = V._vx;
            _m1 = V._vy;
            _m2 = V._vz;
        }

        else if (type == Row::i1)
        {
            _m4 = V._vx;
            _m5 = V._vy;
            _m6 = V._vz;
        }

        else
        {
            _m8 = V._vx;
            _m9 = V._vy;
            _m10 = V._vz;
        }
    }

    void Mat3::set(const Vec3& V0, const Vec3& V1, const Vec3& V2)
    {
        _m0 = V0._vx;
        _m1 = V0._vy;
        _m2 = V0._vz;

        _m4 = V1._vx;
        _m5 = V1._vy;
        _m6 = V1._vz;

        _m8 = V2._vx;
        _m9 = V2._vy;
        _m10 = V2._vz;
    }

    Mat3::Mat3(const Mat3& tM)
    {
        _m0 = tM._m0;
        _m1 = tM._m1;
        _m2 = tM._m2;
                    
        _m4 = tM._m4;
        _m5 = tM._m5;
        _m6 = tM._m6;
                    
        _m8 = tM._m8;
        _m9 = tM._m9;
        _m10 = tM._m10;
    }

    Mat3& Mat3::operator = (const Mat3& A)
    {
        _m0 = A._m0;
        _m1 = A._m1;
        _m2 = A._m2;
              
        _m4 = A._m4;
        _m5 = A._m5;
        _m6 = A._m6;
              
        _m8 = A._m8;
        _m9 = A._m9;
        _m10 = A._m10;

        return *this;
    }

    //Destructor
    Mat3::~Mat3()
    {

    }

    float& Mat3::operator[] (const enum m0_enum)
    {
        return this->_m0;
    }
    float& Mat3::operator[] (const enum m1_enum)
    {
        return this->_m1;
    }
    float& Mat3::operator[] (const enum m2_enum)
    {
        return this->_m2;
    }
    float& Mat3::operator[] (const enum m4_enum)
    {
        return this->_m4;
    }
    float& Mat3::operator[] (const enum m5_enum)
    {
        return this->_m5;
    }
    float& Mat3::operator[] (const enum m6_enum)
    {
        return this->_m6;
    }
    float& Mat3::operator[] (const enum m8_enum)
    {
        return this->_m8;
    }
    float& Mat3::operator[] (const enum m9_enum)
    {
        return this->_m9;
    }
    float& Mat3::operator[] (const enum m10_enum)
    {
        return this->_m10;
    }

    const float Mat3::operator[] (const enum m0_enum) const
    {
        return this->_m0;
    }
    const float Mat3::operator[] (const enum m1_enum) const
    {
        return this->_m1;
    }
    const float Mat3::operator[] (const enum m2_enum) const
    {
        return this->_m2;
    }

    const float Mat3::operator[] (const enum m4_enum) const
    {
        return this->_m4;
    }
    const float Mat3::operator[] (const enum m5_enum) const
    {
        return this->_m5;
    }
    const float Mat3::operator[] (const enum m6_enum) const
    {
        return this->_m6;
    }

    const float Mat3::operator[] (const enum m8_enum) const
    {
        return this->_m8;
    }
    const float Mat3::operator[] (const enum m9_enum) const
    {
        return this->_m9;
    }
    const float Mat3::operator[] (const enum m10_enum) const
    {
        return this->_m10;
    }

    void Mat3::m0(const float v)
    {
        this->_m0 = v;
    }

    void Mat3::m1(const float v)
    {
        this->_m1 = v;
    }

    void Mat3::m2(const float v)
    {
        this->_m2 = v;
    }

    void Mat3::m4(const float v)
    {
        this->_m4 = v;
    }

    void Mat3::m5(const float v)
    {
        this->_m5 = v;
    }

    void Mat3::m6(const float v)
    {
        this->_m6 = v;
    }

    void Mat3::m8(const float v)
    {
        this->_m8 = v;
    }

    void Mat3::m9(const float v)
    {
        this->_m9 = v;
    }

    void Mat3::m10(const float v)
    {
        this->_m10 = v;
    }

    const float Mat3::m0() const
    {
        return this->_m0;
    }

    const float Mat3::m1() const
    {
        return this->_m1;
    }

    const float Mat3::m2() const
    {
        return this->_m2;
    }

    const float Mat3::m4() const
    {
        return this->_m4;
    }

    const float Mat3::m5() const
    {
        return this->_m5;
    }

    const float Mat3::m6() const
    {
        return this->_m6;
    }

    const float Mat3::m8() const
    {
        return this->_m8;
    }

    const float Mat3::m9() const
    {
        return this->_m9;
    }

    const float Mat3::m10() const
    {
        return this->_m10;
    }

    const float Mat3::det() const
    {
        return _m0 * (_m5 *_m10 - _m6 * _m9) - _m1 *
            (_m4 * _m10 - _m6 * _m8) + _m2 * (_m4 * _m9 - _m5 * _m8);
    }

    //transpose
    Mat3& Mat3::T(void)
    {
        //save values
        float a0, a1, a2, a3, a4, a5, a6, a7, a8;
        a0 = _m0;
        a1 = _m1;
        a2 = _m2;

        a3 = _m4;
        a4 = _m5;
        a5 = _m6;

        a6 = _m8;
        a7 = _m9;
        a8 = _m10;

        _m0 = a0;
        _m1 = a3;
        _m2 = a6;

        _m4 = a1;
        _m5 = a4;
        _m6 = a7;

        _m8 = a2;
        _m9 = a5;
        _m10 = a8;

        return *this;
    }

    const Mat3 Mat3::getT(void) const
    {
        //save values
        float a0, a1, a2, a3, a4, a5, a6, a7, a8;
        a0 = _m0;
        a1 = _m1;
        a2 = _m2;

        a3 = _m4;
        a4 = _m5;
        a5 = _m6;

        a6 = _m8;
        a7 = _m9;
        a8 = _m10;

        return Mat3(Vec3(a0, a3, a6), Vec3(a1, a4, a7), Vec3(a2, a5, a8));
    }

    const Mat3 Mat3::getInv(void) const
    {
        //find determinant
        float calc;
        calc = det();

        //check if determinant not zero
        if (calc == 0.0)
        {
            //return original
            return *this;
        }

        float invd = 1.0f / calc;
        float a0, a1, a2, a3, a4, a5, a6, a7, a8;

        a0 = (_m5 * _m10 - _m9 * _m6) * invd; 

        a3 = -(_m4 * _m10 - _m8 * _m6) * invd;

        a6 = (_m4 * _m9 - _m8 * _m5) * invd; 

        a1 = -(_m1 * _m10 - _m9 * _m2) * invd;

        a4 = (_m0 * _m10 - _m8 * _m2) * invd; 

        a7 = -(_m0 * _m9 - _m8 * _m1) * invd; 

        a2 = (_m1 * _m6 - _m5 * _m2) * invd; 

        a5 = -(_m0 * _m6 - _m4 * _m2) * invd; 

        a8 = (_m0 * _m5 - _m4 * _m1) * invd; 

        return Mat3(Vec3(a0, a1, a2), Vec3(a3, a4, a5), Vec3(a6, a7, a8));
    }

    Mat3& Mat3::inv(void)
    {
        //find determinant
        float calc;
        calc = det();

        //check if determinant not zero
        if (calc == 0.0)
        {
            //return original
            return *this;
        }

        float invd = 1.0f / calc;
        float a0, a1, a2, a3, a4, a5, a6, a7, a8;

        a0 = (_m5 * _m10 - _m9 * _m6) * invd;

        a3 = -(_m4 * _m10 - _m8 * _m6) * invd;

        a6 = (_m4 * _m9 - _m8 * _m5) * invd;

        a1 = -(_m1 * _m10 - _m9 * _m2) * invd;

        a4 = (_m0 * _m10 - _m8 * _m2) * invd;

        a7 = -(_m0 * _m9 - _m8 * _m1) * invd;

        a2 = (_m1 * _m6 - _m5 * _m2) * invd;

        a5 = -(_m0 * _m6 - _m4 * _m2) * invd;

        a8 = (_m0 * _m5 - _m4 * _m1) * invd;

        _m0 = a0;
        _m4 = a3;
        _m8 = a6;

        _m1 = a1;
        _m5 = a4;
        _m9 = a7;

        _m2 = a2;
        _m6 = a5;
        _m10 = a8;

        return *this;

    }
    
    bool Mat3::isEqual(const Mat3& A, const float epsilon) const
    {
        return (abs(this->_m0 - A._m0) < epsilon) &&
            (abs(this->_m1 - A._m1) < epsilon) &&
            (abs(this->_m2 - A._m2) < epsilon) &&

            (abs(this->_m4 - A._m4) < epsilon) &&
            (abs(this->_m5 - A._m5) < epsilon) &&
            (abs(this->_m6 - A._m6) < epsilon) &&

            (abs(this->_m8 - A._m8) < epsilon) &&
            (abs(this->_m9 - A._m9) < epsilon) &&
            (abs(this->_m10 - A._m10) < epsilon);
    }

    bool Mat3::isIdentity(const float epsilon) const
    {
        return (abs(this->_m0 - 1) < epsilon) &&
            (abs(this->_m1 - 0) < epsilon) &&
            (abs(this->_m2 - 0) < epsilon) &&

            (abs(this->_m4 - 0) < epsilon) &&
            (abs(this->_m5 - 1) < epsilon) &&
            (abs(this->_m6 - 0) < epsilon) &&

            (abs(this->_m8 - 0) < epsilon) &&
            (abs(this->_m9 - 0) < epsilon) &&
            (abs(this->_m10 - 1) < epsilon);
    }

    Mat3 Mat3::operator+(void) const
    {
        return Mat3(Vec3(_m0, _m1, _m2), Vec3(_m4, _m5, _m6), Vec3(_m8, _m9, _m10));
    }

    Mat3 Mat3::operator+(const Mat3& A) const
    {
        return Mat3(Vec3(_m0 + A._m0, _m1 + A._m1, _m2 + A._m2), 
            Vec3(_m4 + A._m4, _m5 + A._m5, _m6 + A._m6), 
            Vec3(_m8 + A._m8, _m9 + A._m9, _m10 + A._m10));
    }

    void Mat3::operator+=(const Mat3& A)
    {
        _m0 += A._m0;
        _m1 += A._m1;
        _m2 += A._m2;

        _m4 += A._m4;
        _m5 += A._m5;
        _m6 += A._m6;

        _m8 += A._m8;
        _m9 += A._m9;
        _m10 += A._m10;
    }

    Mat3 Mat3::operator-(void) const
    {
        return Mat3(Vec3(-_m0, -_m1, -_m2), Vec3(-_m4, -_m5, -_m6), Vec3(-_m8, -_m9, -_m10));
    }

    Mat3 Mat3::operator-(const Mat3& A) const
    {
        return Mat3(Vec3(_m0 - A._m0, _m1 - A._m1, _m2 - A._m2),
            Vec3(_m4 - A._m4, _m5 - A._m5, _m6 - A._m6),
            Vec3(_m8 - A._m8, _m9 - A._m9, _m10 - A._m10));
    }

    void Mat3::operator-=(const Mat3& A)
    {
        _m0 -= A._m0;
        _m1 -= A._m1;
        _m2 -= A._m2;

        _m4 -= A._m4;
        _m5 -= A._m5;
        _m6 -= A._m6;

        _m8 -= A._m8;
        _m9 -= A._m9;
        _m10 -= A._m10;
    }

    Mat3 Mat3::operator*(const float s) const
    {
        return Mat3(Vec3(_m0 * s, _m1 * s, _m2 * s),
            Vec3(_m4 * s, _m5 * s, _m6 * s),
            Vec3(_m8 * s, _m9 * s, _m10 * s));
    }

    void Mat3::operator*=(const float scale)
    {
        _m0 *= scale;
        _m1 *= scale;
        _m2 *= scale;
 
        _m4 *= scale;
        _m5 *= scale;
        _m6 *= scale;

        _m8 *= scale;
        _m9 *= scale;
        _m10 *= scale;
    }

    Mat3 Mat3::operator*(const Mat3& A) const
    {

        return Mat3(Vec3(_m0 * A._m0 + _m1 * A._m4 + _m2 * A._m8,
            _m0 * A._m1 + _m1 * A._m5 + _m2 * A._m9,
            _m0 * A._m2 + _m1 * A._m6 + _m2 * A._m10),
            Vec3(_m4 * A._m0 + _m5 * A._m4 + _m6 * A._m8,
                _m4 * A._m1 + _m5 * A._m5 + _m6 * A._m9,
                _m4 * A._m2 + _m5 * A._m6 + _m6 * A._m10),
            Vec3(_m8 * A._m0 + _m9 * A._m4 + _m10 * A._m8,
                _m8 * A._m1 + _m9 * A._m5 + _m10 * A._m9,
                _m8 * A._m2 + _m9 * A._m6 + _m10 * A._m10));
    }

    void Mat3::operator*=(const Mat3& A)
    {
        float a0, a1, a2, a3, a4, a5, a6, a7, a8;
        a0 = _m0 * A._m0 + _m1 * A._m4 + _m2 * A._m8;
        a1 = _m0 * A._m1 + _m1 * A._m5 + _m2 * A._m9;
        a2 = _m0 * A._m2 + _m1 * A._m6 + _m2 * A._m10;

        a3 = _m4 * A._m0 + _m5 * A._m4 + _m6 * A._m8;
        a4 = _m4 * A._m1 + _m5 * A._m5 + _m6 * A._m9;
        a5 = _m4 * A._m2 + _m5 * A._m6 + _m6 * A._m10;

        a6 = _m8 * A._m0 + _m9 * A._m4 + _m10 * A._m8;
        a7 = _m8 * A._m1 + _m9 * A._m5 + _m10 * A._m9;
        a8 = _m8 * A._m2 + _m9 * A._m6 + _m10 * A._m10;

        _m0 = a0;
        _m1 = a1;
        _m2 = a2;
                
        _m4 = a3;
        _m5 = a4;
        _m6 = a5;
                
        _m8 = a6;
        _m9 = a7;
        _m10 = a8;

    }

    Mat3 operator*(const float scale, const Mat3& A)
    {
        return Mat3(Vec3(A._m0 * scale, A._m1 * scale, A._m2 * scale),
            Vec3(A._m4 * scale, A._m5 * scale, A._m6 * scale),
            Vec3(A._m8 * scale, A._m9 * scale, A._m10 * scale));
    }

}

// ---  End of File ---
