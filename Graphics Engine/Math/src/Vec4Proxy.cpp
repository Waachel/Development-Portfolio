//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Add Proxy here
	Vec4Proxy::Vec4Proxy(float a, float b, float c, float d)
	{
		x = a;
		y = b;
		z = c;
		w = d;
	}

	Vec4Proxy::operator float() const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w);
	}

	float Vec4Proxy::operator*(const Vec4Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w) * r;
	}

	bool Vec4Proxy::operator>(const Vec4Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w) > r;
	}

	bool Vec4Proxy::operator<(const Vec4Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w) < r;
	}

	bool Vec4Proxy::operator==(const Vec4Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w) == r;
	}

	bool Vec4Proxy::operator>=(const Vec4Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w) >= r;
	}

	bool Vec4Proxy::operator<=(const Vec4Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w) <= r;
	}

	bool Vec4Proxy::operator!=(const Vec4Proxy& r) const
	{
		return Trig::sqrt(x * x + y * y + z * z + w * w) != r;
	}
}

//--- End of File ---
