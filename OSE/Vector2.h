#pragma once

#include <iostream>
#include "SFML/System.hpp"
#include "MathUtils.h"
#include "Box2D.h"

class Vector2
{
public:
	float x;
	float y;
	Vector2(void);
	Vector2(float one, float two);
	Vector2(sf::Vector2f vec);
	Vector2(const Vector2& vec);
	Vector2(const b2Vec2& vec);
	~Vector2();

	Vector2& operator = (const Vector2 &param);
	Vector2& operator = (const sf::Vector2f &param);
	Vector2 operator + (const Vector2);
	Vector2 operator - (const Vector2);
	Vector2 operator * (const float);
	Vector2 operator / (const float);
	bool operator==(const Vector2 &other) const;
	sf::Vector2f SF() {return sf::Vector2f(x,y);};
	b2Vec2 B2() {return b2Vec2(x,y);};

	static Vector2 Random(const float min, const float max); //return random vector
	float Dot(const Vector2 two);
	float Cross(const Vector2 two);
	float Length();
	float LengthSquared();

	Vector2 Rotate(const float deg);
	Vector2 Rotate(const float deg, const Vector2& origin);
	Vector2 Approach(const Vector2 &targ, const float rate);
	Vector2 Normalize();
	Vector2 Normal();
};

