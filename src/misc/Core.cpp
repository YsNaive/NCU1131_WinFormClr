#include "pch.h"
#include "Core.h"

Matrix2x2 Matrix2x2::FromRotation(float degree)
{
	Matrix2x2 ret;
	auto theta = degree * DEG2RAD;
	ret.m00 = cos(theta);
	ret.m10 = sin(theta);
	ret.m01 = -ret.m10;
	ret.m11 = ret.m00;
	return ret;
}
