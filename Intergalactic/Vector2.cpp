#include "Vector2.h"

Vector2::Vector2(void)
{
	x = 0.f;
	y = 0.f;
}

Vector2::Vector2(float One, float Two)
{
	x = One;
	y = Two;
}

Vector2::Vector2(const Vector2& vec)
{
	x = vec.x;
	y = vec.y;
}

Vector2::Vector2(const b2Vec2& vec)
{
	x = vec.x;
	y = vec.y;
}

Vector2::Vector2(sf::Vector2f vec)
{
	x = vec.x;
	y = vec.y;
}

Vector2::~Vector2()
{
}

Vector2& Vector2::operator= (const sf::Vector2f &param)
{
	x = param.x;
	y = param.y;
	return *this;
}

Vector2& Vector2::operator= (const Vector2 &param)
{
	if (this != &param)
	{
		x = param.x;
		y = param.y;
	}
	return *this;
}

Vector2 Vector2::operator* (float param)
{
	return Vector2(x * param, y * param);
}

Vector2 Vector2::operator/ (float param)
{
	return Vector2(x / param, y / param);
}

Vector2 Vector2::operator+ (Vector2 param)
{
	Vector2 temp;
	temp.x = x + param.x;
	temp.y = y + param.y;
	return temp;
}

Vector2 Vector2::operator- (Vector2 param)
{
	Vector2 temp;
	temp.x = x - param.x;
	temp.y = y - param.y;
	return temp;
}

Vector2 Vector2::Random(float min, float max)
{
	return Vector2(ig::Random(min, max), ig::Random(min, max));
}

Vector2 Vector2::Rotate(float deg)
{
	Vector2 nv = *this;
	deg = ig::DegToRad(deg);
	nv.x = (nv.x * std::cos(deg)) - (nv.y * std::sin(deg));
	nv.y = (nv.y * std::cos(deg)) + (nv.x * std::sin(deg));
	return nv;
}

Vector2 Vector2::Rotate(float deg, Vector2& origin)
{
	Vector2 nv = *this;
	nv = nv - origin;
	deg = ig::DegToRad(deg);
	nv.x = origin.x + (nv.x * std::cos(deg)) - (nv.y * std::sin(deg));
	nv.y = origin.y + (nv.y * std::cos(deg)) + (nv.x * std::sin(deg));
	return nv;
}

float Vector2::Dot(Vector2 two)
{
	return (x * two.x) + (y * two.y);
}

float Vector2::Cross(Vector2 two)
{
	return (x * two.y) - (two.x * y);
}

float Vector2::Length()
{
	return std::sqrt(x*x + y*y);
}

float Vector2::LengthSquared()
{
	return (x*x + y*y);
}

Vector2 Vector2::Normalize()
{
	float len = Length();
	if (len == 0) 
		return Vector2(0,0);
	return Vector2(x/len, y/len);
}

Vector2 Vector2::Approach(Vector2 &targ, float rate)
{
	Vector2 vec(x,y);
	vec.x = ig::Approach(x, targ.x, rate);
	vec.y = ig::Approach(y, targ.y, rate);
	return vec;
}

Vector2 Vector2::Normal()
{
	Vector2 n;
	n.x = (y * -1);
	n.y = x;
	return n;
}