//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
    //Default Constructor
    Vec4::Vec4()
        :_vx(0.0f), _vy(0.0f), _vz(0.0f), _vw(0.0f)
    {

    }

    //Specialized Constructor
    Vec4::Vec4(const float in_x, const float in_y, const float in_z, const float in_w)
        :_vx(in_x), _vy(in_y), _vz(in_z), _vw(in_w)
    {

    }

    Vec4::Vec4(const Vec3& v, const float w)
    {
        _vx = v._vx;
        _vy = v._vy;
        _vz = v._vz;
        _vw = w;
    }

    //Destructor
    Vec4::~Vec4()
    {

    }

    Vec4::Vec4(const Vec4& inV)
        :_vx(inV._vx), _vy(inV._vy), _vz(inV._vz), _vw(inV._vw)
    {

    }

    Vec4& Vec4::operator=(const Vec4& v)
    {
        _vx = v._vx;
        _vy = v._vy;
        _vz = v._vz;
        _vw = v._vw;

        return *this;
    }

    float& Vec4::operator[] (const enum x_enum)
    {
        return this->_vx;
    }

    float& Vec4::operator[] (const enum y_enum)
    {
        return this->_vy;
    }

    float& Vec4::operator[] (const enum z_enum)
    {
        return this->_vz;
    }

    float& Vec4::operator[] (const enum w_enum)
    {
        return this->_vw;
    }

    const float Vec4::operator[] (const enum x_enum) const
    {
        return this->_vx;
    }
    const float Vec4::operator[] (const enum y_enum) const
    {
        return this->_vy;
    }
    const float Vec4::operator[] (const enum z_enum) const
    {
        return this->_vz;
    }

    const float Vec4::operator[] (const enum w_enum) const
    {
        return this->_vw;
    }

    //Accessors
    void Vec4::x(const float v)
    {
        this->_vx = v;
    }
    void Vec4::y(const float v)
    {
        this->_vy = v;
    }
    void Vec4::z(const float v)
    {
        this->_vz = v;
    }
    void Vec4::w(const float v)
    {
        this->_vw = v;
    }

    const float Vec4::x() const
    {
        return this->_vx;
    }
    const float Vec4::y() const
    {
        return this->_vy;
    }
    const float Vec4::z() const
    {
        return this->_vz;
    }
    const float Vec4::w() const
    {
        return this->_vw;
    }

    //Sets
    void Vec4::set(const float inX, const float inY, const float inZ, const float inW)
    {
        this->_vx = inX;
        this->_vy = inY;
        this->_vz = inZ;
        this->_vw = inW;
    }
    void Vec4::set(const Vec4& A)
    {
        this->_vx = A._vx;
        this->_vy = A._vy;
        this->_vz = A._vz;
        this->_vw = A._vw;
    }

    void Vec4::set(const Vec3& v, const float w)
    {
        this->_vx = v._vx;
        this->_vy = v._vy;
        this->_vz = v._vz;
        this->_vw = w;
    }

    bool Vec4::isEqual(const Vec4& v, const float epsilon) const
    {
        return (abs(this->_vx - v._vx) < epsilon) &&
            (abs(this->_vy - v._vy) < epsilon) &&
            (abs(this->_vz - v._vz) < epsilon) &&
            (abs(this->_vw - v._vw) < epsilon);
    }

    bool Vec4::isZero(const float epsilon) const
    {
        float calc = Vec4(_vx, _vy, _vz, _vw).len();
        int intCalc = (int)calc;
        return (calc - (float)intCalc) < epsilon;
    }

    //add
    Vec4 Vec4::operator + (void) const
    {
        return Vec4(_vx, _vy, _vz, _vw);
    }
    Vec4 Vec4::operator + (const Vec4& inV) const
    {
        return Vec4(_vx + inV._vx, _vy + inV._vy, _vz + inV._vz, _vw + inV._vw);
    }
    void Vec4::operator += (const Vec4& inV)
    {
        _vx += inV._vx;
        _vy += inV._vy;
        _vz += inV._vz;
        _vw += inV._vw;
    }

    //subtract
    Vec4 Vec4::operator - (const Vec4& inV) const
    {
        return Vec4(_vx - inV._vx, _vy - inV._vy, _vz - inV._vz, _vw - inV._vw);
    }
    void Vec4::operator -= (const Vec4& inV)
    {
        _vx -= inV._vx;
        _vy -= inV._vy;
        _vz -= inV._vz;
        _vw -= inV._vw;
    }
    Vec4 Vec4::operator - (void) const
    {
        return Vec4(-_vx, -_vy, -_vz, -_vw);
    }

    //scale
    Vec4 Vec4::operator * (const float scale) const
    {
        return Vec4(_vx * scale, _vy * scale, _vz * scale, _vw * scale);
    }

    void Vec4::operator *= (const float scale)
    {
        _vx *= scale;
        _vy *= scale;
        _vz *= scale;
        _vw *= scale;
    }

    Vec4 Vec4::operator*(const Mat4& m) const
    {
        float x, y, z, w;
        x = dot(Vec4(m._m0, m._m4, m._m8, m._m12));
        y = dot(Vec4(m._m1, m._m5, m._m9, m._m13));
        z = dot(Vec4(m._m2, m._m6, m._m10, m._m14));
        w = dot(Vec4(m._m3, m._m7, m._m11, m._m15));
        return Vec4(x, y, z, w);
    }

    Vec4 Vec4::operator*=(const Mat4& m)
    {
        float x, y, z, w;
        x = dot(Vec4(m._m0, m._m4, m._m8, m._m12));
        y = dot(Vec4(m._m1, m._m5, m._m9, m._m13));
        z = dot(Vec4(m._m2, m._m6, m._m10, m._m14));
        w = dot(Vec4(m._m3, m._m7, m._m11, m._m15));
        _vx = x;
        _vy = y;
        _vz = z;
        _vw = w;
        return Vec4(x, y, z, w);
    }

    Vec4 operator*(const float scale, const Vec4& inV)
    {
        return Vec4(inV._vx * scale, inV._vy * scale, inV._vz * scale, inV._vw * scale);
    }

    //Normal


    Vec4& Vec4::norm(void)
    {
        float len = this->len();
        if (len > 0.0f)
        {
            float norm = 1.0f / len;
            _vx = _vx * norm;
            _vy = _vy * norm;
            _vz = _vz * norm;
            _vw = _vw * norm;
        }
        return *this;
    }

    Vec4 Vec4::getNorm(void) const
    {
        Vec4 norm = Vec4(_vx, _vy, _vz, _vw);
        norm.norm();
        return norm;
    }

    //dot
    const float Vec4::dot(const Vec4& vIn) const
    {
        return _vx * vIn._vx + _vy * vIn._vy + _vz * vIn._vz + _vw * vIn._vw;
    }

    const Vec4Proxy Vec4::len() const
    {
        return Vec4Proxy(_vx, _vy, _vz, _vw);
    }

    void Vec4::Print(const char* pName) const
    {

        Trace::out("%s:(Vec4) \n", pName);
        Trace::out(" %9.4g %9.4g %9.4g %9.4g\n", _vx, _vy, _vz, _vw);
        Trace::out("\n");
    }

}

//--- End of File ---
