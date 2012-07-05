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
	
	//Seed the random generator to make sure its different every time
	void SeedRandom()
	{
		Rand.seed(long(time(0)));
	}
	
	float Random(float min, float max)
	{
		float frac = (float)Rand();
		return (frac * (max-min)) + min;
	}
	
	int RandomInt(int min, int max)
	{
		float frac = (float)Rand();
		return (int)ig::Floor(((frac * (max-min)) + min) + 0.5f);
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
		return Deg - (360 * (ig::Floor((Deg + 180)/360)));
	}
	
	float Approach(float Current, float Target, float Inc)
	{
		Inc = fabs(Inc);
		
		if (Current < Target)
		{
			return ig::Min(Target, Current + Inc);
		}
		if (Current > Target)
		{
			return ig::Max(Target, Current - Inc);
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
		var = (var < min) ? min : var;
		var = (var > max) ? max : var;
		return var;
	}

	Vector2 GameToSFML(Vector2 Pos)
	{
		Pos.y *= -1;
		Vector2 ScreenCentre = Vector2(gGlobals.GameWidth / 2.f, gGlobals.GameHeight / 2.f);
		
		Pos = Pos + ScreenCentre;
		
		return Pos;
	}

	Vector2 SFMLToGame(Vector2 Pos)
	{
		Pos.y *= -1;
		Vector2 ScreenCentre = Vector2(gGlobals.GameWidth / 2, gGlobals.GameHeight / 2);
		
		Pos = Pos - ScreenCentre;
		
		return Pos;
	}
	
	Vector2 AngleToVector(float angle)
	{
		return Vector2(std::cos(angle), std::sin(angle));
	}
	
	Vector2 NearestGrid(Vector2 pos, int grid_size_x , int grid_size_y)
	{
		Vector2 result;
		result.x = ig::Floor((pos.x / grid_size_x) + 0.5f);
		result.y = ig::Floor((pos.y / grid_size_y) + 0.5f);
		return result;
	}
}