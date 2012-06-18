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
	Vector2 operator + (Vector2);
	Vector2 operator - (Vector2);
	Vector2 operator * (float);
	Vector2 operator / (float);

	sf::Vector2f SF() {return sf::Vector2f(x,y);};
	b2Vec2 B2() {return b2Vec2(x,y);};

	static Vector2 Random(float min, float max); //return random vector
	float Dot(Vector2 two);
	float Cross(Vector2 two);
	float Length();
	float LengthSquared();

	Vector2 Rotate(float deg);
	Vector2 Rotate(float deg, Vector2& origin);
	Vector2 Approach(Vector2 &targ, float rate);
	Vector2 Normalize();
	Vector2 Normal();
};

