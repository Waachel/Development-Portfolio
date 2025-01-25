//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
    //Default Constructor
    Vec3::Vec3()
        :_vx(0.0f), _vy(0.0f), _vz(0.0f)
    {

    }

    //Specialized Constructor
    Vec3::Vec3(const float in_x, const float in_y, const float in_z)
        :_vx(in_x), _vy(in_y), _vz(in_z)
    {

    }

    Vec3::Vec3(const Vec4& v)
    {
        _vx = v._vx;
        _vy = v._vy;
        _vz = v._vz;
    }

    Vec3& Vec3::operator=(const Vec4& v)
    {
        _vx = v._vx;
        _vy = v._vy;
        _vz = v._vz;

        return *this;
    }

    //Destructor
    Vec3::~Vec3()
    {

    }

    Vec3::Vec3(const Vec3& inV)
        :_vx(inV._vx), _vy(inV._vy), _vz(inV._vz)
    {

    }

    Vec3& Vec3::operator=(const Vec3& v)
    {
        _vx = v._vx;
        _vy = v._vy;
        _vz = v._vz;

        return *this;
    }

    float& Vec3::operator[] (const enum x_enum)
    {
        return this->_vx;
    }

    float& Vec3::operator[] (const enum y_enum)
    {
        return this->_vy;
    }

    float& Vec3::operator[] (const enum z_enum)
    {
        return this->_vz;
    }

    const float Vec3::operator[] (const enum x_enum) const
    {
        return this->_vx;
    }
    const float Vec3::operator[] (const enum y_enum) const
    {
        return this->_vy;
    }
    const float Vec3::operator[] (const enum z_enum) const
    {
        return this->_vz;
    }

    //Accessors
    void Vec3::x(const float v)
    {
        this->_vx = v;
    }
    void Vec3::y(const float v)
    {
        this->_vy = v;
    }
    void Vec3::z(const float v)
    {
        this->_vz = v;
    }

    const float Vec3::x() const
    {
        return this->_vx;
    }
    const float Vec3::y() const
    {
        return this->_vy;
    }
    const float Vec3::z() const
    {
        return this->_vz;
    }

    //Sets
    void Vec3::set(const float inX, const float inY, const float inZ)
    {
        this->_vx = inX;
        this->_vy = inY;
        this->_vz = inZ;
    }
    void Vec3::set(const Vec3& A)
    {
        this->_vx = A._vx;
        this->_vy = A._vy;
        this->_vz = A._vz;
    }

    void Vec3::set(const Vec4& A)
    {
        this->_vx = A._vx;
        this->_vy = A._vy;
        this->_vz = A._vz;
    }

    bool Vec3::isEqual(const Vec3& v, const float epsilon) const
    {
        return (abs(this->_vx - v._vx) < epsilon) &&
            (abs(this->_vy - v._vy) < epsilon) &&
            (abs(this->_vz - v._vz) < epsilon);
    }

    bool Vec3::isZero(const float epsilon) const
    {
        float calc = Vec3(_vx, _vy, _vz).len();
        int intCalc = (int)calc;
        return (calc - (float)intCalc) < epsilon;
    }

    //add
    Vec3 Vec3::operator + (void) const
    {
        return Vec3(_vx, _vy, _vz);
    }
    Vec3 Vec3::operator + (const Vec3& inV) const
    {
        return Vec3(_vx + inV._vx, _vy + inV._vy, _vz + inV._vz);
    }
    void Vec3::operator += (const Vec3& inV)
    {
        _vx += inV._vx;
        _vy += inV._vy;
        _vz += inV._vz;
    }

    //subtract
    Vec3 Vec3::operator - (const Vec3& inV) const
    {
        return Vec3(_vx - inV._vx, _vy - inV._vy, _vz - inV._vz);
    }
    void Vec3::operator -= (const Vec3& inV)
    {
        _vx -= inV._vx;
        _vy -= inV._vy;
        _vz -= inV._vz;
    }
    Vec3 Vec3::operator - (void) const
    {
        return Vec3(-_vx, -_vy, -_vz);
    }

    //scale
    Vec3 Vec3::operator * (const float scale) const
    {
        return Vec3(_vx * scale, _vy * scale, _vz * scale);
    }

    void Vec3::operator *= (const float scale)
    {
        _vx *= scale;
        _vy *= scale;
        _vz *= scale;
    }

    Vec3 operator*(const float scale, const Vec3& inV)
    {
        return Vec3(inV._vx * scale, inV._vy * scale, inV._vz * scale);
    }

    //matrix operation
    Vec3 Vec3::operator*(const Mat3& m) const
    {
        float x, y, z;
        x = dot(Vec3(m._m0, m._m4, m._m8));
        y = dot(Vec3(m._m1, m._m5, m._m9));
        z = dot(Vec3(m._m2, m._m6, m._m10));
        return Vec3(x,y,z);
    }

    Vec3 Vec3::operator*=(const Mat3& m)
    {
        float x, y, z;
        x = dot(Vec3(m._m0, m._m4, m._m8));
        y = dot(Vec3(m._m1, m._m5, m._m9));
        z = dot(Vec3(m._m2, m._m6, m._m10));
        _vx = x;
        _vy = y;
        _vz = z;
        return Vec3(x, y, z);
    }
    
    //Normal


    Vec3& Vec3::norm(void)
    {
        float len = this->len();
        if (len > 0.0f)
        {
            float norm = 1.0f / len;
            _vx = _vx * norm;
            _vy = _vy * norm;
            _vz = _vz * norm;
        }
        return *this;
    }

    Vec3 Vec3::getNorm(void) const
    {
        Vec3 norm = Vec3(_vx, _vy, _vz);
        norm.norm();
        return norm;
    }

    //dot
    const float Vec3::dot(const Vec3& vIn) const
    {
        return _vx * vIn._vx + _vy * vIn._vy + _vz * vIn._vz;
    }

    //cross
    const Vec3 Vec3::cross(const Vec3& vIn) const
    {
        return Vec3(
            _vy * vIn._vz - _vz * vIn._vy,
            _vz * vIn._vx - _vx * vIn._vz,
            _vx * vIn._vy - _vy * vIn._vx        
        );
    }

    const Vec3Proxy Vec3::len(void) const
    {
        return Vec3Proxy(_vx, _vy, _vz);
    }

    const float Vec3::getAngle(const Vec3& vIn) const
    {
        float calc = dot(vIn);

        calc = calc/this->len();

        calc = calc/vIn.len();

        return Trig::acos(calc);
    }

    void Vec3::Print(const char* pName) const
    {

        Trace::out("%s:(Vec3) \n", pName);
        Trace::out(" %9.4g %9.4g %9.4g\n", _vx, _vy, _vz);
        Trace::out("\n");
    }

}

//--- End of File ---
