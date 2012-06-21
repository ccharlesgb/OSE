#include "MathUtils.h"
#include "Randomiser.h"
#include <iostream>
#include <time.h>
#include <cmath>
#include "Vector2.h"
#include "../GameGlobals.h"

namespace ig
{
	
	MTRand_closed Rand;
	
	void SeedRandom()
	{
		Rand.seed(long(time(0)));
	}
	
	float Random(float min, float max)
	{
		float frac = Rand();
		return (frac * (max-min)) + min;
	}
	
	int RandomInt(int min, int max)
	{
		float frac = Rand();
		return std::floor(((frac * (max-min)) + min) + 0.5f);
	}
	
	float RadToDeg(float Rad)
	{
		return float((180.f / M_PI) * Rad);
	}
	
	float DegToRad(float Deg)
	{
		return float((M_PI / 180.f) * Deg);
	}
	
	float NormalizeAngle(float Deg)
	{
		return Deg - (360 * (std::floor((Deg + 180)/360)));
	}
	
	float Approach(float Current, float Target, float Inc)
	{
		Inc = fabs(Inc);
		
		if (Current < Target)
		{
			return std::min(Target, Current + Inc);
		}
		if (Current > Target)
		{
			return std::max(Target, Current - Inc);
		}
		return Current;
	}
	
	float RotateTowards(float ang , float targ, float speed)
	{
		if (ang == targ)
			return ang;
		//if (abs(((((argument0-argument1) mod 360)+540) mod 360)-180) >= 179) {
		//return argument0+1;
		
		return ang+(std::sin(DegToRad(targ-ang))*speed);
	}
	
	float Limit(float var, float min, float max)
	{
		if (var < min)
			var = min;
		if (var > max)
			var = max;
		return var;
	}
	
	Vector2 GameToSFML(Vector2 Pos)
	{
		sf::Vector2<double> dPos;
		dPos.x = Pos.x;
		dPos.y = Pos.y;
		dPos.y *= -1;
		sf::Vector2<double> ScreenCentre = sf::Vector2<double>(gGlobals.GameWidth / 2, gGlobals.GameHeight / 2);
		
		dPos = dPos + ScreenCentre;
		
		Pos.x = dPos.x;
		Pos.y = dPos.y;
		
		return Pos.SF();
	}
	
	Vector2 AngleToVector(float angle)
	{
		return Vector2(std::cos(angle), std::sin(angle));
	}
	
}