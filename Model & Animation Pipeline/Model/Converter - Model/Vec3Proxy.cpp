//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Add Proxy here

	Vec3Proxy::Vec3Proxy(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}

	Vec3Proxy::operator float() const
	{
		return Trig::sqrt(x * x + y * y + z * z);
	}

	float Vec3Proxy::operator*(const Vec3Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z) * r;
	}

	bool Vec3Proxy::operator>(const Vec3Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z) > r;
	}

	bool Vec3Proxy::operator<(const Vec3Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z) < r;
	}

	bool Vec3Proxy::operator==(const Vec3Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z) == r;
	}

	bool Vec3Proxy::operator>=(const Vec3Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z) >= r;
	}

	bool Vec3Proxy::operator<=(const Vec3Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z) <= r;
	}

	bool Vec3Proxy::operator!=(const Vec3Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z) != r;
	}

}

//--- End of File ---
