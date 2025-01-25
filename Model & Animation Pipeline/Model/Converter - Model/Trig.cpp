//-----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include <math.h>    // <--- ONLY location for <math.h> allowed
#include "Trig.h"

namespace Azul
{

	// Do your magic here

	float Trig::cos(const float angle_radians)
	{
		return std::cosf(angle_radians);
	}

	float Trig::sin(const float angle_radians)
	{
		return std::sinf(angle_radians);
	}

	float Trig::tan(const float val)
	{
		return std::tanf(val);
	}

	float Trig::atan(const float val)
	{
		return std::atanf(val);
	}

	float Trig::atan2(const float x, const float y)
	{
		return std::atan2f(x, y);
	}

	float Trig::acos(const float val)
	{
		return std::acosf(val);
	}

	float Trig::asin(const float val)
	{
		return std::asinf(val);
	}

	void Trig::cossin(float& cos, float& sin, const float angle_radians)
	{
		cos = std::cosf(angle_radians);
		sin = std::sinf(angle_radians);
	}

	float Trig::sqrt(const float val)
	{
		return std::sqrtf(val);
	}

	float Trig::rsqrt(const float val)
	{
		return (1.0f / std::sqrtf(val));
	}

}

//--- End of File ---
