
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

const Anchor Anchor::UpperLeft    = Anchor(0);
const Anchor Anchor::UpperCenter  = Anchor(1);
const Anchor Anchor::UpperRight   = Anchor(2);
const Anchor Anchor::MiddleLeft   = Anchor(3);
const Anchor Anchor::MiddleCenter = Anchor(4);
const Anchor Anchor::MiddleRight  = Anchor(5);
const Anchor Anchor::LowerLeft    = Anchor(6);
const Anchor Anchor::LowerCenter  = Anchor(7);
const Anchor Anchor::LowerRight   = Anchor(8);

Vector2 Anchor::GetPosition(const Rect& rect)
{
	if (*this == Anchor::UpperLeft)
		return { rect.get_xMin()    , rect.get_yMin() };
	if (*this == Anchor::UpperCenter)
		return { rect.get_center().x, rect.get_yMin() };
	if (*this == Anchor::UpperRight)
		return { rect.get_xMax()    , rect.get_yMin() };
	if (*this == Anchor::MiddleLeft)
		return { rect.get_xMin()    , rect.get_center().y };
	if (*this == Anchor::MiddleCenter)
		return rect.get_center();
	if (*this == Anchor::MiddleRight)
		return { rect.get_xMax()    , rect.get_center().y };
	if (*this == Anchor::LowerLeft)
		return { rect.get_xMin()    , rect.get_yMax() };
	if (*this == Anchor::LowerCenter)
		return { rect.get_center().x, rect.get_yMax() };
	if (*this == Anchor::LowerRight)
		return { rect.get_xMax()    , rect.get_yMax() };
	return { 0,0 };
}
