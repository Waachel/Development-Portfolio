//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	//Default Constructor
    Mat4::Mat4()
        :_m0(0.0f), _m1(0.0f), _m2(0.0f), _m3(0.0f),
        _m4(0.0f), _m5(0.0f), _m6(0.0f), _m7(0.0f),
        _m8(0.0f), _m9(0.0f), _m10(0.0f), _m11(0.0f),
        _m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(0.0f)

    {

    }

    //Specialized Constructor
    Mat4::Mat4::Mat4(const Vec4& tV0, const Vec4& tV1, const Vec4& tV2, const Vec4& tV3)
    {
        _m0 = tV0._vx;
        _m1 = tV0._vy;
        _m2 = tV0._vz;
        _m3 = tV0._vw;
        _m4 = tV1._vx;
        _m5 = tV1._vy;
        _m6 = tV1._vz;
        _m7 = tV1._vw;
        _m8 = tV2._vx;
        _m9 = tV2._vy;
        _m10 = tV2._vz;
        _m11 = tV2._vw;
        _m12 = tV3._vx;
        _m13 = tV3._vy;
        _m14 = tV3._vz;
        _m15 = tV3._vw;

    }

    Mat4::Mat4(const Rot1 type, const float angle)
    {
        this->set(Special::Identity);
        if (type == Rot1::X)
        {
            /*     | 1  m1  m2   m3  | */
            /* m = | m4  cos$  sin$   m7  | */
            /*     | m8  -sin$  cos$  m11 | */
            /*     | m12 m13 m14  m15 | */

            _m5 = _m5 * Trig::cos(angle);
            _m6 = 1 * Trig::sin(angle);
            _m9 = 1 * -Trig::sin(angle);
            _m10 = _m10 * Trig::cos(angle);
        }

        else if (type == Rot1::Y)
        {
            /*     | cos$  m1  -sin$   m3  | */
            /* m = | m4  1  m6   m7  | */
            /*     | sin$  m9  cos$  m11 | */
            /*     | m12 m13 m14  m15 | */

            _m0 = _m0 * Trig::cos(angle);
            _m2 = 1 * -Trig::sin(angle);
            _m8 = 1 * Trig::sin(angle);
            _m10 = _m10 * Trig::cos(angle);
        }

        else
        {
            /*     | cos$  sin$  m2   m3  | */
            /* m = | -sin$ cos$  m6   m7  | */
            /*     | m8  m9  m10  m11 | */
            /*     | m12 m13 m14  m15 | */

            _m0 = _m0 * Trig::cos(angle);
            _m1 = 1 * Trig::sin(angle);
            _m4 = 1 * -Trig::sin(angle);
            _m5 = _m5 * Trig::cos(angle);
        }
    }

    Mat4::Mat4(const Trans t, const float tx, const float ty, const float tz)
    {
        this->set(Special::Identity);
        /*     | m0  m1  m2   m3  | */
        /* m = | m4  m5  m6   m7  | */
        /*     | m8  m9  m10  m11 | */
        /*     | m12 m13 m14  m15 | */
        if ( t == Trans::XYZ)
        {
            _m12 = tx;
            _m13 = ty;
            _m14 = tz;
        }
    }

    Mat4::Mat4(const Trans t, const Vec3& vTrans)
    {
        this->set(Special::Identity);
        /*     | m0  m1  m2   m3  | */
        /* m = | m4  m5  m6   m7  | */
        /*     | m8  m9  m10  m11 | */
        /*     | m12 m13 m14  m15 | */
        if (t == Trans::XYZ)
        {
            _m12 = vTrans.x();
            _m13 = vTrans.y();
            _m14 = vTrans.z();
        }
    }

    Mat4::Mat4(const Scale s, const float sx, const float sy, const float sz)
    {
        this->set(Special::Identity);
        /*     | m0  m1  m2   m3  | */
        /* m = | m4  m5  m6   m7  | */
        /*     | m8  m9  m10  m11 | */
        /*     | m12 m13 m14  m15 | */
        if (s == Scale::XYZ)
        {
            _m0 = _m0 * sx;
            _m1 = _m1 * sx;
            _m2 = _m2 * sx;
            _m3 = _m3 * sx;

            _m4 = _m4 * sy;
            _m5 = _m5 * sy;
            _m6 = _m6 * sy;
            _m7 = _m7 * sy;

            _m8 = _m8 * sz;
            _m9 = _m9 * sz;
            _m10 = _m10 * sz;
            _m11 = _m11 * sz;
        }
    }

    Mat4::Mat4(const Scale s, const Vec3& vScale)
    {
        this->set(Special::Identity);
        /*     | m0  m1  m2   m3  | */
        /* m = | m4  m5  m6   m7  | */
        /*     | m8  m9  m10  m11 | */
        /*     | m12 m13 m14  m15 | */

        if (s == Scale::XYZ)
        {
            _m0 = _m0 * vScale.x();
            _m1 = _m1 * vScale.x();
            _m2 = _m2 * vScale.x();
            _m3 = _m3 * vScale.x();

            _m4 = _m4 * vScale.y();
            _m5 = _m5 * vScale.y();
            _m6 = _m6 * vScale.y();
            _m7 = _m7 * vScale.y();

            _m8 = _m8 * vScale.z();
            _m9 = _m9 * vScale.z();
            _m10 = _m10 * vScale.z();
            _m11 = _m11 * vScale.z();
        }
    }

    Mat4::Mat4(const Rot3 mode, const float angle_0, const float angle_1, const float angle_2)
    {
        this->set(Special::Identity);
        Mat4 tmp(Vec4(_m0, _m4, _m8, _m12), Vec4(_m1, _m5, _m9, _m13), Vec4(_m2, _m6, _m10, _m14), Vec4(_m3, _m7, _m11, _m15));
        if (mode == Rot3::XYZ)
        {
            //X
            /*     | 1  m1  m2   m3  | */
            /* m = | m4  cos$  sin$   m7  | */
            /*     | m8  -sin$  cos$  m11 | */
            /*     | m12 m13 m14  m15 | */

            this->_m5 = this->_m5 * Trig::cos(angle_0);
            this->_m6 = 1 * Trig::sin(angle_0);
            this->_m9 = 1 * -Trig::sin(angle_0);
            this->_m10 = this->_m10 * Trig::cos(angle_0);

            /*     | cos$  m1  -sin$   m3  | */
            /* m = | m4  1  m6   m7  | */
            /*     | sin$  m9  cos$  m11 | */
            /*     | m12 m13 m14  m15 | */

            tmp._m0 = tmp._m0 * Trig::cos(angle_1);
            tmp._m2 = 1 * -Trig::sin(angle_1);
            tmp._m8 = 1 * Trig::sin(angle_1);
            tmp._m10 = tmp._m10 * Trig::cos(angle_1);

            *this = *this * tmp;

            //Z
            /*     | cos$  sin$  m2   m3  | */
            /* m = | -sin$ cos$  m6   m7  | */
            /*     | m8  m9  m10  m11 | */
            /*     | m12 m13 m14  m15 | */

            tmp.set(Special::Identity);

            tmp._m0 = tmp._m0 * Trig::cos(angle_2);
            tmp._m1 = 1 * Trig::sin(angle_2);
            tmp._m4 = 1 * -Trig::sin(angle_2);
            tmp._m5 = tmp._m5 * Trig::cos(angle_2);

            *this = *this * tmp;
        }
    }

    Mat4::Mat4(const Special type)
    {
        if (type == Special::Identity)
        {
            _m0 = 1.0f;
            _m1 = 0.0f;
            _m2 = 0.0f;
            _m3 = 0.0f;

            _m4 = 0.0f;
            _m5 = 1.0f;
            _m6 = 0.0f;
            _m7 = 0.0f;

            _m8 = 0.0f;
            _m9 = 0.0f;
            _m10 = 1.0f;
            _m11 = 0.0f;

            _m12 = 0.0f;
            _m13 = 0.0f;
            _m14 = 0.0f;
            _m15 = 1.0f;
        }

        else
        {
            _m0 = 0.0f;
            _m1 = 0.0f;
            _m2 = 0.0f;
            _m3 = 0.0f;

            _m4 = 0.0f;
            _m5 = 0.0f;
            _m6 = 0.0f;
            _m7 = 0.0f;

            _m8 = 0.0f;
            _m9 = 0.0f;
            _m10 = 0.0f;
            _m11 = 0.0f;

            _m12 = 0.0f;
            _m13 = 0.0f;
            _m14 = 0.0f;
            _m15 = 0.0f;
        }
    }

    void Mat4::set(const Mat4& mIn)
    {
        this->_m0 = mIn._m0;
        this->_m1 = mIn._m1;
        this->_m2 = mIn._m2;
        this->_m3 = mIn._m3;
        this->_m4 = mIn._m4;
        this->_m5 = mIn._m5;
        this->_m6 = mIn._m6;
        this->_m7 = mIn._m7;
        this->_m8 = mIn._m8;
        this->_m9 = mIn._m9;
        this->_m10 = mIn._m10;
        this->_m11 = mIn._m11;
        this->_m12 = mIn._m12;
        this->_m13 = mIn._m13;
        this->_m14 = mIn._m14;
        this->_m15 = mIn._m15;
    }

    void Mat4::set(const Vec4& V0, const Vec4& V1, const Vec4& V2, const Vec4& V3)
    {
        _m0 = V0._vx;
        _m1 = V0._vy;
        _m2 = V0._vz;
        _m3 = V0._vw;

        _m4 = V1._vx;
        _m5 = V1._vy;
        _m6 = V1._vz;
        _m7 = V1._vw;

        _m8 = V2._vx;
        _m9 = V2._vy;
        _m10 = V2._vz;
        _m11 = V2._vw;

        _m12 = V3._vx;
        _m13 = V3._vy;
        _m14 = V3._vz;
        _m15 = V3._vw;
    }

    void Mat4::set(const Rot1 type, const float angle)
    {
        this->set(Special::Identity);
        if (type == Rot1::X)
        {
            /*     | 1  m1  m2   m3  | */
            /* m = | m4  cos$  sin$   m7  | */
            /*     | m8  -sin$  cos$  m11 | */
            /*     | m12 m13 m14  m15 | */

            _m5 = _m5 * Trig::cos(angle);
            _m6 = 1 * Trig::sin(angle);
            _m9 = 1 * -Trig::sin(angle);
            _m10 = _m10 * Trig::cos(angle);
        }

        else if (type == Rot1::Y)
        {
            /*     | cos$  m1  -sin$   m3  | */
            /* m = | m4  1  m6   m7  | */
            /*     | sin$  m9  cos$  m11 | */
            /*     | m12 m13 m14  m15 | */

            _m0 = _m0 * Trig::cos(angle);
            _m2 = 1 * -Trig::sin(angle);
            _m8 = 1 * Trig::sin(angle);
            _m10 = _m10 * Trig::cos(angle);
        }

        else
        {
            /*     | cos$  sin$  m2   m3  | */
            /* m = | -sin$ cos$  m6   m7  | */
            /*     | m8  m9  m10  m11 | */
            /*     | m12 m13 m14  m15 | */

            _m0 = _m0 * Trig::cos(angle);
            _m1 = 1 * Trig::sin(angle);
            _m4 = 1 * -Trig::sin(angle);
            _m5 = _m5 * Trig::cos(angle);
        }
    }

    void Mat4::set(const Rot3 mode, const float angle_0, const float angle_1, const float angle_2)
    {
        this->set(Special::Identity);
        Mat4 tmp(Vec4(_m0, _m4, _m8, _m12), Vec4(_m1, _m5, _m9, _m13), Vec4(_m2, _m6, _m10, _m14), Vec4(_m3, _m7, _m11, _m15));
        if (mode == Rot3::XYZ)
        {
            //X
            /*     | 1  m1  m2   m3  | */
            /* m = | m4  cos$  sin$   m7  | */
            /*     | m8  -sin$  cos$  m11 | */
            /*     | m12 m13 m14  m15 | */

            this->_m5 = this->_m5 * Trig::cos(angle_0);
            this->_m6 = 1 * Trig::sin(angle_0);
            this->_m9 = 1 * -Trig::sin(angle_0);
            this->_m10 = this->_m10 * Trig::cos(angle_0);

            /*     | cos$  m1  -sin$   m3  | */
            /* m = | m4  1  m6   m7  | */
            /*     | sin$  m9  cos$  m11 | */
            /*     | m12 m13 m14  m15 | */

            tmp._m0 = tmp._m0 * Trig::cos(angle_1);
            tmp._m2 = 1 * -Trig::sin(angle_1);
            tmp._m8 = 1 * Trig::sin(angle_1);
            tmp._m10 = tmp._m10 * Trig::cos(angle_1);

            *this = *this * tmp;

            //Z
            /*     | cos$  sin$  m2   m3  | */
            /* m = | -sin$ cos$  m6   m7  | */
            /*     | m8  m9  m10  m11 | */
            /*     | m12 m13 m14  m15 | */

            tmp.set(Special::Identity);

            tmp._m0 = tmp._m0 * Trig::cos(angle_2);
            tmp._m1 = 1 * Trig::sin(angle_2);
            tmp._m4 = 1 * -Trig::sin(angle_2);
            tmp._m5 = tmp._m5 * Trig::cos(angle_2);

            *this = *this * tmp;
        }
    }

    void Mat4::set(const Trans type, const float tx, const float ty, const float tz)
    {
        this->set(Special::Identity);
        /*     | m0  m1  m2   m3  | */
        /* m = | m4  m5  m6   m7  | */
        /*     | m8  m9  m10  m11 | */
        /*     | m12 m13 m14  m15 | */
        if (type == Trans::XYZ)
        {
            _m12 = tx;
            _m13 = ty;
            _m14 = tz;
        }
    }

    void Mat4::set(const Trans type, const Vec3& vTrans)
    {
        this->set(Special::Identity);
        /*     | m0  m1  m2   m3  | */
        /* m = | m4  m5  m6   m7  | */
        /*     | m8  m9  m10  m11 | */
        /*     | m12 m13 m14  m15 | */
        if (type == Trans::XYZ)
        {
            _m12 = vTrans.x();
            _m13 = vTrans.y();
            _m14 = vTrans.z();
        }
    }

    void Mat4::set(const Scale type, const float sx, const float sy, const float sz)
    {
        this->set(Special::Identity);
        /*     | m0  m1  m2   m3  | */
        /* m = | m4  m5  m6   m7  | */
        /*     | m8  m9  m10  m11 | */
        /*     | m12 m13 m14  m15 | */
        if (type == Scale::XYZ)
        {
            _m0 = _m0 * sx;
            _m1 = _m1 * sx;
            _m2 = _m2 * sx;
            _m3 = _m3 * sx;

            _m4 = _m4 * sy;
            _m5 = _m5 * sy;
            _m6 = _m6 * sy;
            _m7 = _m7 * sy;

            _m8 = _m8 * sz;
            _m9 = _m9 * sz;
            _m10 = _m10 * sz;
            _m11 = _m11 * sz;
        }
    }

    void Mat4::set(const Scale type, const Vec3& vScale)
    {
        this->set(Special::Identity);
        if (type == Scale::XYZ)
        {
            _m0 = _m0 * vScale.x();
            _m1 = _m1 * vScale.x();
            _m2 = _m2 * vScale.x();
            _m3 = _m3 * vScale.x();

            _m4 = _m4 * vScale.y();
            _m5 = _m5 * vScale.y();
            _m6 = _m6 * vScale.y();
            _m7 = _m7 * vScale.y();

            _m8 = _m8 * vScale.z();
            _m9 = _m9 * vScale.z();
            _m10 = _m10 * vScale.z();
            _m11 = _m11 * vScale.z();
        }
    }

    void Mat4::set(const Special type)
    {
        if (type == Special::Identity)
        {
            _m0 = 1.0f;
            _m1 = 0.0f;
            _m2 = 0.0f;
            _m3 = 0.0f;
            
            _m4 = 0.0f;
            _m5 = 1.0f;
            _m6 = 0.0f;
            _m7 = 0.0f;
            
            _m8 = 0.0f;
            _m9 = 0.0f;
            _m10 = 1.0f;
            _m11 = 0.0f;
            
            _m12 = 0.0f;
            _m13 = 0.0f;
            _m14 = 0.0f;
            _m15 = 1.0f;
        }

        else
        {
            _m0 = 0.0f;
            _m1 = 0.0f;
            _m2 = 0.0f;
            _m3 = 0.0f;
            _m4 = 0.0f;
            _m5 = 0.0f;
            _m6 = 0.0f;
            _m7 = 0.0f;
            _m8 = 0.0f;
            _m9 = 0.0f;
            _m10 = 0.0f;
            _m11 = 0.0f;
            _m12 = 0.0f;
            _m13 = 0.0f;
            _m14 = 0.0f;
            _m15 = 0.0f;
        }
    }

    void Mat4::set(const Row type, const Vec4& V)
    {
        if (type == Row::i0)
        {
            _m0 = V._vx;
            _m1 = V._vy;
            _m2 = V._vz;
            _m3 = V._vw;
        }

        else if (type == Row::i1)
        {
            _m4 = V._vx;
            _m5 = V._vy;
            _m6 = V._vz;
            _m7 = V._vw;
        }

        else if (type == Row::i2)
        {
            _m8 = V._vx;
            _m9 = V._vy;
            _m10 = V._vz;
            _m11 = V._vw;
        }

        else
        {
            _m12 = V._vx;
            _m13 = V._vy;
            _m14 = V._vz;
            _m15 = V._vw;
        }
    }

    //get row
    Vec4 Mat4::get(const Row type) const
    {
        if (type == Row::i0)
        {
            return Vec4(_m0, _m1, _m2, _m3);
        }

        else if (type == Row::i1)
        {
            return Vec4(_m4, _m5, _m6, _m7);
        }

        else if (type == Row::i2)
        {
            return Vec4(_m8, _m9, _m10, _m11);
        }

        else
        {
            return Vec4(_m12, _m13, _m14, _m15);
        }
    }

    Mat4::Mat4(const Mat4& tM)
    {
        _m0 = tM._m0;
        _m1 = tM._m1;
        _m2 = tM._m2;
        _m3 = tM._m3;
        _m4 = tM._m4;
        _m5 = tM._m5;
        _m6 = tM._m6;
        _m7 = tM._m7;
        _m8 = tM._m8;
        _m9 = tM._m9;
        _m10 = tM._m10;
        _m11 = tM._m11;
        _m12 = tM._m12;
        _m13 = tM._m13;
        _m14 = tM._m14;
        _m15 = tM._m15;
    }

    Mat4& Mat4::operator = (const Mat4& A)
    {
        _m0 = A._m0;
        _m1 = A._m1;
        _m2 = A._m2;
        _m3 = A._m3;
        _m4 = A._m4;
        _m5 = A._m5;
        _m6 = A._m6;
        _m7 = A._m7;
        _m8 = A._m8;
        _m9 = A._m9;
        _m10 = A._m10;
        _m11 = A._m11;
        _m12 = A._m12;
        _m13 = A._m13;
        _m14 = A._m14;
        _m15 = A._m15;

        return *this;
    }

    //Destructor
    Mat4::~Mat4()
    {

    }

    float& Mat4::operator[] (const enum m0_enum)
    {
        return this->_m0;
    }
    float& Mat4::operator[] (const enum m1_enum)
    {
        return this->_m1;
    }
    float& Mat4::operator[] (const enum m2_enum)
    {
        return this->_m2;
    }
    float& Mat4::operator[] (const enum m3_enum)
    {
        return this->_m3;
    }
    float& Mat4::operator[] (const enum m4_enum)
    {
        return this->_m4;
    }
    float& Mat4::operator[] (const enum m5_enum)
    {
        return this->_m5;
    }
    float& Mat4::operator[] (const enum m6_enum)
    {
        return this->_m6;
    }
    float& Mat4::operator[] (const enum m7_enum)
    {
        return this->_m7;
    }
    float& Mat4::operator[] (const enum m8_enum)
    {
        return this->_m8;
    }
    float& Mat4::operator[] (const enum m9_enum)
    {
        return this->_m9;
    }
    float& Mat4::operator[] (const enum m10_enum)
    {
        return this->_m10;
    }
    float& Mat4::operator[] (const enum m11_enum)
    {
        return this->_m11;
    }
    float& Mat4::operator[] (const enum m12_enum)
    {
        return this->_m12;
    }
    float& Mat4::operator[] (const enum m13_enum)
    {
        return this->_m13;
    }
    float& Mat4::operator[] (const enum m14_enum)
    {
        return this->_m14;
    }
    float& Mat4::operator[] (const enum m15_enum)
    {
        return this->_m15;
    }

    const float Mat4::operator[] (const enum m0_enum) const
    {
        return this->_m0;
    }
    const float Mat4::operator[] (const enum m1_enum) const
    {
        return this->_m1;
    }
    const float Mat4::operator[] (const enum m2_enum) const
    {
        return this->_m2;
    }
    const float Mat4::operator[] (const enum m3_enum) const
    {
        return this->_m3;
    }
    const float Mat4::operator[] (const enum m4_enum) const
    {
        return this->_m4;
    }
    const float Mat4::operator[] (const enum m5_enum) const
    {
        return this->_m5;
    }
    const float Mat4::operator[] (const enum m6_enum) const
    {
        return this->_m6;
    }
    const float Mat4::operator[] (const enum m7_enum) const
    {
        return this->_m7;
    }
    const float Mat4::operator[] (const enum m8_enum) const
    {
        return this->_m8;
    }
    const float Mat4::operator[] (const enum m9_enum) const
    {
        return this->_m9;
    }
    const float Mat4::operator[] (const enum m10_enum) const
    {
        return this->_m10;
    }
    const float Mat4::operator[] (const enum m11_enum) const
    {
        return this->_m11;
    }
    const float Mat4::operator[] (const enum m12_enum) const
    {
        return this->_m12;
    }
    const float Mat4::operator[] (const enum m13_enum) const
    {
        return this->_m13;
    }
    const float Mat4::operator[] (const enum m14_enum) const
    {
        return this->_m14;
    }
    const float Mat4::operator[] (const enum m15_enum) const
    {
        return this->_m15;
    }

    //accessors
    void Mat4::m0(const float v)
    {
        this->_m0 = v;
    }
    void Mat4::m1(const float v)
    {
        this->_m1 = v;
    }
    void Mat4::m2(const float v)
    {
        this->_m2 = v;
    }
    void Mat4::m3(const float v)
    {
        this->_m3 = v;
    }
    void Mat4::m4(const float v)
    {
        this->_m4 = v;
    }
    void Mat4::m5(const float v)
    {
        this->_m5 = v;
    }
    void Mat4::m6(const float v)
    {
        this->_m6 = v;
    }
    void Mat4::m7(const float v)
    {
        this->_m7 = v;
    }
    void Mat4::m8(const float v)
    {
        this->_m8 = v;
    }
    void Mat4::m9(const float v)
    {
        this->_m9 = v;
    }
    void Mat4::m10(const float v)
    {
        this->_m10 = v;
    }
    void Mat4::m11(const float v)
    {
        this->_m11 = v;
    }
    void Mat4::m12(const float v)
    {
        this->_m12 = v;
    }
    void Mat4::m13(const float v)
    {
        this->_m13 = v;
    }
    void Mat4::m14(const float v)
    {
        this->_m14 = v;
    }
    void Mat4::m15(const float v)
    {
        this->_m15 = v;
    }

    const float Mat4::m0() const
    {
        return this->_m0;
    }
    const float Mat4::m1() const
    {
        return this->_m1;
    }
    const float Mat4::m2() const
    {
        return this->_m2;
    }
    const float Mat4::m3() const
    {
        return this->_m3;
    }
    const float Mat4::m4() const
    {
        return this->_m4;
    }
    const float Mat4::m5() const
    {
        return this->_m5;
    }
    const float Mat4::m6() const
    {
        return this->_m6;
    }
    const float Mat4::m7() const
    {
        return this->_m7;
    }
    const float Mat4::m8() const
    {
        return this->_m8;
    }
    const float Mat4::m9() const
    {
        return this->_m9;
    }
    const float Mat4::m10() const
    {
        return this->_m10;
    }
    const float Mat4::m11() const
    {
        return this->_m11;
    }
    const float Mat4::m12() const
    {
        return this->_m12;
    }
    const float Mat4::m13() const
    {
        return this->_m13;
    }
    const float Mat4::m14() const
    {
        return this->_m14;
    }
    const float Mat4::m15() const
    {
        return this->_m15;
    }

    // Determinant
    const float Mat4::det() const
    {
        return (_m0 * _m5 * _m10 * _m15 + _m0 * _m6 * _m11 * _m13 + _m0 * _m7 * _m9 * _m14
            - _m0 * _m7 * _m10 * _m13 - _m0 * _m6 * _m9 * _m15 - _m0 * _m5 * _m11 * _m14
            - _m1 * _m4 * _m10 * _m15 - _m2 * _m4 * _m11 * _m13 - _m3 * _m4 * _m9 * _m14
            + _m3 * _m4 * _m10 * _m13 + _m2 * _m4 * _m9 * _m15 + _m1 * _m4 * _m11 * _m14
            + _m1 * _m6 * _m8 * _m15 + _m2 * _m7 * _m8 * _m13 + _m3 * _m5 * _m8 * _m14
            - _m3 * _m6 * _m8 * _m13 - _m2 * _m5 * _m8 * _m15 - _m1 * _m7 * _m8 * _m14
            - _m1 * _m6 * _m11 * _m12 - _m2 * _m7 * _m9 * _m12 - _m3 * _m5 * _m10 * _m12
            + _m3 * _m6 * _m9 * _m12 + _m2 * _m5 * _m11 * _m12 + _m1 * _m7 * _m10 * _m12);
    }

    //Transpose
    Mat4& Mat4::T(void)
    {
        //save values
        float a0, a1, a2, a3, a4, a5, a6, a7, a8,
            a9, a10, a11, a12, a13, a14, a15;

        a0 = _m0;
        a1 = _m1;
        a2 = _m2;
        a3 = _m3;

        a4 = _m4;
        a5 = _m5;
        a6 = _m6;
        a7 = _m7;

        a8 = _m8;
        a9 = _m9;
        a10 = _m10;
        a11 = _m11;

        a12 = _m12;
        a13 = _m13;
        a14 = _m14;
        a15 = _m15;

        _m0 = a0;
        _m1 = a4;
        _m2 = a8;
        _m3 = a12;

        _m4 = a1;
        _m5 = a5;
        _m6 = a9;
        _m7 = a13;

        _m8 = a2;
        _m9 = a6;
        _m10 = a10;
        _m11 = a14;

        _m12 = a3;
        _m13 = a7;
        _m14 = a11;
        _m15 = a15;

        return *this;
    }

    const Mat4 Mat4::getT(void) const
    {
        //save values
        float a0, a1, a2, a3, a4, a5, a6, a7, a8,
            a9, a10, a11, a12, a13, a14, a15;

        a0 = _m0;
        a1 = _m1;
        a2 = _m2;
        a3 = _m3;

        a4 = _m4;
        a5 = _m5;
        a6 = _m6;
        a7 = _m7;

        a8 = _m8;
        a9 = _m9;
        a10 = _m10;
        a11 = _m11;

        a12 = _m12;
        a13 = _m13;
        a14 = _m14;
        a15 = _m15;

        return Mat4(Vec4(a0, a4, a8, a12), Vec4(a1, a5, a9, a13), 
            Vec4(a2, a6, a10, a14), Vec4(a3, a7, a11, a15));;
    }

    const Mat4 Mat4::getInv(void) const
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

        float a0, a1, a2, a3, a4, a5, a6, a7, a8,
            a9, a10, a11, a12, a13, a14, a15;

        a0 = (_m5 * _m10 * _m15 + _m6 * _m11 * _m13 + _m7 * _m9 * _m14
            - _m7 * _m10 * _m13 - _m6 * _m9 * _m15 - _m5 * _m11 * _m14) * invd;

        a1 = (-_m1 * _m10 * _m15 - _m2 * _m11 * _m13 - _m3 * _m9 * _m14
            + _m3 * _m10 * _m13 + _m2 * _m9 * _m15 + _m1 * _m11 * _m14) * invd;

        a2 = (_m1 * _m6 * _m15 + _m2 * _m7 * _m13 + _m3 * _m5 * _m14
            - _m3 * _m6 * _m13 - _m2 * _m5 * _m15 - _m1 * _m7 * _m14) * invd;

        a3 = (-_m1 * _m6 * _m11 - _m2 * _m7 * _m9 - _m3 * _m5 * _m10
            + _m3 * _m6 * _m9 + _m2 * _m5 * _m11 + _m1 * _m7 * _m10) * invd;

        a4 = (-_m4 * _m10 * _m15 - _m6 * _m11 * _m12 - _m7 * _m8 * _m14
            + _m7 * _m10 * _m12 + _m6 * _m8 * _m15 + _m4 * _m11 * _m14) * invd;

        a5 = (_m0 * _m10 * _m15 + _m2 * _m11 * _m12 + _m3 * _m8 * _m14
            - _m3 * _m10 * _m12 - _m2 * _m8 * _m15 - _m0 * _m11 * _m14) * invd;

        a6 = (-_m0 * _m6 * _m15 - _m2 * _m7 * _m12 - _m3 * _m4 * _m14
            + _m3 * _m6 * _m12 + _m2 * _m4 * _m15 + _m0 * _m7 * _m14) * invd;

        a7 = (_m0 * _m6 * _m11 + _m2 * _m7 * _m8 + _m3 * _m4 * _m10
            - _m3 * _m6 * _m8 - _m2 * _m4 * _m11 - _m0 * _m7 * _m10) * invd;

        a8 = (_m4 * _m9 * _m15 + _m5 * _m11 * _m12 + _m7 * _m8 * _m13
            - _m7 * _m9 * _m12 - _m5 * _m8 * _m15 - _m4 * _m11 * _m13) * invd;

        a9 = (-_m0 * _m9 * _m15 - _m1 * _m11 * _m12 - _m3 * _m8 * _m13
            + _m3 * _m9 * _m12 + _m1 * _m8 * _m15 + _m0 * _m11 * _m13) * invd;

        a10 = (_m0 * _m5 * _m15 + _m1 * _m7 * _m12 + _m3 * _m4 * _m13
            - _m3 * _m5 * _m12 - _m1 * _m4 * _m15 - _m0 * _m7 * _m13) * invd;

        a11 = (-_m0 * _m5 * _m11 - _m1 * _m7 * _m8 - _m3 * _m4 * _m9
            + _m3 * _m5 * _m8 + _m1 * _m4 * _m11 + _m0 * _m7 * _m9) * invd;

        a12 = (-_m4 * _m9 * _m14 - _m5 * _m10 * _m12 - _m6 * _m8 * _m13
            + _m6 * _m9 * _m12 + _m5 * _m8 * _m14 + _m4 * _m10 * _m13) * invd;

        a13 = (_m0 * _m9 * _m14 + _m1 * _m10 * _m12 + _m2 * _m8 * _m13
            - _m2 * _m9 * _m12 - _m1 * _m8 * _m14 - _m0 * _m10 * _m13) * invd;

        a14 = (-_m0 * _m5 * _m14 - _m1 * _m6 * _m12 - _m2 * _m4 * _m13
            + _m2 * _m5 * _m12 + _m1 * _m4 * _m14 + _m0 * _m6 * _m13) * invd;

        a15 = (_m0 * _m5 * _m10 + _m1 * _m6 * _m8 + _m2 * _m4 * _m9
            - _m2 * _m5 * _m8 - _m1 * _m4 * _m10 - _m0 * _m6 * _m9) * invd;

        return Mat4(Vec4(a0, a1, a2, a3), Vec4(a4, a5, a6, a7), Vec4(a8, a9, a10, a11), Vec4(a12, a13, a14, a15));
    }

    Mat4& Mat4::inv(void)
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

        float a0, a1, a2, a3, a4, a5, a6, a7, a8,
            a9, a10, a11, a12, a13, a14, a15;

        a0 = (_m5 * _m10 * _m15 + _m6 * _m11 * _m13 + _m7 * _m9 * _m14
            - _m7 * _m10 * _m13 - _m6 * _m9 * _m15 - _m5 * _m11 * _m14) * invd;

        a1 = (- _m1 * _m10 * _m15 - _m2 * _m11 * _m13 - _m3 * _m9 * _m14
            + _m3 * _m10 * _m13 + _m2 * _m9 * _m15 + _m1 * _m11 * _m14) * invd;

        a2 = (_m1 * _m6 * _m15 + _m2 * _m7 * _m13 + _m3 * _m5 * _m14
            - _m3 * _m6 * _m13 - _m2 * _m5 * _m15 - _m1 * _m7 * _m14) * invd;

        a3 = (- _m1 * _m6 * _m11 - _m2 * _m7 * _m9 - _m3 * _m5 * _m10
            + _m3 * _m6 * _m9 + _m2 * _m5 * _m11 + _m1 * _m7 * _m10) * invd;

        a4 = (- _m4 * _m10 * _m15 - _m6 * _m11 * _m12 - _m7 * _m8 * _m14
            + _m7 * _m10 * _m12 + _m6 * _m8 * _m15 + _m4 * _m11 * _m14) * invd;

        a5 = (_m0 * _m10 * _m15 + _m2 * _m11 * _m12 + _m3 * _m8 * _m14
            - _m3 * _m10 * _m12 - _m2 * _m8 * _m15 - _m0 * _m11 * _m14) * invd;

        a6 = (- _m0 * _m6 * _m15 - _m2 * _m7 * _m12 - _m3 * _m4 * _m14
            + _m3 * _m6 * _m12 + _m2 * _m4 * _m15 + _m0 * _m7 * _m14) * invd;

        a7 = (_m0 * _m6 * _m11 + _m2 * _m7 * _m8 + _m3 * _m4 * _m10
            - _m3 * _m6 * _m8 - _m2 * _m4 * _m11 - _m0 * _m7 * _m10) * invd;

        a8 = (_m4 * _m9 * _m15 + _m5 * _m11 * _m12 + _m7 * _m8 * _m13
            - _m7 * _m9 * _m12 - _m5 * _m8 * _m15 - _m4 * _m11 * _m13) * invd;

        a9 = (- _m0 * _m9 * _m15 - _m1 * _m11 * _m12 - _m3 * _m8 * _m13
            + _m3 * _m9 * _m12 + _m1 * _m8 * _m15 + _m0 * _m11 * _m13) * invd;

        a10 = (_m0 * _m5 * _m15 + _m1 * _m7 * _m12 + _m3 * _m4 * _m13
            - _m3 * _m5 * _m12 - _m1 * _m4 * _m15 - _m0 * _m7 * _m13) * invd;

        a11 = (- _m0 * _m5 * _m11 - _m1 * _m7 * _m8 - _m3 * _m4 * _m9
            + _m3 * _m5 * _m8 + _m1 * _m4 * _m11 + _m0 * _m7 * _m9) * invd;

        a12 = (- _m4 * _m9 * _m14 - _m5 * _m10 * _m12 - _m6 * _m8 * _m13
            + _m6 * _m9 * _m12 + _m5 * _m8 * _m14 + _m4 * _m10 * _m13) * invd;

        a13 = (_m0 * _m9 * _m14 + _m1 * _m10 * _m12 + _m2 * _m8 * _m13
            - _m2 * _m9 * _m12 - _m1 * _m8 * _m14 - _m0 * _m10 * _m13) * invd;

        a14 = (- _m0 * _m5 * _m14 - _m1 * _m6 * _m12 - _m2 * _m4 * _m13
            + _m2 * _m5 * _m12 + _m1 * _m4 * _m14 + _m0 * _m6 * _m13) * invd;

        a15 = (_m0 * _m5 * _m10 + _m1 * _m6 * _m8 + _m2 * _m4 * _m9
            - _m2 * _m5 * _m8 - _m1 * _m4 * _m10 - _m0 * _m6 * _m9) * invd;

        _m0 = a0;
        _m4 = a4;
        _m8 = a8;
        _m12 = a12;

        _m1 = a1;
        _m5 = a5;
        _m9 = a9;
        _m13 = a13;

        _m2 = a2;
        _m6 = a6;
        _m10 = a10;
        _m14 = a14;

        _m3 = a3;
        _m7 = a7;
        _m11 = a11;
        _m15 = a15;

        return *this;

    }

    //comparisons
    const bool Mat4::isEqual(const Mat4& A, const float epsilon) const
    {
        return (abs(this->_m0 - A._m0) < epsilon) &&
            (abs(this->_m1 - A._m1) < epsilon) &&
            (abs(this->_m2 - A._m2) < epsilon) &&
            (abs(this->_m3 - A._m3) < epsilon) &&

            (abs(this->_m4 - A._m4) < epsilon) &&
            (abs(this->_m5 - A._m5) < epsilon) &&
            (abs(this->_m6 - A._m6) < epsilon) &&
            (abs(this->_m7 - A._m7) < epsilon) &&

            (abs(this->_m8 - A._m8) < epsilon) &&
            (abs(this->_m9 - A._m9) < epsilon) &&
            (abs(this->_m10 - A._m10) < epsilon) &&
            (abs(this->_m11 - A._m11) < epsilon) &&
            
            (abs(this->_m12 - A._m12) < epsilon) &&
            (abs(this->_m13 - A._m13) < epsilon) &&
            (abs(this->_m14 - A._m14) < epsilon) &&
            (abs(this->_m15 - A._m15) < epsilon);
    }

    const bool Mat4::isIdentity(const float epsilon) const
    {
        return (abs(this->_m0 - 1) < epsilon) &&
            (abs(this->_m1 - 0) < epsilon) &&
            (abs(this->_m2 - 0) < epsilon) &&
            (abs(this->_m3 - 0) < epsilon) &&

            (abs(this->_m4 - 0) < epsilon) &&
            (abs(this->_m5 - 1) < epsilon) &&
            (abs(this->_m6 - 0) < epsilon) &&
            (abs(this->_m7 - 0) < epsilon) &&

            (abs(this->_m8 - 0) < epsilon) &&
            (abs(this->_m9 - 0) < epsilon) &&
            (abs(this->_m10 - 1) < epsilon) &&
            (abs(this->_m11 - 0) < epsilon) &&

            (abs(this->_m12 - 0) < epsilon) &&
            (abs(this->_m13 - 0) < epsilon) &&
            (abs(this->_m14 - 0) < epsilon) &&
            (abs(this->_m15 - 1) < epsilon);
    }

    //add operators
    Mat4 Mat4::operator+(void) const
    {
        return Mat4(Vec4(_m0, _m1, _m2, _m3), Vec4(_m4, _m5, _m6, _m7), 
            Vec4(_m8, _m9, _m10, _m11), Vec4(_m12, _m13, _m14, _m15));
    }

    Mat4 Mat4::operator+(const Mat4& A) const
    {
        return Mat4(Vec4(_m0 + A._m0, _m1 + A._m1, _m2 + A._m2, _m3 + A._m3),
            Vec4(_m4 + A._m4, _m5 + A._m5, _m6 + A._m6, _m7 + A._m7),
            Vec4(_m8 + A._m8, _m9 + A._m9, _m10 + A._m10, _m11 + A._m11),
            Vec4(_m12 + A._m12, _m13 + A._m13, _m14 + A._m14, _m15 + A._m15));
    }

    void Mat4::operator+=(const Mat4& A)
    {
        _m0 += A._m0;
        _m1 += A._m1;
        _m2 += A._m2;
        _m3 += A._m3;

        _m4 += A._m4;
        _m5 += A._m5;
        _m6 += A._m6;
        _m7 += A._m7;

        _m8 += A._m8;
        _m9 += A._m9;
        _m10 += A._m10;
        _m11 += A._m11;

        _m12 += A._m12;
        _m13 += A._m13;
        _m14 += A._m14;
        _m15 += A._m15;
    }

    //subtraction
    Mat4 Mat4::operator-(void) const
    {
        return Mat4(Vec4(-_m0, -_m1, -_m2, -_m3), Vec4(-_m4, -_m5, -_m6, -_m7),
            Vec4(-_m8, -_m9, -_m10, -_m11), Vec4(-_m12, -_m13, -_m14, -_m15));
    }

    Mat4 Mat4::operator-(const Mat4& A) const
    {
        return Mat4(Vec4(_m0 - A._m0, _m1 - A._m1, _m2 - A._m2, _m3 - A._m3),
            Vec4(_m4 - A._m4, _m5 - A._m5, _m6 - A._m6, _m7 - A._m7),
            Vec4(_m8 - A._m8, _m9 - A._m9, _m10 - A._m10, _m11 - A._m11),
            Vec4(_m12 - A._m12, _m13 - A._m13, _m14 - A._m14, _m15 - A._m15));
    }

    void Mat4::operator-=(const Mat4& A)
    {
        _m0 -= A._m0;
        _m1 -= A._m1;
        _m2 -= A._m2;
        _m3 -= A._m3;

        _m4 -= A._m4;
        _m5 -= A._m5;
        _m6 -= A._m6;
        _m7 -= A._m7;

        _m8 -= A._m8;
        _m9 -= A._m9;
        _m10 -= A._m10;
        _m11 -= A._m11;

        _m12 -= A._m12;
        _m13 -= A._m13;
        _m14 -= A._m14;
        _m15 -= A._m15;
    }

    // Scale operators
    Mat4 Mat4::operator*(const float s) const
    {
        return Mat4(Vec4(_m0 * s, _m1 * s, _m2 * s, _m3 * s),
            Vec4(_m4 * s, _m5 * s, _m6 * s, _m7 * s),
            Vec4(_m8 * s, _m9 * s, _m10 * s, _m11 * s),
            Vec4(_m12 * s, _m13 * s, _m14 * s, _m15 * s));
    }

    void Mat4::operator*=(const float scale)
    {
        _m0 *= scale;
        _m1 *= scale;
        _m2 *= scale;
        _m3 *= scale;

        _m4 *= scale;
        _m5 *= scale;
        _m6 *= scale;
        _m7 *= scale;

        _m8 *= scale;
        _m9 *= scale;
        _m10 *= scale;
        _m11 *= scale;

        _m12 *= scale;
        _m13 *= scale;
        _m14 *= scale;
        _m15 *= scale;
    }

    //Multiply operators
    Mat4 Mat4::operator*(const Mat4& A) const
    {
        /*     | m0  m1  m2   m3  | */
        /* m = | m4  m5  m6   m7  | */
        /*     | m8  m9  m10  m11 | */
        /*     | m12 m13 m14  m15 | */
        return Mat4(Vec4(_m0 * A._m0 + _m1 * A._m4 + _m2 * A._m8 + _m3 * A._m12,
            _m0 * A._m1 + _m1 * A._m5 + _m2 * A._m9 + _m3 * A._m13,
            _m0 * A._m2 + _m1 * A._m6 + _m2 * A._m10 + _m3 * A._m14,
            _m0 * A._m3 + _m1 * A._m7 + _m2 * A._m11 + _m3 * A._m15),
            Vec4(_m4 * A._m0 + _m5 * A._m4 + _m6 * A._m8 + _m7 * A._m12,
                _m4 * A._m1 + _m5 * A._m5 + _m6 * A._m9 + _m7 * A._m13,
                _m4 * A._m2 + _m5 * A._m6 + _m6 * A._m10 + _m7 * A._m14,
                _m4 * A._m3 + _m5 * A._m7 + _m6 * A._m11 + _m7 * A._m15),
            Vec4(_m8 * A._m0 + _m9 * A._m4 + _m10 * A._m8 + _m11 * A._m12,
                _m8 * A._m1 + _m9 * A._m5 + _m10 * A._m9 + _m11 * A._m13,
                _m8 * A._m2 + _m9 * A._m6 + _m10 * A._m10 + _m11 * A._m14,
                _m8 * A._m3 + _m9 * A._m7 + _m10 * A._m11 + _m11 * A._m15),
            Vec4(_m12 * A._m0 + _m13 * A._m4 + _m14 * A._m8 + _m15 * A._m12,
                _m12 * A._m1 + _m13 * A._m5 + _m14 * A._m9 + _m15 * A._m13,
                _m12 * A._m2 + _m13 * A._m6 + _m14 * A._m10 + _m15 * A._m14,
                _m12 * A._m3 + _m13 * A._m7 + _m14 * A._m11 + _m15 * A._m15));
    }

    void Mat4::operator*=(const Mat4& A)
    {
        float a0, a1, a2, a3, a4, a5, a6, a7, a8,
            a9, a10, a11, a12, a13, a14, a15;
        a0 = _m0 * A._m0 + _m1 * A._m4 + _m2 * A._m8 + _m3 * A._m12;
        a1 = _m0 * A._m1 + _m1 * A._m5 + _m2 * A._m9 + _m3 * A._m13;
        a2 = _m0 * A._m2 + _m1 * A._m6 + _m2 * A._m10 + _m3 * A._m14;
        a3 = _m0 * A._m3 + _m1 * A._m7 + _m2 * A._m11 + _m3 * A._m15;
        
        a4 = _m4 * A._m0 + _m5 * A._m4 + _m6 * A._m8 + _m7 * A._m12;
        a5 = _m4 * A._m1 + _m5 * A._m5 + _m6 * A._m9 + _m7 * A._m13;
        a6 = _m4 * A._m2 + _m5 * A._m6 + _m6 * A._m10 + _m7 * A._m14;
        a7 = _m4 * A._m3 + _m5 * A._m7 + _m6 * A._m11 + _m7 * A._m15;
       
        a8 = _m8 * A._m0 + _m9 * A._m4 + _m10 * A._m8 + _m11 * A._m12;
        a9 = _m8 * A._m1 + _m9 * A._m5 + _m10 * A._m9 + _m11 * A._m13;
        a10 = _m8 * A._m2 + _m9 * A._m6 + _m10 * A._m10 + _m11 * A._m14;
        a11 = _m8 * A._m3 + _m9 * A._m7 + _m10 * A._m11 + _m11 * A._m15;
        
        a12 = _m12 * A._m0 + _m13 * A._m4 + _m14 * A._m8 + _m15 * A._m12;
        a13 = _m12 * A._m1 + _m13 * A._m5 + _m14 * A._m9 + _m15 * A._m13;
        a14 = _m12 * A._m2 + _m13 * A._m6 + _m14 * A._m10 + _m15 * A._m14;
        a15 = _m12 * A._m3 + _m13 * A._m7 + _m14 * A._m11 + _m15 * A._m15;

        _m0 = a0;
        _m1 = a1;
        _m2 = a2;
        _m3 = a3;
        _m4 = a4;
        _m5 = a5;
        _m6 = a6;
        _m7 = a7;
        _m8 = a8;
        _m9 = a9;
        _m10 = a10;
        _m11 = a11;
        _m12 = a12;
        _m13 = a13;
        _m14 = a14;
        _m15 = a15;
    }


    Mat4::Mat4(const Rot r, const Vec3& vAxis, const float angle_radians)
    {
        if (r == Rot::AxisAngle)
        {
            float c = cos(angle_radians);
            float s = sin(angle_radians);
            float t = 1.0f - c;

            Vec3 norm = vAxis.getNorm();

            _m0 = c + norm.x() * norm.x() * t;
            _m5 = c + norm.y() * norm.y() * t;
            _m10 = c + norm.z() * norm.z() * t;

            float tmp1 = norm.x() * norm.y() * t;
            float tmp2 = norm.z() * s;

            _m1 = tmp1 + tmp2;
            _m4 = tmp1 - tmp2;
            tmp1 = norm.x() * norm.z() * t;
            tmp2 = norm.y() * s;
            _m2 = tmp1 - tmp2;
            _m8 = tmp1 + tmp2;    tmp1 = norm.y() * norm.z() * t;
            tmp2 = norm.x() * s;
            _m6 = tmp1 + tmp2;
            _m9 = tmp1 - tmp2;

            _m3 = 0;
            _m7 = 0;
            _m11 = 0;
            _m12 = 0;
            _m13 = 0;
            _m14 = 0;
            _m15 = 1;
        }

    }

    Mat4::Mat4(const Orient type, const Vec3& dof, const Vec3& up)
    {
        if (type == Orient::LocalToWorld)
        {
            const Vec3 rz = dof.getNorm();

            /* find rx */
            Vec3 rx = up.cross(rz);
            rx.norm();

            /* find ry */
            Vec3 ry = rz.cross(rx);
            ry.norm();

            this->set(Vec4(rx, 0), Vec4(ry, 0), Vec4(rz, 0), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
            this->_m3 = 0.0f;
            this->_m7 = 0.0f;
            this->_m11 = 0.0f;
            this->_m15 = 1.0f;
        }
    }

    void Mat4::set(const Rot r, const Vec3& vAxis, const float angle_radians)
    {
        if (r == Rot::AxisAngle)
        {
            float t = 1.0f - Trig::cos(angle_radians);

            Vec3 norm = vAxis.getNorm();

            _m0 = Trig::cos(angle_radians) + norm.x() * norm.x() * t;
            _m5 = Trig::cos(angle_radians) + norm.y() * norm.y() * t;
            _m10 = Trig::cos(angle_radians) + norm.z() * norm.z() * t;

            float tmp1 = norm.x() * norm.y() * t;
            float tmp2 = norm.z() * Trig::sin(angle_radians);

            _m1 = tmp1 + tmp2;
            _m4 = tmp1 - tmp2;
            tmp1 = norm.x() * norm.z() * t;
            tmp2 = norm.y() * Trig::sin(angle_radians);
            _m2 = tmp1 - tmp2;
            _m8 = tmp1 + tmp2;    tmp1 = norm.y() * norm.z() * t;
            tmp2 = norm.x() * Trig::sin(angle_radians);
            _m6 = tmp1 + tmp2;
            _m9 = tmp1 - tmp2;

            _m3 = 0;
            _m7 = 0;
            _m11 = 0;
            _m12 = 0;
            _m13 = 0;
            _m14 = 0;
            _m15 = 1;
        }
    }

    void Mat4::set(const Orient o, const Vec3& dof, const Vec3& up)
    {
        if (o == Orient::LocalToWorld)
        {
            const Vec3 rz = dof.getNorm();

            /* find rx */
            Vec3 rx = up.cross(rz);
            rx.norm();

            /* find ry */
            Vec3 ry = rz.cross(rx);
            ry.norm();

            this->set(Vec4(rx, 0), Vec4(ry, 0), Vec4(rz, 0), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
            this->_m3 = 0.0f;
            this->_m7 = 0.0f;
            this->_m11 = 0.0f;
            this->_m15 = 1.0f;
        }
    }

    Mat4 operator*(const float scale, const Mat4& A)
    {
        return Mat4(Vec4(A._m0 * scale, A._m1 * scale, A._m2 * scale, A._m3 * scale),
            Vec4(A._m4 * scale, A._m5 * scale, A._m6 * scale, A._m7 * scale),
            Vec4(A._m8 * scale, A._m9 * scale, A._m10 * scale, A._m11 * scale),
            Vec4(A._m12 * scale, A._m13 * scale, A._m14 * scale, A._m15 * scale));
    }

    bool Mat4::privHasHint() const
    {
        return ((_m15_bits & 0xFFFFFFF0) == 0x3f800000);
    }

}

// ---  End of File ---
